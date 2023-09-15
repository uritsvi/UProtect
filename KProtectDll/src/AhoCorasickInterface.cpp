#include <string.h>

#include "..\include\AhoCorasickInterface.h"

#include "..\..\Common\Common.h"

#include <iostream>

#define ALL_PATHS_SUB_KEY L"Build"
#define ALL_PATHS_MULTI_STRING_VALUE_NAME L"Build"

bool AhoCorasick::m_InitAhoCorasick;

AhoCorasick::AhoCorasick() {
	if (!m_InitAhoCorasick) {
		init_aho_corasick(
			malloc,
			free,
			[](void* dest, const void* src, size_t count)
			{ memcpy_s(dest, count, src, count); }
		);

		m_InitAhoCorasick = true;
	}

	m_BuildTrie =
		std::make_shared<BuildTrieEntry>();

	m_fullContext =
		std::make_shared<FullContext>();
	init_full_context(m_fullContext.get());
}
AhoCorasick::~AhoCorasick() {
}

bool AhoCorasick::Init(_In_ const WCHAR* RootPath) {
	
	bool res =CreateOrReadAllSubKeys(RootPath);
	return res;
}

bool AhoCorasick::Save() {
	return Save(m_AllPaths);
}

bool AhoCorasick::Save(_In_ std::list<std::wstring> Paths) {
	bool res = SaveTrie();
	if (!res) {
		return false;
	}
	res = SavePaths(Paths);
	if (!res) {
		return false;
	}
	return true;
}

bool AhoCorasick::SavePaths(_In_ std::list<std::wstring> Paths) {
	bool res = 
		m_AllPathsRegKey->DeleteAllValue();
	if (!res) {
		return false;
	}

	res = m_AllPathsRegKey->WriteMultiWString(
		ALL_PATHS_MULTI_STRING_VALUE_NAME,
		Paths
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

		std::cout << m_AllPaths.size() << "\n";
		for (auto a : m_AllPaths) {
			std::wcout << a << "\n";
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

	BuildTrieEntry entry = {0};

	make_wchar_range(&Range);
	
	int finalNumOfTries = 0;
	for (auto path : Paths) {
		int numOfTries;
		add_leaves(
			path.c_str(), 
			&entry,
			&numOfTries, 
			&Range,
			m_fullContext.get());

		finalNumOfTries += numOfTries;
	}

	make_final_trie(
		&entry,
		finalNumOfTries, 
		&Range, 
		Trie, 
		Size);

	return true;
}

bool AhoCorasick::AddPath(_In_ std::wstring Path) {
	


	if (m_AllPaths.size() >= MAX_PATHS) {
		return false;;
	}

	if (Path.length() >= MAX_PATH_LEN) {
		return false;
	}

	for (auto path : m_AllPaths) {
		if (path == Path) {
			return false;
		}
	}



	m_AllPaths.push_back(Path);
	return true;
}
bool AhoCorasick::RemovePath(_In_ std::wstring Path) {

	for (auto path : m_AllPaths) {

		if (path == Path) {


			m_AllPaths.remove(path);
			return true;
		}
	}

	return false;
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
		return !m_AllPathsRegKey->FailedToCreate();
	}
	return true;
}

bool AhoCorasick::OpenRootKey(_In_ const WCHAR* RootPath) {
	m_RootKey =
		std::make_shared<RAIIReigstryKey>(
			HKEY_LOCAL_MACHINE,
			RootPath,
			KEY_ALL_ACCESS,
			FALSE
		);
	return !m_RootKey->FailedToCreate();
}

bool AhoCorasick::CreateOrReadAllSubKeys(_In_ const WCHAR* RootPath) {

	bool res = true;
	do {
		res =
			OpenRootKey(RootPath);
		if (!res) {
			break;
		}

		res =
			CreateOrOpenBuiledPathsRegKey();
		if (!res) {
			break;
		}
	} while (false);

	return res;
}


bool AhoCorasick::BuiledStringFromList(
	_Out_ WCHAR* String,
	_In_ int Size,
	_In_ std::list<std::wstring> Paths) {
	
	bool res = true;
	int i = 0;
	for (auto string : Paths) {
		if (i + ((string.length() + 1) * sizeof(wchar_t)) >= Size) {
			res = false;
			break;
		}


		memcpy(
			(String + i),
			string.c_str(),
			string.length() * sizeof(wchar_t)
		);

		i += string.length();
		String[i] = ';';
		i += 1;
	}

	String[i] = '\0';
	return res;
}

bool AhoCorasick::ReadBuildPaths(
	_Out_ PWCHAR Path,
	_In_ int Size) {
	
	bool res = true;

	std::list<std::wstring> paths;
	res = m_AllPathsRegKey->ReadMultyStringValue(
		ALL_PATHS_MULTI_STRING_VALUE_NAME,
		paths
	);
	if (!res) {
		return false;
	}
	
	res = BuiledStringFromList(
		Path, 
		Size, 
		paths
	);
	
	return res;
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
		res = true;

		auto finalTrie =
			reinterpret_cast<FinalTrieEntry*>(buffer);

		MatchContext context = { 0};
		int len = wcslen(Path);

		Found = match(
			finalTrie,
			finalTrie,
			Path,
			len,
			&context,
			&wCharRange);
		

	} while (false);

	return res;
}
