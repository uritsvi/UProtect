#pragma once

#include "RAIIRegistry.h"
#include "..\AhoCorasickWithDump-CPP\include\aho_corasick.h"

class AhoCorasickInterface
{
public:
	AhoCorasickInterface();
	~AhoCorasickInterface();

	bool Init(_In_ const WCHAR* RootKey);
	bool Match(_In_ UNICODE_STRING* Path);

private:
	NTSTATUS LoadAchoCorasickTrie(_In_ const WCHAR* Path);

private:
	FinalTrieEntry* m_BlockTrie;
	WCharRange m_WCharRange;
};

