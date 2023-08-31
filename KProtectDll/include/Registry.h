#pragma once 

#include <Windows.h>

#include <memory>
#include <sal.h>
#include <list>

#include "..\..\AhoCorasickWithDump-CPP\include\aho_corasick.h"
#include "..\..\Common\Common.h"

class Registry {
	
public:
	static bool SaveTrieToRegitry(
		_In_ const wchar_t* Path,
		_In_ FinalTrieEntry* Trie,
		_In_ int Size,
		_In_ WCharRange* Range);

	static bool SaveBuildPaths(
		_In_ std::list<std::wstring>& const Paths
	);
	static bool LoadBuildPaths(
		_In_ std::list<std::wstring>& Paths
	);
};

