#include "..\Common\Common.h"

#include "Registry.h"
#include "RAIIRegistry.h"
#include "..\KTL\include\KTLMemory.hpp"

#define MAX_REG_PATH 256

NTSTATUS RegistryCallback(
	_In_ PVOID CallbackContext,
	_In_opt_ PVOID Argument1,
	_In_opt_ PVOID Argument2) {

	auto instance = 
		reinterpret_cast<RegistryBlocker*>(CallbackContext);

	return instance->SelfRegistryCallback(
		Argument1, 
		Argument2);

}

NTSTATUS RegistryBlocker::CreateRegistryBlocker(_Out_ RegistryBlocker **Blocker) {
	NTSTATUS status = STATUS_SUCCESS;

	do {
		if (Blocker == nullptr) {
			status = STATUS_INVALID_ADDRESS;
			break;
		}

		auto out = 
			new (POOL_FLAG_PAGED, DRIVER_TAG)RegistryBlocker();

		if (!NT_SUCCESS(status)) {
			break;
		}

		*Blocker = out;


	} while (false);

	return status;
}

RegistryBlocker::RegistryBlocker() {
	m_Cookie = { 0 };
	m_AhoCorasickInterface = AhoCorasickInterface();
}

bool RegistryBlocker::Init() {
	return m_AhoCorasickInterface.Init(DRIVER_REG_INFO_ROOT_PATH);
}


RegistryBlocker::~RegistryBlocker() {
	CmUnRegisterCallback(m_Cookie);
}

void RegistryBlocker::StartProtect() {
	NTSTATUS status = STATUS_SUCCESS;

	status = CmRegisterCallback(
		RegistryCallback,
		this,
		&m_Cookie);
	if (!NT_SUCCESS(status)) {
		KdPrint(("Failed to registr registry callback"));
	}
}

NTSTATUS RegistryBlocker::SelfRegistryCallback(
	_In_opt_ PVOID Argument1,
	_In_opt_ PVOID Argument2) {

	UNREFERENCED_PARAMETER(Argument1);
	UNREFERENCED_PARAMETER(Argument2);


	NTSTATUS status = STATUS_SUCCESS;


	
		switch ((REG_NOTIFY_CLASS)(ULONG_PTR)Argument1)
		{
			case RegNtPreDeleteValueKey: {
				auto params =
					reinterpret_cast<REG_DELETE_VALUE_KEY_INFORMATION*>(Argument2);


				PCUNICODE_STRING name;

				status = 
					CmCallbackGetKeyObjectIDEx(
						&m_Cookie, 
						params->Object, 
						nullptr, 
						&name, 
						0);
				if (!NT_SUCCESS(status)) {
					KdPrint(("Error in getting key name"));
					break;
				}

				WCHAR keyName[MAX_REG_PATH] = {0};
				memcpy(keyName, name->Buffer, name->Length);

				CmCallbackReleaseKeyObjectIDEx(name);

				MatchContext context = {0};
				if (m_AhoCorasickInterface.Match((const WCHAR*)keyName)) {

					KdPrint(("Blocked key %wZ", name));

					return STATUS_ACCESS_DENIED;
				}

			}break;
		}
		
	
	return status;
}