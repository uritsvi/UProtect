#pragma once

#include <fltKernel.h>



#include "Driver.h"



#include "..\AhoCorasickWithDump-CPP\include\aho_corasick.h"

class MiniFilter
{
public:
	~MiniFilter();

	NTSTATUS Init(
		_In_ DRIVER_OBJECT* Driver, 
		_In_ UNICODE_STRING* Registry);
	NTSTATUS StartProtect();

private:
	NTSTATUS LoadAchoCorasickTrie();

	NTSTATUS MiniFilter::InitMiniFilter(
		DRIVER_OBJECT* Driver
	);

	void tempInitRegistry(UNICODE_STRING* RegistryPath);




public:
	PFLT_FILTER m_Filter;

private:
	FinalTrieEntry* m_BlockTrie;
};

