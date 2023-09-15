#include <list>

#include "..\include\KProtectDll.h"
#include "..\..\Log\Log.h"
#include "..\..\AhoCorasickWithDump-CPP\include\aho_corasick.h"
#include "..\..\Common\Common.h"
#include "..\include\AhoCorasickInterface.h"

#pragma warning(disable:4996)

AhoCorasick g_Registry;
AhoCorasick g_FilePaths;

std::list<std::wstring> m_AllFilePaths;

bool ConvertDosPathToNTFSPath(
	_In_ const WCHAR* DosPath,
	_Out_ WCHAR* NTFSBuffer,
	_In_ int BufferSize
);

EXPORT bool InitKProtectInteface() {
	bool res = true;


	do {
		res =
			g_Registry.Init(REG_INFO_ROOT_PATH);
		if (!res) {
			break;
		}
		res =
			g_FilePaths.Init(MINIFILTER_INFO_ROOT_PATH);

		HMODULE ntdllHandle = GetModuleHandleA("ntdll.dll");
		if (ntdllHandle == nullptr) {
			res = false;
			break;
		}



	} while (false);


	return res;
}

bool ConvertDosPathToNTFSPath(
	_In_ const WCHAR* DosPath,
	_Out_ WCHAR* NTFSBuffer,
	_In_ int BufferSize)
{

	if (BufferSize <= wcslen(DosPath)) {
		return false;
	}

	wchar_t path[DEFAULT_BUFFER_SIZE];
	wcscpy_s(path, DosPath);

	wchar_t* context = { 0 };
	wchar_t* driver = wcstok_s(
		path,
		L"\\",
		&context
	);

	QueryDosDeviceW(
		driver,
		NTFSBuffer,
		DEFAULT_BUFFER_SIZE);

	if (BufferSize <= wcslen(DosPath) + wcslen(NTFSBuffer)) {
		return false;
	}


	wchar_t* name = wcstok_s(
		nullptr,
		L"\0",
		&context
	);

	wcscat(NTFSBuffer, L"\\");
	wcscat(NTFSBuffer, name);

	return true;

}
bool AddRegistryPathToProtect(_In_ const PWCHAR Path) {
	return g_Registry.AddPath(Path);
}
bool ApplyRegistryPaths() {
	return g_Registry.Save();
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

	bool res = true;
	WCHAR buffer[DEFAULT_BUFFER_SIZE];
	res = ConvertDosPathToNTFSPath(
		Path,
		buffer,
		DEFAULT_BUFFER_SIZE
	);
	if (!res) {
		return false;
	}
	res = g_FilePaths.AddPath(buffer);
	if (!res) {
		return false;
	}
	m_AllFilePaths.push_back(Path);

	return res;

}
EXPORT bool ApplyFilePaths() {
	return g_FilePaths.Save(m_AllFilePaths);
}
EXPORT bool TestMatchFilePath(
	_In_ const PWCHAR Path,
	_Out_ bool& Found) {

	return g_FilePaths.TestAhoCorsickMatch(
		Path,
		Found
	);

}
EXPORT bool QueryReg(
	_Out_ PWCHAR Paths,
	_In_ int Size)
{
	return g_Registry.ReadBuildPaths(
		Paths,
		Size
	);
}

extern "C" EXPORT bool QueryMiniFilter(
	_Out_ PWCHAR Paths,
	_In_ int Size) {

	return g_FilePaths.ReadBuildPaths(
		Paths,
		Size
	);
}

extern "C" EXPORT bool RemoveRegPath(_In_ const PWCHAR Path) {
	return g_Registry.RemovePath(Path);
}
extern "C" EXPORT bool RemoveMiniFilterPath(_In_ const PWCHAR Path) {
	bool found = false;
	for (auto path : m_AllFilePaths) {
		if (path == Path) {
			m_AllFilePaths.remove(path);
			found = true;
			break;
		}
	}

	if (!found) {
		return false;
	}

	WCHAR buffer[DEFAULT_BUFFER_SIZE];
	bool res =
		ConvertDosPathToNTFSPath(Path, buffer, DEFAULT_BUFFER_SIZE);
	if (!res) {
		return false;
	}
	return g_FilePaths.RemovePath(buffer);

}