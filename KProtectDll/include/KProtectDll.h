#pragma once 

#define EXPORT __declspec(dllexport)

#include <Windows.h>


extern "C" EXPORT bool InitKProtectInteface();

extern "C" EXPORT bool AddRegistryPathToProtect(_In_ const PWCHAR Path);
extern "C" EXPORT bool ApplyRegistryPaths();
extern "C" EXPORT bool TestMatchRegistry(
	_In_ const PWCHAR Path,
	_Out_ bool& Found
);
extern "C" EXPORT bool AddFilePathToProtect(_In_ const PWCHAR Path);

extern "C" EXPORT bool ApplyFilePaths();
extern "C" EXPORT bool TestMatchFilePath(
	_In_ const PWCHAR Path,
	_Out_ bool& Found
);

extern "C" EXPORT bool QueryReg(
	_Out_ PWCHAR Paths,
	_In_ int Size
);

extern "C" EXPORT bool QueryMiniFilter(
	_Out_ PWCHAR Paths,
	_In_ int Size
);

extern "C" EXPORT bool RemoveRegPath(_In_ const PWCHAR Path);
extern "C" EXPORT bool RemoveMiniFilterPath(_In_ const PWCHAR Path);

extern "C" EXPORT bool StartDriver();
extern "C" EXPORT bool StopDriver();

extern "C" EXPORT bool IsDriverLoaded();