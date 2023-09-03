#pragma once 

#include <Windows.h>

#include <list>

class RAIIReigstryKey {

public:
	RAIIReigstryKey() = default;
	RAIIReigstryKey(
		_In_ HKEY Root,
		_In_ const WCHAR* RelativePath,
		_In_ REGSAM Access,
		_In_ bool Create);

	~RAIIReigstryKey();

	HKEY GetHANDLE();

	bool FailedToCreate();

	bool WriteValueDWORD(
		_In_ const WCHAR* ValueName,
		_In_ DWORD Value
	);
	bool WriteValueQWORD(
		_In_ const WCHAR* ValueName, 
		_In_ DWORD64 Value
	);
	bool WriteMultiWString(
		_In_ const WCHAR* ValueName, 
		_In_ std::list<std::wstring> String
	);
	bool WriteValue(
		_In_ const WCHAR* ValueName,
		_In_ const void* Buffer,
		_In_ int Size
	);

	bool ReadMultyStringValue(
		_In_ const WCHAR* Name, 
		_Out_ std::list<std::wstring>& Paths
	);
	bool ReadValueDWORD(
		_In_ const WCHAR* Name, 
		_Out_ DWORD& Dword
	);
	bool ReadValueQWORD(
		_In_ const WCHAR* Name,
		_Out_ DWORD64& Dword
	);
	bool ReadValue(
		_In_ const WCHAR* Name, 
		_Out_ char* Buffer,
		_Inout_ DWORD& Size
	);

	bool DeleteAllValue();
	bool DeleteValue(_In_ const WCHAR* Name);

private:
	HKEY m_Key;
	bool m_Failed;
};