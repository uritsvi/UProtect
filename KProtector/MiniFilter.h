#pragma once

#include <fltKernel.h>

#include "Driver.h"
#include "AhoCorasickInterface.h"

#include "..\AhoCorasickWithDump-CPP\include\aho_corasick.h"

class MiniFilter
{
public:
	static MiniFilter* GetInstance();
	void Delete();

	MiniFilter();
	~MiniFilter();

	NTSTATUS Init(
		_In_ DRIVER_OBJECT* Driver, 
		_In_ UNICODE_STRING* Registry);
	NTSTATUS StartProtect();

	bool AllowToModify(_In_ UNICODE_STRING* Path);

private:
	NTSTATUS MiniFilter::InitMiniFilter(
		DRIVER_OBJECT* Driver
	);

	void tempInitRegistry(UNICODE_STRING* RegistryPath);

public:
	PFLT_FILTER m_Filter;

private:
	static MiniFilter* m_Instance;
	AhoCorasickInterface* m_AhoCorasick; 
};

