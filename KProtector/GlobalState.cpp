#include "GlobalState.h"
#include "..\KTL\include\KTLMemory.hpp"
#include "Driver.h"

GlobalState* GlobalState::m_Instance;

GlobalState::GlobalState() {
	m_Unloaded = false;
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
	return m_Unloaded;
}

/*
* TODO: Add locks
* 
*/
void GlobalState::UnloadDriver() {
	m_Unloaded = true;
}
void GlobalState::LoadDriver() {
	m_Unloaded = false;
}