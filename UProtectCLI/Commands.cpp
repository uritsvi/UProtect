#include "Commands.h"
#include "..\KProtectDll\include\KProtectDll.h"
#include "..\Log\Log.h"

#include "Messages.h"

#define PROTECT_COMMAND L"protect"
#define APPLY_COMMAND L"apply"
#define TEST_COMMAND L"test"

#define REGISTRY L"reg"

Commands& Commands::GetInstance() {
	static auto instance = Commands();
	return instance;
}

Commands::Commands() {
	m_UndefinedCommand = std::make_shared<UndefinedCommand>();

	m_Commands[PROTECT_COMMAND] = std::make_shared<ProtectCommand>();
	m_Commands[APPLY_COMMAND] = std::make_shared<ApplyCommand>();
	m_Commands[TEST_COMMAND] = std::make_shared<TestCommand>();

	m_Targets[REGISTRY] = CommandTarget::Reg;
}
Commands::~Commands() {
}

std::shared_ptr<Command> Commands::MakeCommand(_In_ const WCHAR* Str) {
	auto res = m_Commands[Str];
	if (res == nullptr) {
		return m_UndefinedCommand;
	}
	return res;
}
CommandTarget Commands::MakeTarget(_In_ const WCHAR* Str) {
	return m_Targets[Str];
}

bool UndefinedCommand::Execute(
	_In_ CommandTarget Target,
	_In_ const WCHAR* Extra,
	_Out_ std::wstring& Msg) {
	
	return false;
}
bool UndefinedCommand::IsDefinedCommand() {
	return false;
}

bool ApplyCommand::Execute(
	_In_ CommandTarget Target,
	_In_ const WCHAR* Extra,
	_Out_ std::wstring& Msg) {
	
	bool res = true;

	if (Target == CommandTarget::Undefined) {
		return false;
	}

	switch(Target) {
		case CommandTarget::Reg: 
			res = ApplayRegistryPaths();break;
	}
	Msg = SUCCESSFULY_APPLYED_PATHS;
	return res;
}
bool ApplyCommand::IsDefinedCommand() {
	return true;
}

bool ProtectCommand::Execute(
	_In_ CommandTarget Target,
	_In_ const WCHAR* Extra,
	_Out_ std::wstring& Msg) {

	bool res = true;
	if (Extra == nullptr || Target == CommandTarget::Undefined) {
		return false;
	}

	switch(Target) {
		case CommandTarget::Reg: 
			res = AddRegistryPathToProtect((PWCHAR)Extra); break;
	}
	Msg = SUCCESSFULY_PROTECT_PATHS; 
	return res;
}
bool ProtectCommand::IsDefinedCommand() {
	return true;
}

bool TestCommand::Execute(
	_In_ CommandTarget Target,
	_In_ const WCHAR* Extra,
	_Out_ std::wstring& Msg) {

	if (Extra == nullptr || Target == CommandTarget::Undefined) {
		return false;
	}

	bool res = false;
	bool match = false;

	switch (Target) {
	case CommandTarget::Reg:
		res = TestMatchRegistry(
			(PWCHAR)Extra, 
			match
		);
	}
	if (match) {
		Msg = SUCCESS_TEST_PATHS;
	}
	if(res && !match) {
		Msg = FAILED_TEST_PATHS;
	}

	return res;
}
bool TestCommand::IsDefinedCommand() {
	return true;
}