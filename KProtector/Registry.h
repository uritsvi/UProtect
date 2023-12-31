#pragma once

#include "Driver.h"
#include "AhoCorasickInterface.h"

class RegistryBlocker
{
public:
	static NTSTATUS CreateRegistryBlocker
		(_Out_ RegistryBlocker** RegistryBlocker);
	

	RegistryBlocker();
	~RegistryBlocker();

	void StartProtect();

	NTSTATUS AllowToModify(_In_ PVOID Object);

	NTSTATUS SelfRegistryCallback(
		_In_opt_ PVOID Argument1,
		_In_opt_ PVOID Argument2);

private:
	LARGE_INTEGER m_Cookie;
	
};

