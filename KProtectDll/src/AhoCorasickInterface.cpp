#include "..\include\AhoCorasickInterface.h"

#include "..\..\Common\Common.h"

#define ALL_PATHS_SUB_KEY L"Build"
#define ALL_PATHS_MULTI_STRING_VALUE_NAME L"Build"

bool AhoCorasick::m_InitAhoCorasick;

AhoCorasick::AhoCorasick(
	_In_ std::shared_ptr<RAIIReigstryKey> RootKey) {

	if (!m_InitAhoCorasick) {
		init_aho_corasick(
			malloc,
			free,
			[](void* dest, const void* src, size_t count)
			{ memcpy_s(dest, count, src, count); }
		);

		m_InitAhoCorasick = true;
	}

	m_RootKey = RootKey;

	m_BuildTrie =
		std::make_shared<BuildTrieEntry>();
}
AhoCorasick::~AhoCorasick() {
}

bool AhoCorasick::Init(_In_ bool LoadBuildPaths) {
	return CreateOrReadAllSubKeys(
		LoadBuildPaths
	);
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
		FALSE);

	if (key.FailedToCreate()) {
		
		key = RAIIReigstryKey(
			HKEY_LOCAL_MACHINE,
			REG_BUILD_PATHS,
			KEY_ALL_ACCESS,
			TRUE);
		if (key.FailedToCreate()) {
			return false;
		}
	}

	bool res = key.DeleteAllValue();
	if (!res) {
		return false;
	}

	res = key.WriteMultiWString(
		ALL_PATHS_MULTI_STRING_VALUE_NAME,
		m_AllPaths
	);
	if (!res) {
		return false;
	}

	return true;
}

bool AhoCorasick::SaveRange(
	_In_ std::shared_ptr<RAIIReigstryKey> Key,
	_In_ WCharRange& Range
) {
	DWORD64 range = Range.Max;
	range = (range << 32);
	range += Range.Min;

	BOOL res =
		m_RootKey->WriteValueQWORD(
			REG_TRIE_WCHAR_RANGE_VALUE_NAME,
			range);

	return res;
}
bool AhoCorasick::SaveSize(
	_In_ std::shared_ptr<RAIIReigstryKey> Key,
	_In_ int Size
) {

	bool res = m_RootKey->WriteValueDWORD(
		REG_TRIE_SIZE_VALUE_NAME,
		Size);
	
	return res;
}

bool AhoCorasick::SaveTrie() {
	FinalTrieEntry* finalTrie;
	WCharRange wcRange;

	int size;
	bool res = true;

	do {
		bool res = 
			m_RootKey->DeleteAllValue();
		if (!res) {
			break;
		}

		res = MakeTrie(
			m_AllPaths, 
			&finalTrie, 
			&size,
			wcRange);
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

		res = SaveRange(m_RootKey, wcRange);
		if (!res) {
			break;
		}
		res = SaveSize(m_RootKey, size);
		if (!res) {
			break;
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

	make_wchar_range(&Range);
	
	int finalNumOfTries = 0;
	for (auto path : Paths) {
		int numOfTries;
		add_leaves(
			path.c_str(), 
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

bool AhoCorasick::AddPath(_In_ std::wstring Path) {
	m_AllPaths.push_back(Path);
	return true;
}

bool AhoCorasick::CreateOrOpenBuiledPathsRegKey() {
	m_AllPathsRegKey =
		std::make_shared<RAIIReigstryKey>(
			m_RootKey->GetHANDLE(),
			ALL_PATHS_SUB_KEY,
			KEY_ALL_ACCESS,
			FALSE);

	if (m_AllPathsRegKey->FailedToCreate()) {
		m_AllPathsRegKey =
			std::make_shared<RAIIReigstryKey>(
				m_RootKey->GetHANDLE(),
				ALL_PATHS_SUB_KEY,
				KEY_ALL_ACCESS,
				TRUE);
		return false;
	}
	return true;
}

bool AhoCorasick::CreateOrReadAllSubKeys(
	_In_ bool LoadBuiledPaths) {

	bool res = true;
	do {
		res =
			CreateOrOpenBuiledPathsRegKey();
		if (!res) {
			break;
		}
		if (!LoadBuiledPaths) {
			m_AllPathsRegKey->DeleteAllValue();
			break;
		}
		ReadBuildPaths();

	} while (false);

	return res;
}

void AhoCorasick::ReadBuildPaths() {

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

bool AhoCorasick::TestAhoCorsickMatch(
	_In_ const WCHAR* Path,
	_Out_ bool& Found) {

	Found = false;

	WCharRange wCharRange;
	bool res = true;

	do {

		DWORD size;
		DWORD64 range;

		res = m_RootKey->ReadValueDWORD(
			REG_TRIE_SIZE_VALUE_NAME,
			size
		);
		if (!res) {
			break;
		}
		res = m_RootKey->ReadValueQWORD(
			REG_TRIE_WCHAR_RANGE_VALUE_NAME,
			range
		);
		if (!res) {
			break;
		}

		wCharRange.Min = (int)range;
		wCharRange.Max = (range >> 32);

		char* buffer = (char*)malloc(size);
		int i = 0;
		while (true) {
			char* current = buffer + (i * DEFAULT_BUFFER_SIZE);

			DWORD size = DEFAULT_BUFFER_SIZE;
			res = m_RootKey->ReadValue(
				std::to_wstring(i).c_str(),
				current,
				size
			);
			if (!res) {
				break;
			}

			i++;
		}

		auto finalTrie =
			reinterpret_cast<FinalTrieEntry*>(buffer);

		MatchContext context = { 0};
		Found = match(
			finalTrie,
			finalTrie,
			Path,
			&context,
			&wCharRange);


	} while (false);

	return res;
}
