#pragma once 

#include <sal.h>

#include "..\..\AhoCorasickWithDump\include\aho_corasick.h"


void SaveToRegistry(
	_In_ wchar_t* RegistryPath,
	_In_ FinalTrieEntry* Trie);

void ExtructFromRegistry(
	_In_ wchar_t* RegistryPath, 
	_Out_ FinalTrieEntry** Trie);
