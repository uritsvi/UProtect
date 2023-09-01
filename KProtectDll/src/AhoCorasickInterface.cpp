#include "..\include\AhoCorasickInterface.h"

#include "..\..\Common\Common.h"

#define ALL_PATHS_SUB_KEY L"Build"
#define ALL_PATHS_MULTI_STRING_VALUE_NAME L"Build"

bool AhoCorasick::m_InitAhoCorasick;

AhoCorasick::AhoCorasick(_In_ std::shared_ptr<RAIIReigstryKey> RootKey) {
	m_RootKey = RootKey;

	if (m_InitAhoCorasick) {
		init_aho_corasick(
			malloc,
			free,
			[](void* dest, const void* src, size_t count)
			{ memcpy_s(dest, count, src, count); }
		);
		
		m_InitAhoCorasick = true;
	}

	CreateOrReadAllSubKeys(RootKey);

}
AhoCorasick::~AhoCorasick() {
}

bool AhoCorasick::Save() {
	bool res = SaveTrie();
	if (!res) {
		return false;
	}
	res = SavePaths();
	if (!res) {
		return false;
	}
	return true;
}

bool AhoCorasick::SavePaths() {
	auto key = RAIIReigstryKey(
		HKEY_LOCAL_MACHINE,
		REG_BUILD_PATHS,
		KEY_ALL_ACCESS,
		true);

	if (key.FailedToCreate()) {
		return false;
	}


	bool res = key.WriteMultiWString(
		ALL_PATHS_MULTI_STRING_VALUE_NAME,
		m_AllPaths
	);
	if (!res) {
		return false;
	}


	return true;
}

bool AhoCorasick::SaveTrie() {
	FinalTrieEntry* finalTrie;
	WCharRange wcRange;

	int size;
	bool res = true;

	do {

		bool res = MakeTrie(
			m_AllPaths, 
			&finalTrie, 
			&size,
			wcRange);
		if (!res) {
			res = false;
			break;
		}

		m_RootKey->DeleteAllValue();

		res = m_RootKey->WriteValueDWORD(
			REG_TRIE_SIZE_VALUE_NAME,
			size);
		if (!res) {
			res = false;
			break;
		}

		DWORD64 range = wcRange.Max;
		range = (range << 32);
		range += wcRange.Min;

		res =
			m_RootKey->WriteValueQWORD(
				REG_TRIE_WCHAR_RANGE_VALUE_NAME,
				range);

		if (!res) {
			res = false;
			break;
		}


		int c = 0;
		for (int i = 0; i < size; i += DEFAULT_BUFFER_SIZE) {
			int writeSize = DEFAULT_BUFFER_SIZE;
			if ((size - i) < writeSize) {
				writeSize = (size - i);
			}

			m_RootKey->WriteValue(
				std::to_wstring(c++).c_str(),
				((char*)finalTrie + i),
				writeSize);

		}

	} while (false);

	return res;

}
bool AhoCorasick::MakeTrie(
	_In_ std::list<std::wstring> Paths,
	_Out_ FinalTrieEntry** Trie,
	_Out_ int* Size,
	_Out_ WCharRange& Range
) {
	if (Trie == nullptr || Size == nullptr) {
		return false;
	}

	m_BuildTrie = { 0 };

	make_wchar_range(&Range);
	
	int finalNumOfTries = 0;
	for (auto path : Paths) {
		int numOfTries;
		add_leaves(
			path.c_str(), 
			1, 
			m_BuildTrie.get(),
			&numOfTries, 
			&Range);

		finalNumOfTries += numOfTries;
	}

	make_final_trie(
		m_BuildTrie.get(),
		finalNumOfTries, 
		&Range, 
		Trie, 
		Size);

	return true;
}

void AhoCorasick::CreateOrReadAllSubKeys(_In_ std::shared_ptr<RAIIReigstryKey> RootKey) {
	ReadBuildPaths(RootKey);
}

void AhoCorasick::ReadBuildPaths(_In_ std::shared_ptr<RAIIReigstryKey> RootKey) {
	m_AllPathsRegKey =
		std::make_shared<RAIIReigstryKey>(
			RootKey->GetHANDLE(),
			ALL_PATHS_SUB_KEY,
			KEY_ALL_ACCESS,
			FALSE);

	if (m_AllPathsRegKey->FailedToCreate()) {
		m_AllPathsRegKey =
			std::make_shared<RAIIReigstryKey>(
				RootKey->GetHANDLE(),
				ALL_PATHS_SUB_KEY,
				KEY_ALL_ACCESS,
				TRUE);
		return;
	}

	std::list<std::wstring> paths;
	bool res = m_AllPathsRegKey->ReadMultyStringValue(
		ALL_PATHS_MULTI_STRING_VALUE_NAME,
		paths
	);

	if (!res) {
		return;
	}

	for (auto string : paths) {
		m_AllPaths.push_back(string);
	}
}