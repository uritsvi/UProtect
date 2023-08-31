#include "../include/lib.h"

MallocFunction g_MallocFunction;
FreeFunction g_FreeFunction;
CopyMemoryFunction g_CopyMemoryFunction;

void init_lib(
	_In_ MallocFunction Malloc,
	_In_ FreeFunction Free,
	_In_ CopyMemoryFunction CopyMemory) {

	g_MallocFunction = Malloc;
	g_FreeFunction = Free;
	g_CopyMemoryFunction = CopyMemory;
}

void* lib_malloc(size_t size) {
	return g_MallocFunction(size);
}

void lib_free(void* p) {
	g_FreeFunction(p);
}

void lib_copy_memory(void* dest, const void* src, size_t count) {
	return g_CopyMemoryFunction(dest, src, count);
}