#include "GlobalState.h"
#include "..\KTL\include\KTLMemory.h"
#include "Driver.h"

GlobalState* GlobalState::m_Instance;

GlobalState::GlobalState() {
	m_Unloaded = false;
	m_FastMutex.Init();
}
GlobalState::~GlobalState() {
}

GlobalState* GlobalState::GetInstance() {
	if (m_Instance == nullptr) {
		m_Instance = new (POOL_FLAG_PAGED, DRIVER_TAG)GlobalState();
	}
	return m_Instance;
}

bool GlobalState::IsDriverUnloaded() {

	m_FastMutex.Lock();

	bool res = m_Unloaded;

	m_FastMutex.Release();
	return res;
}

void GlobalState::UnloadDriver() {
	m_FastMutex.Lock();

	m_Unloaded = true;

	m_FastMutex.Release();
}
void GlobalState::LoadDriver() {
	m_FastMutex.Lock();

	m_Unloaded = false;

	m_FastMutex.Release();
}