#pragma once 

#include <ntddk.h>


void* operator new(
	_In_ size_t size,
	_In_ POOL_FLAGS PoolFlags,
	_In_ ULONG DriverTag);

void operator delete(void* p, size_t);