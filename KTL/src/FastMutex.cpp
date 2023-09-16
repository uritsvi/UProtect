#include "..\include\FastMutex.h"

void FastMutex::Init() {
	ExInitializeFastMutex(&m_Mutex);
}

void FastMutex::Lock() {
	ExAcquireFastMutex(&m_Mutex);
}
void FastMutex::Release() {
	ExReleaseFastMutex(&m_Mutex);
}
