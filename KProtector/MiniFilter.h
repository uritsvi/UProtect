#pragma once

#include <fltKernel.h>

#include "Driver.h"
#include "AhoCorasickInterface.h"

#include "..\AhoCorasickWithDump-CPP\include\aho_corasick.h"

class MiniFilter
{
public:
	static MiniFilter* GetInstance();

	MiniFilter();
	~MiniFilter();

	NTSTATUS Init(
		_In_ DRIVER_OBJECT* Driver, 
		_In_ UNICODE_STRING* Registry);
	NTSTATUS StartProtect();

	NTSTATUS ReloadPolicy();

	bool AllowToModify(_In_ UNICODE_STRING* Path);

private:
	NTSTATUS MiniFilter::InitMiniFilter(
		DRIVER_OBJECT* Driver
	);

	void InitRegistry(UNICODE_STRING* RegistryPath);


private:
	static MiniFilter* m_Instance;
	
	PFLT_FILTER m_Filter;
	AhoCorasickInterface m_AhoCorasick; 
};

