#include "Driver.h"
#include "Processes.h"

#include "..\KTL\include\KTLMemory.hpp"

Processes* Processes::m_Instance;

/*
void PcreateProcessNotifyRoutineEx(
	_Inout_           PEPROCESS Process,
	_In_                HANDLE ProcessId,
	_Inout_ PPS_CREATE_NOTIFY_INFO CreateInfo) {

	Processes::GetInstance()->SelfPcreateProcessNotifyRoutineEx(
		Process, 
		ProcessId, 
		CreateInfo
	);
}



NTSTATUS Processes::Init() {
	NTSTATUS status = PsSetCreateProcessNotifyRoutineEx(
		PcreateProcessNotifyRoutineEx, 
		FALSE
	);

	return status;
}

void Processes::ShutDown() {
	PsSetCreateProcessNotifyRoutineEx(
		PcreateProcessNotifyRoutineEx,
		TRUE
	);
}

void Processes::SelfPcreateProcessNotifyRoutineEx(
	_Inout_           PEPROCESS Process,
	_In_                HANDLE ProcessId,
	_Inout_ PPS_CREATE_NOTIFY_INFO CreateInfo) {

	UNREFERENCED_PARAMETER(Process);

	if (CreateInfo == nullptr) {
		if (g_AllowedProcessId == (LONG64)HandleToULong(ProcessId)) {
			g_AllowedProcessId = NULL_PROCESS_ID;
		}

		return;
	}

	if (g_AllowedProcessId != NULL_PROCESS_ID) {
		return;
	}

	if (CreateInfo->ImageFileName == nullptr) {
		return;
	}

	auto path = L"\\??\\C:\\bin";
	if (memcmp(path, CreateInfo->ImageFileName->Buffer, (wcslen(path) * sizeof(wchar_t))) == 0) {
		g_AllowedProcessId = (LONG64)ProcessId;
	}

}
*/

/*
* TODO add locks
*/

typedef NTSTATUS (*ZwQueryInformationProcess)(
	_In_      HANDLE           ProcessHandle,
	_In_      PROCESSINFOCLASS ProcessInformationClass,
	_Out_     PVOID            ProcessInformation,
	_In_      ULONG            ProcessInformationLength,
	_Out_opt_ PULONG           ReturnLength
);

ZwQueryInformationProcess ZwQueryInformationProcessPtr;

Processes* Processes::GetInstance() {
	if (m_Instance == nullptr) {
		m_Instance = new (POOL_FLAG_PAGED, DRIVER_TAG)Processes();
	}
	return m_Instance;
}

Processes::Processes() {

	m_ControlAppIsLoaded = false;
}

NTSTATUS Processes::Init() {

	UNICODE_STRING functionName =
		RTL_CONSTANT_STRING(L"ZwQueryInformationProcess");
	ZwQueryInformationProcessPtr =
		(ZwQueryInformationProcess)MmGetSystemRoutineAddress(&functionName);

	if (ZwQueryInformationProcessPtr == nullptr) {
		return STATUS_NOT_FOUND;
	}
	return STATUS_SUCCESS;

}

Processes::~Processes() {
}

bool Processes::IsControlApp(_In_ HANDLE Process) {
	
	bool res = false;
	
	do {
		UNICODE_STRING* processName = (UNICODE_STRING*)ExAllocatePool2(
			POOL_FLAG_PAGED,
			1024,
			DRIVER_TAG
		);

		NTSTATUS status = ZwQueryInformationProcessPtr(
			Process,
			ProcessImageFileName,
			processName,
			1024,
			nullptr);
		if (NT_SUCCESS(status)) {
			auto path = L"\\Device\\HarddiskVolume3\\bin";
			if (memcmp(path, processName->Buffer, wcslen(path) * sizeof(wchar_t)) == 0) {
				res = true;
				break;
			}
			res = false;
		}

		ExFreePool(processName);
	} while (false);


	return res;
}
void Processes::SetControlAppPID(_In_ ULONG PID) {
	m_AppPID = PID;
	m_ControlAppIsLoaded = true;
}

void Processes::OnControlAppUnload() {
	m_ControlAppIsLoaded = false;
}

bool Processes::IsProcessAllowedAccess(_In_ ULONG PID) {
	if (m_ControlAppIsLoaded && m_AppPID == PID) {
		return true;
	}
	return false;
}