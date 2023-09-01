#include <list>

#include "..\include\KProtectDll.h"
#include "..\..\Log\Log.h"
#include "..\..\AhoCorasickWithDump-CPP\include\aho_corasick.h"
#include "..\include\Registry.h"
#include "..\..\Common\Common.h"

bool InitKProtectInteface() {
	return true;
}
bool AddRegistryPathToProtect(const PWCHAR Path) {
	return true;
}
bool ApplayRegistryPaths() {
	return true;
}
bool SaveBuildPaths() {
	return true;
}
bool LoadRegBuildPaths() {
	return true;
}