#include "Driver.h"
#include "Processes.h"

#include "..\KTL\include\KTLMemory.h"
#include "..\Common\Common.h"


Processes* Processes::m_Instance;

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

	m_FastMutex.Init();

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
			DEFAULT_BUFFER_SIZE,
			DRIVER_TAG
		);

		NTSTATUS status = ZwQueryInformationProcessPtr(
			Process,
			ProcessImageFileName,
			processName,
			DEFAULT_BUFFER_SIZE,
			nullptr);
		if (NT_SUCCESS(status)) {
			auto path = ALL_PROGRAMS_INSTALL_PATH;
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
	m_FastMutex.Lock();
	
	m_AppPID = PID;
	m_ControlAppIsLoaded = true;

	m_FastMutex.Release();
}

void Processes::OnControlAppUnload() {
	m_FastMutex.Lock();

	m_ControlAppIsLoaded = false;

	m_FastMutex.Release();
}

bool Processes::IsProcessAllowedAccess(_In_ ULONG PID) {
	
	m_FastMutex.Lock();

	bool res = false;
	if (m_ControlAppIsLoaded && m_AppPID == PID) {
		res = true;
	}

	m_FastMutex.Release();

	return res;
}