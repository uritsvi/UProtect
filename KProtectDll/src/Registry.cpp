#include <string>

#include "..\include\Registry.h"
#include "..\..\Common\Common.h"

#include "..\include\RAIIRegistry.h"

#define BUILD_PATHS_VALUE_NAME L"Build"

bool RAIIReigstryKey::DeleteValue(_In_ const WCHAR* Name) {
	auto status = RegDeleteValue(
		m_Key, 
		Name);
	if (status != ERROR_SUCCESS) {
		return false;
	}
	return true;
}

bool RAIIReigstryKey::DeleteAllValue() {
	int i = 0;

	
	LSTATUS status;
	while (true) {		
		WCHAR valueName[DEFAULT_BUFFER_SIZE] = { 0 };
		int valueNameLen = sizeof(valueName);

		status = RegEnumValueW(
			m_Key,
			0,
			valueName,
			(DWORD*)&valueNameLen,
			nullptr,
			nullptr,
			nullptr,
			nullptr);

			DeleteValue(valueName);

			if (status != ERROR_SUCCESS) {
				return false;
			}

	}
}

bool Registry::SaveTrieToRegitry(
	_In_ const wchar_t* Path,
	_In_ FinalTrieEntry* Trie,
	_In_ int Size,
	_In_ WCharRange* Range) {

	

	bool res = true;

	do {
		auto key = RAIIReigstryKey(
			HKEY_LOCAL_MACHINE,
			Path,
			KEY_ALL_ACCESS,
			false);

		if (key.FailedToCreate()) {
			res = false;
		}

		key.DeleteAllValue();

		res = key.WriteValueDWORD(
			REG_TRIE_SIZE_VALUE_NAME, 
			Size);
		if (!res) {
			return false;
		}

		DWORD64 range = Range->Max;
		range =  (range << 32);
		range += Range->Min;

		res = 
			key.WriteValueQWORD(
				REG_TRIE_WCHAR_RANGE_VALUE_NAME, 
				range);

		if (!res) {
			return false;
		}
		
		
		int c = 0;
		for (int i = 0; i < Size; i += DEFAULT_BUFFER_SIZE) {
			int writeSize = DEFAULT_BUFFER_SIZE;
			if ((Size - i) < writeSize) {
				writeSize = (Size - i);
			}

			key.WriteValue(
				std::to_wstring(c++).c_str(), 
				((char*)Trie + i), 
				writeSize);

		}
	
	} while (false);

	return res;

}

bool Registry::SaveBuildPaths(
	_In_ std::list<std::wstring>& const Paths
) {
	auto key = RAIIReigstryKey(
		HKEY_LOCAL_MACHINE,
		REG_BUILD_PATHS, 
		KEY_ALL_ACCESS, 
		true);

	if (key.FailedToCreate()) {
		return false;
	}


	bool res = key.WriteMultiWString(
		BUILD_PATHS_VALUE_NAME,
		Paths
	);
	if (!res) {
		return false;
	}


	return true;
}

bool Registry::LoadBuildPaths(
	_In_ std::list<std::wstring>& Paths
) {
	auto key = RAIIReigstryKey(
		HKEY_LOCAL_MACHINE,
		REG_BUILD_PATHS,
		KEY_ALL_ACCESS,
		true);

	bool res = 
		key.ReadMultyStringValue(
			BUILD_PATHS_VALUE_NAME, 
			Paths);
	if (!res) {
		return false;
	}

	return true;

}