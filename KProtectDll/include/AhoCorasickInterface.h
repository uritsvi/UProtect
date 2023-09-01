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
	AhoCorasick(_In_ std::shared_ptr<RAIIReigstryKey> RootKey);
	~AhoCorasick();

	bool Save();

	bool MakeTrie(
		_In_ std::list<std::wstring> Paths, 
		_Out_ FinalTrieEntry** Trie, 
		_Out_ int* Size,
		_Out_ WCharRange& range);

private:
	bool SaveTrie();
	bool SavePaths();

	void ReadBuildPaths(_In_ std::shared_ptr<RAIIReigstryKey> RootKey);
	void CreateOrReadAllSubKeys(_In_ std::shared_ptr<RAIIReigstryKey> RootKey);

private:
	std::shared_ptr<RAIIReigstryKey> m_AllPathsRegKey;
	std::shared_ptr<RAIIReigstryKey> m_TrieRegKey;
	std::shared_ptr<RAIIReigstryKey> m_RootKey;

	std::shared_ptr<BuildTrieEntry> m_BuildTrie;

	std::list<std::wstring> m_AllPaths;

	static bool m_InitAhoCorasick;
};

