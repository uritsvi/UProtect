#pragma once 

#include <ntddk.h>

class FastMutex {
public:
	void Init();
	
	void Lock();
	void Release();

private:
	FAST_MUTEX m_Mutex;
};