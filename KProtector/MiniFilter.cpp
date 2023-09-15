#include "MiniFilter.h"
#include "..\Common\Common.h"
#include "..\KTL\include\KTLMemory.hpp"

DRIVER_OBJECT* Object;

MiniFilter* MiniFilter::m_Instance;

void CleanUp(_In_ DRIVER_OBJECT* Driver);

NTSTATUS Unload(
	FLT_FILTER_UNLOAD_FLAGS Flags) {

	//CleanUp(Object);

	UNREFERENCED_PARAMETER(Flags);

	return STATUS_SUCCESS;
}

NTSTATUS InstanceSetUp(
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ FLT_INSTANCE_SETUP_FLAGS Flags,
	_In_ DEVICE_TYPE VolumeDeviceType,
	_In_ FLT_FILESYSTEM_TYPE VolumeFilesystemType) {

	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);
	UNREFERENCED_PARAMETER(VolumeDeviceType);

	if (VolumeFilesystemType == FLT_FSTYPE_NTFS) {
		return STATUS_SUCCESS;
	}

	return STATUS_FLT_DO_NOT_ATTACH;
}

FLT_PREOP_CALLBACK_STATUS PreCreateCallback(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Outptr_result_maybenull_ PVOID* CompletionContext) {

	UNREFERENCED_PARAMETER(CompletionContext);
	UNREFERENCED_PARAMETER(FltObjects);

	FLT_PREOP_CALLBACK_STATUS status =
		FLT_PREOP_SUCCESS_NO_CALLBACK;
	
	do {
		if (Data->RequestorMode == KernelMode) {
			return status;
		}

		auto params = Data->Iopb->Parameters.Create;
		if (params.Options & FILE_DELETE_ON_CLOSE) {


			KdPrint(("Create\n"));

			if (FltObjects->FileObject == nullptr) {
				break;
			}

			auto fileName =
				&FltObjects->FileObject->FileName;
			if (!MiniFilter::GetInstance()->AllowToModify(fileName)) {
				status = FLT_PREOP_COMPLETE;
			}

			// TODO: Check if delete is allowed
		}
	} while (false);

	return status;
}

FLT_PREOP_CALLBACK_STATUS PreWriteCallback(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Outptr_result_maybenull_ PVOID* CompletionContext) {

	UNREFERENCED_PARAMETER(CompletionContext);
	UNREFERENCED_PARAMETER(FltObjects);


	FLT_PREOP_CALLBACK_STATUS finalStatus =
		FLT_PREOP_SUCCESS_NO_CALLBACK;

	PFLT_FILE_NAME_INFORMATION fileNameInfo;
	do {
		NTSTATUS status = FltGetFileNameInformation(
			Data,
			FLT_FILE_NAME_QUERY_DEFAULT | FLT_FILE_NAME_NORMALIZED,
			&fileNameInfo);
		if (!NT_SUCCESS(status)) {
			finalStatus = FLT_PREOP_COMPLETE;
			break;
		}

		KdPrint(("Write\n"));
		if (!MiniFilter::GetInstance()->AllowToModify(&fileNameInfo->Name)) {
			Data->IoStatus.Status = STATUS_ACCESS_DENIED;
			finalStatus = FLT_PREOP_COMPLETE;
		}

		FltReleaseFileNameInformation(fileNameInfo);


	} while (false);

	return finalStatus;

}

FLT_PREOP_CALLBACK_STATUS PreSetInfoCallback(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Outptr_result_maybenull_ PVOID* CompletionContext) {

	UNREFERENCED_PARAMETER(CompletionContext);
	UNREFERENCED_PARAMETER(FltObjects);

	FLT_PREOP_CALLBACK_STATUS finalStatus =
		FLT_PREOP_SUCCESS_NO_CALLBACK;

	if (Data->RequestorMode == KernelMode) {
		return finalStatus;
	}

	auto& params =
		Data->Iopb->Parameters.SetFileInformation;

	do {

	if (params.FileInformationClass == FileDispositionInformation ||
		params.FileInformationClass == FileDispositionInformationEx) {

		auto info =
			(FILE_DISPOSITION_INFORMATION*)params.InfoBuffer;

		PFLT_FILE_NAME_INFORMATION fileNameInfo;

		if (info->DeleteFile & 1) {
			KdPrint(("Set Info\n"));

			NTSTATUS status = FltGetFileNameInformation(
				Data,
				FLT_FILE_NAME_QUERY_DEFAULT | FLT_FILE_NAME_NORMALIZED,
				&fileNameInfo);
			if (!NT_SUCCESS(status)) {
				KdPrint(("Failed to get file name"));
				break;
			}


			// TODO: Check if delete is allowed
			bool res = 
				MiniFilter::GetInstance()->AllowToModify(&fileNameInfo->Name);
			if (!res) {
				Data->IoStatus.Status = STATUS_ACCESS_DENIED;
				finalStatus = FLT_PREOP_COMPLETE;
			}

			FltReleaseFileNameInformation(fileNameInfo);
		}
	}
	else if (params.FileInformationClass == FileRenameInformation ||
			 params.FileInformationClass == FileRenameInformationEx) {
		PFLT_FILE_NAME_INFORMATION fileNameInfo;

			NTSTATUS status = FltGetFileNameInformation(
				Data,
				FLT_FILE_NAME_QUERY_DEFAULT | FLT_FILE_NAME_NORMALIZED,
				&fileNameInfo);
			if (!NT_SUCCESS(status)) {
				KdPrint(("Failed to get file name"));
				break;
			}
			// TODO: Check if rename is allowed
			bool res =
				MiniFilter::GetInstance()->AllowToModify(&fileNameInfo->Name);
			if (!res) {
				Data->IoStatus.Status = STATUS_ACCESS_DENIED;
				finalStatus = FLT_PREOP_COMPLETE;
			}


	}
	} while (false);


	return finalStatus;
}

NTSTATUS TeardownQuery(
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags) {

	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Flags);

	return STATUS_SUCCESS;
}

void TeardownStart(
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ FLT_INSTANCE_TEARDOWN_FLAGS Reason) {

	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Reason);
}

void TeardownComplete(
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ FLT_INSTANCE_TEARDOWN_FLAGS Reason) {

	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(Reason);
}

MiniFilter* MiniFilter::GetInstance() {
	
	if (m_Instance == nullptr) {
		m_Instance = new(POOL_FLAG_PAGED, DRIVER_TAG)MiniFilter();
	}

	return m_Instance;
}

void MiniFilter::Delete() {

}

MiniFilter::~MiniFilter() {
	FltUnregisterFilter(m_Filter);
}
MiniFilter::MiniFilter() {
	m_AhoCorasick = 
		new (POOL_FLAG_NON_PAGED, DRIVER_TAG)AhoCorasickInterface();
}


/* TODO: Make the installer set this data */
void MiniFilter::tempInitRegistry(UNICODE_STRING* RegistryPath) {
	HANDLE hKey = nullptr, hSubKey = nullptr;
	NTSTATUS status;
	OBJECT_ATTRIBUTES keyAttr = RTL_CONSTANT_OBJECT_ATTRIBUTES(
		RegistryPath, OBJ_KERNEL_HANDLE);
	status = ZwOpenKey(&hKey, KEY_WRITE, &keyAttr);
	UNICODE_STRING subKey = RTL_CONSTANT_STRING(L"Instances");
	OBJECT_ATTRIBUTES subKeyAttr;
	InitializeObjectAttributes(&subKeyAttr, &subKey, OBJ_KERNEL_HANDLE, hKey,
		nullptr);
	status = ZwCreateKey(&hSubKey, KEY_WRITE, &subKeyAttr, 0, nullptr, 0, nullptr);

	UNICODE_STRING valueName = RTL_CONSTANT_STRING(L"DefaultInstance");
	WCHAR name[] = L"DelProtectDefaultInstance";
	status = ZwSetValueKey(hSubKey, &valueName, 0, REG_SZ, name, sizeof(name));

	UNICODE_STRING instKeyName;
	RtlInitUnicodeString(&instKeyName, name);
	HANDLE hInstKey;
	InitializeObjectAttributes(&subKeyAttr, &instKeyName, OBJ_KERNEL_HANDLE,
		hSubKey, nullptr);
	status = ZwCreateKey(&hInstKey, KEY_WRITE, &subKeyAttr, 0, nullptr, 0, nullptr);

	WCHAR altitude[] = L"425342";
	UNICODE_STRING altitudeName = RTL_CONSTANT_STRING(L"Altitude");
	status = ZwSetValueKey(hInstKey, &altitudeName, 0, REG_SZ,
		altitude, sizeof(altitude));

	UNICODE_STRING flagsName = RTL_CONSTANT_STRING(L"Flags");
	ULONG flags = 0;
	status = ZwSetValueKey(hInstKey, &flagsName, 0, REG_DWORD,
		&flags, sizeof(flags));
	ZwClose(hInstKey);

}


NTSTATUS MiniFilter::InitMiniFilter(DRIVER_OBJECT* Driver) {


	NTSTATUS status = STATUS_SUCCESS;

	do {

		FLT_OPERATION_REGISTRATION const callbacks[] = {
			{IRP_MJ_CREATE, 0, PreCreateCallback , nullptr},
			{IRP_MJ_SET_INFORMATION, 0, PreSetInfoCallback , nullptr},
			{IRP_MJ_WRITE, 0, PreWriteCallback, nullptr},
			{IRP_MJ_OPERATION_END},
		};

		FLT_REGISTRATION const reg = {
			sizeof(FLT_REGISTRATION),
			FLT_REGISTRATION_VERSION,
			0,
			nullptr,
			callbacks,
			Unload,
			InstanceSetUp,
			TeardownQuery,
			TeardownStart,
			TeardownComplete
		};

		status = FltRegisterFilter(
			Driver,
			&reg,
			&m_Filter);

		if (!NT_SUCCESS(status)) {
			break;
		}
		bool res =
			m_AhoCorasick->Init(DRIVER_MINI_FILTER_INFO_ROOT_PATH);
		if (!res) {
			status = STATUS_NOT_FOUND;
		}


	} while (false);


	KdPrint(("Finish Init"));

	return status;
}

NTSTATUS MiniFilter::Init(
	_In_ DRIVER_OBJECT* Driver, 
	_In_ UNICODE_STRING* Registry) {
	
	Object = Driver;

	tempInitRegistry(Registry);
	auto status = InitMiniFilter(
		Driver);

	
	return status;

}

bool MiniFilter::AllowToModify(_In_ UNICODE_STRING* Path) {
	return !m_AhoCorasick->Match(Path);
}

NTSTATUS MiniFilter::StartProtect() {
	return FltStartFiltering(m_Filter);
}

NTSTATUS MiniFilter::ReloadPolicy() {
	return m_AhoCorasick->ReloadPolicy(DRIVER_MINI_FILTER_INFO_ROOT_PATH);
}

