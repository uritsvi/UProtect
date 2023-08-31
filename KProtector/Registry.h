#pragma once



#include "Driver.h"
#include "..\AhoCorasickWithDump-CPP\include\aho_corasick.h"



class RegistryBlocker
{
public:
	static NTSTATUS CreateRegistryBlocker
		(_Out_ RegistryBlocker** RegistryBlocker);
	
	RegistryBlocker();
	~RegistryBlocker();

	void StartProtect();

	NTSTATUS SelfRegistryCallback(
		_In_opt_ PVOID Argument1,
		_In_opt_ PVOID Argument2);

private:
	NTSTATUS LoadAchoCorasickTrie();

private:
	LARGE_INTEGER m_Cookie;

	FinalTrieEntry* m_BlockTrie;
	WCharRange m_WCharRange;
};

