#ifndef __LIB__
#define __LIB__

#include <sal.h>
#include <stddef.h>

typedef void* (*MallocFunction)(size_t size);
typedef void(*FreeFunction)(void*);
typedef void (*CopyMemoryFunction)(void* dest, const void* src, size_t count);

void init_lib(
	_In_ MallocFunction Malloc, 
	_In_ FreeFunction Free,
	_In_ CopyMemoryFunction CopyMemory);

void* lib_malloc(size_t size);
void lib_free(void* p);
void lib_copy_memory(void* dest, const void* src, size_t count);

wchar_t* lib_wcstok(wchar_t* s1, const wchar_t* s2, wchar_t** ptr);

#endif