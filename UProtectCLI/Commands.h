#pragma once

#include <sal.h>
#include <memory>
#include <map>
#include <string>

#include <Windows.h>

enum class CommandTarget {
	Undefined,
	Reg,
	FilePath,
};

class Command {

public:
	virtual bool Execute(
		_In_ CommandTarget Target,
		_In_ const WCHAR* Extra,
		_Out_ std::wstring& Msg) = 0;
	virtual bool IsDefinedCommand()  = 0;
};

class UndefinedCommand :public Command {

public:
	bool Execute(
		_In_ CommandTarget Target,
		_In_ const WCHAR* Extra,
		_Out_ std::wstring& Msg) override;
	bool IsDefinedCommand() override;

};
class ApplyCommand : public Command {

public:
	bool Execute(
		_In_ CommandTarget Target,
		_In_ const WCHAR* Extra,
		_Out_ std::wstring& Msg) override;
	bool IsDefinedCommand() override;
};
class ProtectCommand : public Command {

public:
	bool Execute(
		_In_ CommandTarget Target,
		_In_ const WCHAR* Extra,
		_Out_ std::wstring& Msg) override;
	bool IsDefinedCommand() override;
};
class TestCommand : public Command {

public:
	bool Execute(
		_In_ CommandTarget Target,
		_In_ const WCHAR* Extra,
		_Out_ std::wstring& Msg) override;
	bool IsDefinedCommand() override;
};


class Commands
{
public:
	static Commands& GetInstance();

	Commands();
	~Commands();

	std::shared_ptr<Command> MakeCommand(_In_ const WCHAR* Str);
	CommandTarget MakeTarget(_In_ const WCHAR* Str);

private:
	std::shared_ptr<UndefinedCommand> m_UndefinedCommand;

	std::map<std::wstring, std::shared_ptr<Command>> m_Commands;
	std::map<std::wstring, CommandTarget> m_Targets;

};

