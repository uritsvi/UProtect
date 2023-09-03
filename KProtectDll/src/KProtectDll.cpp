#include <list>

#include "..\include\KProtectDll.h"
#include "..\..\Log\Log.h"
#include "..\..\AhoCorasickWithDump-CPP\include\aho_corasick.h"
#include "..\include\Registry.h"
#include "..\..\Common\Common.h"

std::shared_ptr<Registry> g_Registry;

bool InitKProtectInteface() {	
	bool res = true;

	do {
		g_Registry =
			std::make_shared<Registry>();

		bool res =
			g_Registry->Init();
		if (!res) {
			break;
		}
	} while (false);

	return res;
}
bool AddRegistryPathToProtect(_In_ const PWCHAR Path) {
	return g_Registry->Protect(Path);
}
bool ApplayRegistryPaths() {
	return g_Registry->Apply();
}
bool TestMatchRegistry(
	_In_ const PWCHAR Path,
	_Out_ bool& Found) {
	return g_Registry->TestAhoCorsickMatch(
		Path, 
		Found
	);
}