#pragma once

#include <Windows.h>

#include <list>
#include <string>
#include <memory>

#include "..\..\AhoCorasickWithDump-CPP\include\aho_corasick.h"
#include "..\include\RAIIRegistry.h"

class AhoCorasick
{
public:
	AhoCorasick();
	~AhoCorasick();

	bool Init(_In_ const WCHAR* RootPath);

	bool Save();
	bool Save(_In_ std::list<std::wstring> Paths);

	bool MakeTrie(
		_In_ std::list<std::wstring> Paths, 
		_Out_ FinalTrieEntry** Trie, 
		_Out_ int* Size,
		_Out_ WCharRange& range);
	
	bool AddPath(_In_ std::wstring Path);
	bool RemovePath(_In_ std::wstring Path);

	bool TestAhoCorsickMatch(
		_In_ const WCHAR* Path,
		_Out_ bool& Found
	);
	
	bool ReadBuildPaths(
		_Out_ PWCHAR Path,
		_In_ int Size
	);

private:
	bool BuiledStringFromList(
		_Out_ WCHAR* String, 
		_In_ int Len, 
		_In_ std::list<std::wstring> Paths
	);

	bool SaveTrie();
	bool SaveRange(
		_In_ std::shared_ptr<RAIIReigstryKey> Key, 
		_In_ WCharRange& Range
	);
	bool SaveSize(
		_In_ std::shared_ptr<RAIIReigstryKey> Key,
		_In_ int Size
	);
	
	bool SavePaths(_In_ std::list<std::wstring> Paths);

	bool CreateOrReadAllSubKeys(_In_ const WCHAR* RootPath);

	bool CreateOrOpenBuiledPathsRegKey();
	bool OpenRootKey(_In_ const WCHAR* RootPath);

private:
	std::shared_ptr<RAIIReigstryKey> m_AllPathsRegKey;
	std::shared_ptr<RAIIReigstryKey> m_RootKey;

	std::shared_ptr<BuildTrieEntry> m_BuildTrie;

	std::shared_ptr<FullContext> m_fullContext;

	std::list<std::wstring> m_AllPaths;

	static bool m_InitAhoCorasick;
};

