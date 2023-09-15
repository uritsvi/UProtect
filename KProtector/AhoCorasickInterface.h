#pragma once

#include "RAIIRegistry.h"
#include "..\AhoCorasickWithDump-CPP\include\aho_corasick.h"
#include "..\KTL\include\EResource.h"

class AhoCorasickInterface
{
public:
	AhoCorasickInterface();
	~AhoCorasickInterface();

	bool Init(_In_ const WCHAR* RootKey);
	bool Match(_In_ UNICODE_STRING* Path);

	NTSTATUS ReloadPolicy(const WCHAR* RootKey);

private:
	NTSTATUS LoadAchoCorasickTrie(_In_ const WCHAR* Path);

private:
	FinalTrieEntry* m_BlockTrie;
	WCharRange m_WCharRange;

	ExecutiveResource* m_EResource;
};

