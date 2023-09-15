#include "AhoCorasickInterface.h"
#include "..\Common\Common.h"
#include "..\KTL\include\KTLMemory.hpp"

/*
* I include this file last becaus it is dependent on
* ntddk.h witch should be included first
*/
#include <ntstrsafe.h>

FAST_MUTEX mutex;

#define NUM_OF_CHARS_FOR_SIZE 6

struct ReadChuk {
	KEY_VALUE_PARTIAL_INFORMATION Info;
	char Data[DEFAULT_BUFFER_SIZE];
};

NTSTATUS AhoCorasickInterface::LoadAchoCorasickTrie(_In_ const WCHAR* Root) {

	NTSTATUS status = STATUS_SUCCESS;
	auto key =
		RAIIReigstryKey(
			nullptr,
			Root,
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

	if (size == 0) {
		m_BlockTrie = nullptr;
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
		wchar_t Name[NUM_OF_CHARS_FOR_SIZE] = { 0 };
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

AhoCorasickInterface::AhoCorasickInterface() {
	m_BlockTrie = nullptr;
	m_WCharRange = { 0 };

	m_EResource = new(POOL_FLAG_NON_PAGED, DRIVER_TAG)ExecutiveResource();

	ExInitializeFastMutex(&mutex);
}
AhoCorasickInterface::~AhoCorasickInterface() {
	m_EResource->Delete();
	delete m_EResource;

	if (m_BlockTrie) {
		delete m_BlockTrie;
	}
}

bool AhoCorasickInterface::Init(_In_ const WCHAR* RootKey) {
	m_EResource->Init();
	
	if (NT_SUCCESS(LoadAchoCorasickTrie(RootKey))) {
		return true;
	}
	return false;
}

bool AhoCorasickInterface::Match(_In_ UNICODE_STRING* Path) {

	m_EResource->LockShared();

	if (m_BlockTrie == nullptr) {
		m_EResource->UnlockShared();

		return false;
	}

	WCharRange range = m_WCharRange;

	MatchContext context = {0};
	bool res =  match(
		m_BlockTrie, 
		m_BlockTrie, 
		Path->Buffer,
		Path->Length / 2,
		&context, 
		&range);

	m_EResource->UnlockShared();

	return res;
}

NTSTATUS AhoCorasickInterface::ReloadPolicy(const WCHAR* RootKey) {
	m_EResource->Lock();

	if (m_BlockTrie != nullptr) {
		delete m_BlockTrie;

	}

	NTSTATUS status = 
		LoadAchoCorasickTrie(RootKey);

	m_EResource->Unlock();

	return status;
}