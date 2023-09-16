#include <list>

#include "..\..\Common\Common.h"
#include "..\include\KProtectDll.h"
#include "..\..\Log\Log.h"
#include "..\..\AhoCorasickWithDump-CPP\include\aho_corasick.h"
#include "..\..\Common\Common.h"
#include "..\include\AhoCorasickInterface.h"

#include <psapi.h>


#pragma warning(disable:4996)

AhoCorasick g_Registry;
AhoCorasick g_FilePaths;

std::list<std::wstring> m_AllFilePaths;

HANDLE g_DriverHandle;

extern "C" EXPORT bool RegistrInDriver();

bool ConvertDosPathToNTFSPath(
	_In_ const WCHAR* DosPath,
	_Out_ WCHAR* NTFSBuffer,
	_In_ int BufferSize
);

bool LoadDriver() {
	g_DriverHandle = 
		CreateFile(
			DRIVER_SYM_LINK, 
			FILE_READ_ACCESS | FILE_WRITE_ACCESS, 
			0, 
			nullptr, 
			OPEN_EXISTING, 
			0, 
			nullptr);

	if (g_DriverHandle == INVALID_HANDLE_VALUE) {
		return false;
	}

	RegistrInDriver();
	return true;
}

extern "C" EXPORT bool RegistrInDriver() {
	auto processId = GetCurrentProcessId();
	return 	DeviceIoControl(
		g_DriverHandle,
		IOCTL_DLL_LOAD,
		(LPVOID)processId,
		sizeof(processId),
		nullptr,
		0,
		nullptr,
		nullptr);
}

bool ShutDownd() {
	return DeviceIoControl(
		g_DriverHandle,
		IOCTL_DLL_UNLOAD,
		nullptr,
		0,
		nullptr,
		0,
		nullptr,
		nullptr);
}

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

		LoadDriver();

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
	bool res = 
		g_FilePaths.Save(m_AllFilePaths);
	if (!res) {
		return false;
	}

	if (g_DriverHandle != INVALID_HANDLE_VALUE) {
		/* Make the driver reload the policy */
		res = DeviceIoControl(
			g_DriverHandle, 
			IOCTL_RELOAD_MINIFILTER_POLICY, 
			nullptr, 
			0, 
			nullptr, 
			0, 
			nullptr, 
			nullptr
		);
	}

	return res;
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

extern "C" EXPORT bool StartDriver() {

	if (g_DriverHandle == INVALID_HANDLE_VALUE) {
		PROCESS_INFORMATION pi;
		STARTUPINFO si = { sizeof(STARTUPINFO) };

		
		CreateProcessW(
			L"C:\\Windows\\System32\\cmd.exe",
			(LPWSTR)L"/C sc start Kprotect",
			NULL,
			NULL,
			0,
			//CREATE_NO_WINDOW,
			0,
			NULL,
			NULL,
			&si,
			&pi);
		WaitForSingleObject(pi.hProcess, INFINITE);

		return LoadDriver();

	}

	return DeviceIoControl(
		g_DriverHandle,
		IOCTL_DRIVER_LOAD,
		nullptr,
		0,
		nullptr,
		0,
		nullptr,
		nullptr);

}
extern "C" EXPORT bool StopDriver() {
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;

	return DeviceIoControl(
		g_DriverHandle,
		IOCTL_DRIVER_UNLOAD,
		nullptr,
		0,
		nullptr,
		0,
		nullptr,
		nullptr);

}

extern "C" EXPORT bool IsDriverLoaded() {
	LPVOID drivers[DEFAULT_BUFFER_SIZE];
	DWORD cbNeeded;
	int cDrivers, i;

	if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
	{
		TCHAR szDriver[DEFAULT_BUFFER_SIZE];

		cDrivers = cbNeeded / sizeof(drivers[0]);

		for (i = 0; i < cDrivers; i++)
		{
			if (GetDeviceDriverBaseName(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0])))
			{
				if (wcscmp(szDriver, L"KProtector.sys") == 0) {
					return true;
				}
			}
		}
	}
	return false;

}