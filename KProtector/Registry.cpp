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

NTSTATUS RegistryBlocker::AllowToModify(_In_ PVOID Object) {
	PCUNICODE_STRING name;
	NTSTATUS status;

	do {
		status =
			CmCallbackGetKeyObjectIDEx(
				&m_Cookie,
				Object,
				nullptr,
				&name,
				0);
		if (!NT_SUCCESS(status)) {
			KdPrint(("Error in getting key name"));
			break;
		}


		



		MatchContext context = { 0 };
		if (m_AhoCorasickInterface.Match((UNICODE_STRING*)name)) {

			KdPrint(("Blocked key %wZ", name));

			status = STATUS_ACCESS_DENIED;
		}

		CmCallbackReleaseKeyObjectIDEx(name);


	} while (false);

	return status;
}

NTSTATUS RegistryBlocker::SelfRegistryCallback(
	_In_opt_ PVOID Argument1,
	_In_opt_ PVOID Argument2) {

	UNREFERENCED_PARAMETER(Argument1);
	UNREFERENCED_PARAMETER(Argument2);


	NTSTATUS status = STATUS_SUCCESS;

	PVOID object = { 0 };
	switch ((REG_NOTIFY_CLASS)(ULONG_PTR)Argument1)
	{
	case RegNtPreDeleteValueKey: {
		auto params =
			reinterpret_cast<REG_DELETE_VALUE_KEY_INFORMATION*>(Argument2);
		object = params->Object;
	}break;

	case RegNtPreRenameKey: {
		auto params =
			reinterpret_cast<REG_RENAME_KEY_INFORMATION*>(Argument2);
		object = params->Object;
	}break;

	case RegNtPreSetValueKey: {

		auto params =
			reinterpret_cast<REG_SET_VALUE_KEY_INFORMATION*>(Argument2);
		object = params->Object;
	}

	case RegNtDeleteKey: {
		auto params =
			reinterpret_cast<REG_DELETE_KEY_INFORMATION*>(Argument2);
		object = params->Object;
	}break;
	}
	if (object != nullptr) {
		status = AllowToModify(object);
	}
	return status;
}