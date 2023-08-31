#pragma once 

#define EXPORT __declspec(dllexport)

#include <Windows.h>


EXPORT bool InitKProtectInteface();
EXPORT bool AddRegistryPathToProtect(const PWCHAR Path);
EXPORT bool ApplayRegistryPaths();
EXPORT bool SaveBuildPaths();
EXPORT bool LoadRegBuildPaths();