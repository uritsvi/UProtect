#pragma once

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

	bool m_Unloaded;
};

