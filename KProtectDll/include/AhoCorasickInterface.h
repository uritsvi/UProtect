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

	bool Init(
		_In_ const WCHAR* RootPath, 
		_In_ bool LoadBuildPaths
	);

	bool Save();

	bool MakeTrie(
		_In_ std::list<std::wstring> Paths, 
		_Out_ FinalTrieEntry** Trie, 
		_Out_ int* Size,
		_Out_ WCharRange& range);
	
	bool AddPath(_In_ std::wstring Path);

	bool TestAhoCorsickMatch(
		_In_ const WCHAR* Path,
		_Out_ bool& Found
	);

private:
	bool SaveTrie();
	bool SaveRange(
		_In_ std::shared_ptr<RAIIReigstryKey> Key, 
		_In_ WCharRange& Range
	);
	bool SaveSize(
		_In_ std::shared_ptr<RAIIReigstryKey> Key,
		_In_ int Size
	);
	
	bool SavePaths();

	void ReadBuildPaths();
	bool CreateOrReadAllSubKeys(
		_In_ const WCHAR* RootPath,
		_In_ bool LoadBuiledPaths
	);

	bool CreateOrOpenBuiledPathsRegKey();
	bool OpenRootKey(_In_ const WCHAR* RootPath);

private:
	std::shared_ptr<RAIIReigstryKey> m_AllPathsRegKey;
	std::shared_ptr<RAIIReigstryKey> m_RootKey;

	std::shared_ptr<BuildTrieEntry> m_BuildTrie;

	std::list<std::wstring> m_AllPaths;

	static bool m_InitAhoCorasick;
};

