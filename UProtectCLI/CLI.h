#pragma once

#include <Windows.h>

#include <sal.h>
#include <string>
#include <list>

#include "Commands.h"

class CLI
{
public:
	void Run();

private:
	bool ParsCommand(
		_In_ std::wstring& CommandString,
		_Out_ std::shared_ptr<Command>& Command,
		_Out_ CommandTarget& Target,
		_Out_ std::wstring& ExtraParameter,
		_Out_ std::wstring& Msg
	);

	bool ProccessCommand(
		_In_ std::shared_ptr<Command> Command,
		_In_ CommandTarget& Target,
		_In_ std::wstring ExtraParameter,
		_Out_ std::wstring& Msg
	);

	void FillCommandTranslation();
	void FillCommandTargetTranslation();

};

