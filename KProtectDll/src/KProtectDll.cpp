#include <list>

#include "..\include\KProtectDll.h"
#include "..\..\Log\Log.h"
#include "..\..\AhoCorasickWithDump-CPP\include\aho_corasick.h"
#include "..\include\Registry.h"
#include "..\include\FilePaths.h"
#include "..\..\Common\Common.h"

Registry g_Registry;
FilePaths g_FilePaths;

bool InitKProtectInteface() {	
	bool res = true;

	do {
		res =
			g_Registry.Init();
		if (!res) {
			break;
		}
		res =
			g_FilePaths.Init();
	} while (false);

	return res;
}
bool AddRegistryPathToProtect(_In_ const PWCHAR Path) {
	return g_Registry.Protect(Path);
}
bool ApplyRegistryPaths() {
	return g_Registry.Apply();
}
bool TestMatchRegistry(
	_In_ const PWCHAR Path,
	_Out_ bool& Found) {
	return g_Registry.TestAhoCorsickMatch(
		Path, 
		Found
	);
}

EXPORT bool AddFilePathToProtect(_In_ const PWCHAR Path) {
	return g_FilePaths.Protect(Path);
}
EXPORT bool ApplyFilePaths() {
	return g_FilePaths.Apply();
}
EXPORT bool TestMatchFilePath(
	_In_ const PWCHAR Path,
	_Out_ bool& Found) {
	
	return g_FilePaths.TestAhoCorsickMatch(
		Path, 
		Found
	);
}