#pragma once 

#include <ntddk.h>

template <class T>
class FullItem {
	LIST_ENTRY Entry;
	T Data;
};