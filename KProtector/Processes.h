#pragma once

//#include <ntddk.h>

//#define NULL_PROCESS_ID -100

class Processes
{
public:
	static Processes* GetInstance();


	/*
	void ShutDown();

	void SelfPcreateProcessNotifyRoutineEx(
		_Inout_           PEPROCESS Process,
		_In_                HANDLE ProcessId,
		_Inout_ PPS_CREATE_NOTIFY_INFO CreateInfo);

	bool IsAppAlowed(_In_ ULONG PID);


	*/
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

	//LONG64 g_AllowedProcessId = -100;
};

