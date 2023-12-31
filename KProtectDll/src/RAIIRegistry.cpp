#include <iostream>
#include <sstream>

#include "..\include\RAIIRegistry.h"
#include "..\..\Common\Common.h"

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

		bool res = 
			DeleteValue(valueName);

		if (!res && (status == ERROR_NO_MORE_ITEMS)) {
			return true;
		}
		if (status != ERROR_SUCCESS) {
			return false;
		}
	}
}

RAIIReigstryKey::RAIIReigstryKey(
	_In_ HKEY Root,
	_In_ const WCHAR* RelativePath,
	_In_ REGSAM Access,
	_In_ bool Create) {

	m_Failed = false;
	m_Key = { 0 };

	LSTATUS status;
	
	if (Create) {
		 status = RegCreateKeyW(
			Root, 
			RelativePath, 
			&m_Key);

		if (status != ERROR_SUCCESS) {
			m_Failed = true;
		}

		return;
	}

	status = RegOpenKeyExW(
		Root,
		RelativePath,
		0,
		Access,
		&m_Key);

	if (status != ERROR_SUCCESS) {
		m_Failed = true;
	}
}

RAIIReigstryKey::~RAIIReigstryKey() {
	RegCloseKey(m_Key);
}

bool RAIIReigstryKey::FailedToCreate() {
	return m_Failed;
}

HKEY RAIIReigstryKey::GetHANDLE() {
	return m_Key;
}

bool RAIIReigstryKey::WriteValueDWORD(
	_In_ const WCHAR* ValueName,
	_In_ DWORD Value) {

	auto status =
		RegSetKeyValueW(
			m_Key,
			nullptr,
			ValueName,
			REG_DWORD,
			&Value,
			sizeof(Value));

	if (status == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

bool RAIIReigstryKey::WriteValueQWORD(
	_In_ const WCHAR* ValueName,
	_In_ DWORD64 Value) {

	auto status =
		RegSetKeyValueW(
			m_Key,
			nullptr,
			ValueName,
			REG_QWORD,
			&Value,
			sizeof(Value));

	if (status == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

bool RAIIReigstryKey::WriteMultiWString(
	_In_ const WCHAR* ValueName,
	_In_ std::list<std::wstring> Strings) {

	size_t size = 2;
	for (auto string : Strings) {
		size += ((string.size() + 1) * 2);
	}
	auto builder = std::wstringstream();
	for (auto string : Strings) {
		builder << string << L'\0';
	}
	builder << L'\0';
	
	auto string = builder.str();
	auto status =
		RegSetKeyValueW(
			m_Key,
			nullptr,
			ValueName,
			REG_MULTI_SZ,
			string.c_str(),
			(DWORD)size);
	if (status == ERROR_SUCCESS) {
		return true;
	}

	return false;
}

bool RAIIReigstryKey::WriteValue(
	_In_ const WCHAR* ValueName,
	_In_ const void* Buffer,
	_In_ int Size) {

	auto status =
		RegSetKeyValueW(
			m_Key,
			nullptr,
			ValueName,
			REG_BINARY,
			Buffer,
			Size);

	if (status == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

bool RAIIReigstryKey::ReadMultyStringValue(
	const WCHAR* Name,
	std::list<std::wstring>& Paths) {

	DWORD size = 0;
	auto status = RegGetValueW(
		m_Key, 
		nullptr, 
		Name, 
		RRF_RT_REG_MULTI_SZ, 
		0, 
		nullptr, 
		&size);
	if (status != ERROR_SUCCESS) {
		return false;
	}

	WCHAR* buffer = (WCHAR*)malloc(size);
	status = RegGetValueW(
		m_Key,
		nullptr,
		Name,
		RRF_RT_REG_MULTI_SZ,
		0,
		buffer,
		&size);
	if (status != ERROR_SUCCESS) {
		return false;
	}

	auto builder = std::wstringstream();
	for (DWORD i = 0; i < size / 2; i++) {
		if (buffer[i] != L'\0') {
			builder << buffer[i];
			continue;
		}
		auto current = builder.str();
		if (current == L"") {
			break;
		}


		Paths.push_back(current);
		builder = std::wstringstream();
	}

	free(buffer);
	return true;
}

bool RAIIReigstryKey::ReadValueDWORD(
	_In_ const WCHAR* Name,
	_Out_ DWORD& Dword) {

	bool res = true;
	DWORD dword;

	do {
		DWORD size = sizeof(DWORD);
		auto status =
			RegGetValueW(
				m_Key,
				nullptr,
				Name,
				RRF_RT_REG_DWORD,
				0,
				&dword,
				&size);
		if (status != ERROR_SUCCESS) {
			res = false;
			break;
		}

		Dword = dword;

	} while (false);
	
	return res;

}
bool RAIIReigstryKey::ReadValueQWORD(
	_In_ const WCHAR* Name,
	_Out_ DWORD64& Dword) {

	DWORD64 qword;
	bool res = true;
	do {
		DWORD size = sizeof(DWORD64);
		auto status =
			RegGetValueW(
				m_Key,
				nullptr,
				Name,
				RRF_RT_REG_QWORD,
				0,
				&qword,
				&size);
		if (status != ERROR_SUCCESS) {
			res = false;
			break;
		}

		Dword = qword;
	} while (false);

	return res;
}

bool RAIIReigstryKey::ReadValue(
	_In_ const WCHAR* Name,
	_Out_ char* Buffer,
	_Inout_ DWORD& Size) {

	bool res = true;
	do {
		auto status =
			RegGetValueW(
				m_Key,
				nullptr,
				Name,
				RRF_RT_REG_BINARY,
				0,
				Buffer,
				&Size);
		if (status != ERROR_SUCCESS) {
			res = false;
			break;
		}
	} while (false);

	return res;
}
