#pragma once


#include "Driver.h"

class RAIIReigstryKey {

public:
	RAIIReigstryKey(
		_In_ HANDLE Root, 
		_In_ const WCHAR* Path,
		_In_ ACCESS_MASK Access);


	~RAIIReigstryKey();

	HANDLE GetHANDLE();

	bool FailedToCreate();

	bool ReadValueDWORD(
		_In_ const WCHAR* ValueName,
		_Out_ int* Value);
	bool ReadValueQWORD(
		_In_ const WCHAR* ValueName,
		_Out_ LONG_PTR* Value);


	bool ReadValue(
		_In_ const WCHAR* ValueName,
		_Out_ KEY_VALUE_PARTIAL_INFORMATION* Buffer,
		_In_ int Size);


private:
	HANDLE m_Key;
	bool m_Failed;
};