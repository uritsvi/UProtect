#include "..\include\KTLMemory.hpp"

void* operator new(
	_In_ size_t size,
	_In_ POOL_FLAGS PoolFlags,
	_In_ ULONG DriverTag) {

	return ExAllocatePool2(
		PoolFlags,
		size,
		DriverTag);
}

void operator delete(void* p, size_t) {
	ExFreePool(p);
}
