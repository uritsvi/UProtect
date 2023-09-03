#include "..\Common\Common.h"


#include "Registry.h"
#include "RAIIRegistry.h"
#include "..\KTL\include\KTLMemory.hpp"

/*
* I include this file last becaus it is dependent on 
* ntddk.h witch should be included first
*/
#include <ntstrsafe.h>

#define NUM_OF_CHARS_FOR_SIZE 6
#define MAX_REG_PATH 256

struct ReadChuk {
	KEY_VALUE_PARTIAL_INFORMATION Info;
	char Data[DEFAULT_BUFFER_SIZE];
};



NTSTATUS RegistryBlocker::LoadAchoCorasickTrie() {
	
	NTSTATUS status = STATUS_SUCCESS;
		auto key =
			RAIIReigstryKey(
				nullptr,
				DRIVER_REG_INFO_ROOT_PATH,
				KEY_READ);

		if (key.FailedToCreate()) {
			KdPrint(("Failed to open reg key"));
			status = STATUS_NOT_FOUND;
			goto end;
		}

		KdPrint(("Successfuly opened a reg key"));

		int size;
		bool res =
			key.ReadValueDWORD(
				REG_TRIE_SIZE_VALUE_NAME, 
				&size);

		if (!res) {
			KdPrint(("Failed to read trie size"));
			status = STATUS_NOT_FOUND;
			goto end;
		}

		LONG_PTR range;
		res =
			key.ReadValueQWORD(
				REG_TRIE_WCHAR_RANGE_VALUE_NAME, 
				&range);
		if (!res) {
			KdPrint(("Failed to read trie range"));
			status = STATUS_NOT_FOUND;
			goto end;
		}

		m_WCharRange.Min = (int)range;
		m_WCharRange.Max = (range >> 32);

		KdPrint(("Trie size:%d", size));

		m_BlockTrie = (FinalTrieEntry*)ExAllocatePool2(
			POOL_FLAG_PAGED, 
			size, 
			DRIVER_TAG);
		if (m_BlockTrie == nullptr) {
			status = STATUS_NO_MEMORY;
			goto end;
		}

		int c = 0;
		for (int i = 0; i < size; i += DEFAULT_BUFFER_SIZE) {
			wchar_t Name[NUM_OF_CHARS_FOR_SIZE] = {0};
			status = RtlStringCbPrintfW(
				Name, 
				sizeof(Name), 
				L"%d", 
				c++);
			if (!NT_SUCCESS(status)) {
				KdPrint(("Failed to convert a decimal to a string"));
				goto end;
			}

			ReadChuk chunk;
			res = key.ReadValue(
				Name, 
				&chunk.Info,
				sizeof(chunk));
			if (!res) {
				status = STATUS_REGISTRY_IO_FAILED;
				goto end;
			}

			memcpy(
				((char*)m_BlockTrie + i),
				chunk.Info.Data, 
				chunk.Info.DataLength);
		}

end:
	return status;
}

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

		status =  out->LoadAchoCorasickTrie();
		if (!NT_SUCCESS(status)) {
			break;
		}

		*Blocker = out;


	} while (false);

	return status;
}

RegistryBlocker::RegistryBlocker() {
	m_Cookie = { 0 };
	m_BlockTrie = { 0 };
}

RegistryBlocker::~RegistryBlocker() {
	CmUnRegisterCallback(m_Cookie);
	delete m_BlockTrie;
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
				if (match(
					m_BlockTrie,
					m_BlockTrie,
					keyName,
					&context,
					&m_WCharRange)) {

					KdPrint(("Blocked key %wZ", name));

					return STATUS_ACCESS_DENIED;
				}

			}break;
		}
		
	
	return status;
}