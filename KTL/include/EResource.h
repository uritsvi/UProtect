#pragma once

#include <ntddk.h>

class ExecutiveResource {
public:
	void Init();
	void Delete();

	void Lock();
	void Unlock();

	void LockShared();
	void UnlockShared();

private:
	ERESOURCE m_res;
};

#ifdef KTL_NAMESPACE
}
#endif
