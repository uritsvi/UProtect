#pragma once

#include "RAIIRegistry.h"
#include "..\AhoCorasickWithDump-CPP\include\aho_corasick.h"

class AhoCorasickInterface
{
public:
	AhoCorasickInterface();
	~AhoCorasickInterface();

	bool Init(_In_ const WCHAR* RootKey);
	bool Match(_In_ const WCHAR* Path);

private:
	NTSTATUS LoadAchoCorasickTrie(_In_ const WCHAR* Path);

private:
	RAIIReigstryKey* m_RootKey;
	FinalTrieEntry* m_BlockTrie;
	WCharRange m_WCharRange;
};

