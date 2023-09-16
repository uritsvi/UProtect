#pragma once

#include "..\KTL\include\FastMutex.h"

class GlobalState
{
public:
	GlobalState();
	~GlobalState();

	static GlobalState* GetInstance();

	bool IsDriverUnloaded();
	
	void UnloadDriver();
	void LoadDriver();

private:
	static GlobalState* m_Instance;

	FastMutex m_FastMutex;
	bool m_Unloaded;
};

