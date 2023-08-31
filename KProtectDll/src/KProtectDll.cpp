#include <list>

#include "..\include\KProtectDll.h"
#include "..\..\Log\Log.h"
#include "..\..\AhoCorasickWithDump-CPP\include\aho_corasick.h"
#include "..\include\Registry.h"
#include "..\..\Common\Common.h"

#define DRIVER_PATH L"\\\\.\\KProtect"

static HANDLE g_DriverHandle = INVALID_HANDLE_VALUE;

static BuildTrieEntry g_BuildTrieEntry = {0};
static int g_NumOfNodes = 0;

static WCharRange g_WCharRange = { 0 };

static std::list<std::wstring> g_BuildRegistryPaths;

/*
#ifdef _DEBUG
	#define CheckDriverHandle() \
		if (g_DriverHandle == INVALID_HANDLE_VALUE) \
		{return false;}return true;
#else
	#define CheckDriverHandle()
#endif
*/


bool OpenDriverHandle() {
	g_DriverHandle = CreateFile(
		DRIVER_PATH, 
		GENERIC_WRITE, 
		0, 
		nullptr, 
		OPEN_EXISTING, 
		0, 
		nullptr);

	if (g_DriverHandle == INVALID_HANDLE_VALUE) {
		return false;
	}

	return true;
}

bool InitKProtectInteface() {
	init_aho_corasick(
		malloc,
		free,
		[](void* dest, const void* src, size_t count)
			{ memcpy_s(dest, count, src, count); }
	);

	make_wchar_range(&g_WCharRange);

	bool res = OpenDriverHandle();
	if (!res) {
		return res;
	}

	return true;
}

bool AddRegistryPathToProtect(const PWCHAR Path) {
	int numOfNodes = 0;
	add_leaves(
		Path,
		1,
		&g_BuildTrieEntry,
		&numOfNodes,
		&g_WCharRange);

	g_NumOfNodes += numOfNodes;

	g_BuildRegistryPaths.push_back(Path);

	return true;

}
bool ApplayRegistryPaths() {

	if (g_NumOfNodes == 0) {
		return true;
	}

	FinalTrieEntry* buffer = 0;
	int bufferSize;

	
	make_final_trie(
		&g_BuildTrieEntry, 
		g_NumOfNodes, 
		&g_WCharRange,
		&buffer, 
		&bufferSize);

	bool res = Registry::SaveTrieToRegitry(
		REG_INFO_ROOT_PATH,
		buffer,
		bufferSize,
		&g_WCharRange);

	if (!res) {
		return false;
	}

	free(buffer);


	return true;

}

bool SaveBuildPaths() {
	return Registry::SaveBuildPaths(g_BuildRegistryPaths);
}
bool LoadRegBuildPaths() {
	std::list<std::wstring> paths;
	bool res = 
		Registry::LoadBuildPaths(paths);
	if (!res) {
		return false;
	}

	for (auto path : paths) {
		AddRegistryPathToProtect((PWCHAR)path.c_str());
	}

	return true;
}