#include "MiniFilter.h"

#include "Driver.h"
#include "Registry.h"
#include "RAIIRegistry.h"

#include "..\KTL\include\KTLMemory.hpp"

#define DEV_NAME L"\\Device\\KProtect"
#define SYM_LINK L"\\??\\KProtect"

RegistryBlocker* g_Registry;

void CleanUp(_In_ DRIVER_OBJECT* Driver) {
	/*
	if (g_Filter) {
		delete g_Filter;
	}
	*/

	FltUnregisterFilter(MiniFilter::GetInstance()->m_Filter);
	
	if (Driver->DeviceObject != nullptr) {
		UNICODE_STRING symLink = 
			RTL_CONSTANT_STRING(SYM_LINK);

		IoDeleteDevice(Driver->DeviceObject);
		IoDeleteSymbolicLink(&symLink);
	}

	delete g_Registry;
}

NTSTATUS CompleteReq(
	_In_ IRP* Irp, 
	_In_ NTSTATUS Status=STATUS_SUCCESS, 
	_In_ int Info=0) {
	
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = Info;

	IoCompleteRequest(
		Irp, 
		IO_NO_INCREMENT);

	return Status;
}

NTSTATUS CreateClose(
	_In_ DEVICE_OBJECT* Device,
	_In_ IRP* Irp) {

	UNREFERENCED_PARAMETER(Device);

	return CompleteReq(Irp);
}

NTSTATUS Control(
	_In_ DEVICE_OBJECT* Device, 
	_In_ IRP* Irp) {

	UNREFERENCED_PARAMETER(Device);

	return CompleteReq(Irp);
}

void Unload(_In_ DRIVER_OBJECT* Driver) {
	CleanUp(Driver);
}

extern "C" NTSTATUS DriverEntry(
	_In_ DRIVER_OBJECT * Driver, 
	_In_ UNICODE_STRING * Registry) {
	

	UNREFERENCED_PARAMETER(Registry);

	KdPrint(("Start"));

	NTSTATUS status = STATUS_SUCCESS;

	do {
		UNICODE_STRING devName = 
			RTL_CONSTANT_STRING(DEV_NAME);
		
		UNICODE_STRING symLink = 
			RTL_CONSTANT_STRING(SYM_LINK);

		DEVICE_OBJECT* dev;
		status = IoCreateDevice(
			Driver, 
			0, 
			&devName, 
			FILE_DEVICE_UNKNOWN, 
			0, 
			FALSE, 
			&dev);
		if (!NT_SUCCESS(status)) {
			CleanUp(Driver);

			KdPrint(("Failed to create dev object"));
			break;
		}
		
		status = IoCreateSymbolicLink(
			&symLink, 
			&devName);
		if (!NT_SUCCESS(status)) {
			CleanUp(Driver);

			KdPrint(("Failed to create sym link"));
			break;
		}

		dev->Flags |= DO_BUFFERED_IO;


#ifdef DBG
		Driver->DriverUnload = Unload;
#endif
		Driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Control;
		Driver->MajorFunction[IRP_MJ_CREATE] = CreateClose;
		Driver->MajorFunction[IRP_MJ_CLOSE] = CreateClose;

		init_aho_corasick(
			[](size_t size) {
				return 
					ExAllocatePool2(
					POOL_FLAG_PAGED, 
					size, 
					DRIVER_TAG); 
			},
			[](void* mem) {
				ExFreePool(mem);
			},
			[](void* dest, const void* src, size_t count) {
					RtlCopyMemory(dest, src, count);
			}
		);

		
		status = 
			RegistryBlocker::CreateRegistryBlocker(&g_Registry);
		if (!NT_SUCCESS(status)) {
			CleanUp(Driver);
			break;
		}
		status = 
			g_Registry->Init();
		if (!NT_SUCCESS(status)) {
			CleanUp(Driver);
			break;
		}
	
		status = MiniFilter::GetInstance()->Init(
			Driver, 
			Registry);
		if (!NT_SUCCESS(status)) {
			CleanUp(Driver);
			KdPrint(("Failed to init mini filter"));
			break;
		}

		status = 
			MiniFilter::GetInstance()->StartProtect();
		if (!NT_SUCCESS(status)) {
			KdPrint(("Failed to start filtering"));
			break;
		}
		g_Registry->StartProtect();

	} while (false);

	return status;
}

