#pragma once 

#define EXPORT __declspec(dllexport)

#include <Windows.h>

EXPORT bool InitKProtectInteface();

EXPORT bool AddRegistryPathToProtect(_In_ const PWCHAR Path);
EXPORT bool ApplyRegistryPaths();
EXPORT bool TestMatchRegistry(
	_In_ const PWCHAR Path, 
	_Out_ bool& Found
);
EXPORT bool AddFilePathToProtect(_In_ const PWCHAR Path);
EXPORT bool ApplyFilePaths();
EXPORT bool TestMatchFilePath(
	_In_ const PWCHAR Path,
	_Out_ bool& Found
);