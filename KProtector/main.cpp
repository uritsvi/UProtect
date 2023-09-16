#include "..\Common\Common.h"

#include "MiniFilter.h"

#include "Driver.h"
#include "Registry.h"
#include "RAIIRegistry.h"
#include "Processes.h"
#include "GlobalState.h"

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

	//Processes::GetInstance()->ShutDown();
}

NTSTATUS CompleteReq(
	_In_ IRP* Irp,
	_In_ NTSTATUS Status = STATUS_SUCCESS,
	_In_ int Info = 0) {

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

	auto sp = IoGetCurrentIrpStackLocation(Irp);
	auto dic = sp->Parameters.DeviceIoControl;

	NTSTATUS status = STATUS_SUCCESS;

	do {

	if (!Processes::GetInstance()->IsControlApp(NtCurrentProcess())) {
		status = STATUS_ACCESS_DENIED;
		break;
	}


	switch (dic.IoControlCode)
	{
	case IOCTL_RELOAD_MINIFILTER_POLICY: {
		KdPrint(("IOCTL"));
		status = MiniFilter::GetInstance()->ReloadPolicy();
	}break;

	case IOCTL_DLL_LOAD: {
		HANDLE processHandle = PsGetCurrentProcessId();
		ULONG pid = HandleToUlong(processHandle);

		Processes::GetInstance()->SetControlAppPID(pid);
	}break;

	case IOCTL_DLL_UNLOAD: {
		Processes::GetInstance()->OnControlAppUnload();

	}break;

	case IOCTL_DRIVER_UNLOAD: {
		GlobalState::GetInstance()->UnloadDriver();

	}break;
	case IOCTL_DRIVER_LOAD: {
		GlobalState::GetInstance()->LoadDriver();
	}break;
	}
	} while (false);

	return CompleteReq(Irp, status);
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

		status = Processes::GetInstance()->Init();

	} while (false);

	return status;
}

