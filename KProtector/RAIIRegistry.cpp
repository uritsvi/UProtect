#include "RAIIRegistry.h"

RAIIReigstryKey::RAIIReigstryKey(
	_In_ HANDLE Root,
	_In_ const WCHAR* Path,
	_In_ ACCESS_MASK Access) {

	m_Failed = false;
	m_Key = { 0 };

	UNICODE_STRING UnicodePath;
	RtlInitUnicodeString(
		&UnicodePath, 
		Path);
	
	OBJECT_ATTRIBUTES att = { 0 };
	InitializeObjectAttributes(
		&att, 
		&UnicodePath,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
		Root,
		NULL);

	NTSTATUS status = ZwOpenKey(
		&m_Key, 
		Access, 
		&att);


	if (!NT_SUCCESS(status)) {
		KdPrint(("Failed to open a reg key"));

		m_Failed = true;
		return;
	}
	KdPrint(("Open a reg key succesfuly"));
}

RAIIReigstryKey::~RAIIReigstryKey() {
	ZwClose(m_Key);
}

HANDLE RAIIReigstryKey::GetHANDLE() {
	return m_Key;
}

bool RAIIReigstryKey::FailedToCreate() {
	return m_Failed;
}

bool RAIIReigstryKey::ReadValueQWORD(
	_In_ const WCHAR* ValueName,
	_Out_ LONG_PTR* Value) {

	*Value = { 0 };

	char Buffer[sizeof(KEY_VALUE_PARTIAL_INFORMATION) + sizeof(LONG_PTR)];

	UNICODE_STRING Name;
	RtlInitUnicodeString(
		&Name,
		ValueName);

	ULONG OutSize;
	NTSTATUS status = ZwQueryValueKey(
		m_Key,
		&Name,
		KeyValuePartialInformation,
		Buffer,
		sizeof(Buffer),
		&OutSize);
	if (!NT_SUCCESS(status)) {
		return false;
	}

	auto Info =
		reinterpret_cast<KEY_VALUE_PARTIAL_INFORMATION*>(Buffer);


	if (Info->DataLength > sizeof(ULONG_PTR)) {
		KdPrint(("Reg value is > sizeof(DWORD)"));
		return false;
	}

	memcpy(
		Value,
		Info->Data,
		sizeof(ULONG_PTR));

	return true;
}
bool RAIIReigstryKey::ReadValueDWORD(
	_In_ const WCHAR* ValueName,
	_Out_ int* Value) {

	*Value = { 0 };

	char Buffer[sizeof(KEY_VALUE_PARTIAL_INFORMATION) + sizeof(int)];

	UNICODE_STRING Name;
	RtlInitUnicodeString(
		&Name, 
		ValueName);

	ULONG OutSize;
	NTSTATUS status = ZwQueryValueKey(
		m_Key, 
		&Name, 
		KeyValuePartialInformation, 
		Buffer, 
		sizeof(Buffer), 
		&OutSize);
	if (!NT_SUCCESS(status)) {
		return false;
	}

	auto Info =
		reinterpret_cast<KEY_VALUE_PARTIAL_INFORMATION*>(Buffer);


	if (Info->DataLength > sizeof(int)) {
		KdPrint(("Reg value is > sizeof(DWORD)"));
		return false;
	}

	memcpy(
		Value, 
		Info->Data, 
		sizeof(int));

	return true;
}

bool RAIIReigstryKey::ReadValue(
	_In_ const WCHAR* ValueName,
	_Out_ KEY_VALUE_PARTIAL_INFORMATION* Buffer,
	_In_ int Size) {

	UNICODE_STRING Name;
	RtlInitUnicodeString(&Name, ValueName);

	if (Buffer == nullptr) {
		KdPrint(("Try to read reg value into a nullptr"));
		return false;
	}

	ULONG OutSize;
	NTSTATUS status = ZwQueryValueKey(
		m_Key,
		&Name,
		KeyValuePartialInformation,
		Buffer,
		Size,
		&OutSize);

	if (!NT_SUCCESS(status)) {
		KdPrint(("Failed to read value from a reg key"));
		return false;
	}

	return true;
}