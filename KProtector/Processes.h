#pragma once

#include "..\KTL\include\FastMutex.h"


class Processes
{
public:
	static Processes* GetInstance();

	Processes();
	~Processes();

	NTSTATUS Init();


	bool IsProcessAllowedAccess(_In_ ULONG PID);

	bool IsControlApp(_In_ HANDLE Process);

	void SetControlAppPID(_In_ ULONG PID);
	void OnControlAppUnload();

private:
	static Processes* m_Instance;

	ULONG m_AppPID;
	bool m_ControlAppIsLoaded;

	FastMutex m_FastMutex;
};

