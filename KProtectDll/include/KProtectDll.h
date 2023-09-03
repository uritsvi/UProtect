#pragma once 

#define EXPORT __declspec(dllexport)

#include <Windows.h>

EXPORT bool InitKProtectInteface();
EXPORT bool AddRegistryPathToProtect(_In_ const PWCHAR Path);
EXPORT bool ApplayRegistryPaths();
EXPORT bool TestMatchRegistry(
	_In_ const PWCHAR Path, 
	_Out_ bool& Found
);
