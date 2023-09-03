#include <iostream>
#include <string>

#include "CLI.h"

#include "..\Log\Log.h"
#include "..\KProtectDll\include\KProtectDll.h"
#include "..\Common\Common.h"
#include "Messages.h"

#pragma comment(lib, "KProtectDll.lib")

#define MAX_COMMAND_SIZE_IN_BITES DEFAULT_BUFFER_SIZE

#define DELIMITER L" "


void CLI::Run() {
	bool res =
		InitKProtectInteface();

	while (true) {
		std::wcout << Log::White << ENTER_COMMAND;

		std::wstring commandString;
		std::getline(std::wcin, commandString);

		std::shared_ptr<Command> command;
		CommandTarget target;
		std::wstring extraParameter;

		if (commandString.size() > MAX_COMMAND_SIZE_IN_BITES) {
			std::wcout << Log::Red << COMMAND_TO_LONG << "\n";
		}

		std::wstring msg;
		bool res = ParsCommand(
			commandString, 
			command, 
			target,
			extraParameter,
			msg);

		if (!res) {
			std::wcout << Log::Red << msg << "\n";
			continue;
		}

		res = ProccessCommand(
			command, 
			target, 
			extraParameter, 
			msg
		);
		if (!res) {
			std::wcout << Log::Red << msg << "\n";
			continue;
		}
		
		std::wcout << Log::Green << msg << "\n";
	}
}
bool CLI::ParsCommand(
	_In_ std::wstring& CommandString,
	_Out_ std::shared_ptr<Command>& Command,
	_Out_ CommandTarget& Target,
	_Out_ std::wstring& ExtraParameter,
	_Out_ std::wstring& Msg) {

	Command = nullptr;

	ExtraParameter = L"";
	Msg = L"";

	Target = CommandTarget::Undefined;

	bool res = true; 
	do {
		wchar_t commandBuffer[DEFAULT_BUFFER_SIZE] = {0};
		CommandString.copy(
			commandBuffer,
			DEFAULT_BUFFER_SIZE,
			0
		);

		wchar_t* context = { 0 };
		auto current = wcstok_s(
			commandBuffer,
			DELIMITER,
			&context);
		if (current == nullptr) {
			Msg = COMMAND_NOT_ENOUGH_PARAMETERS;

			res = false;
			return res;
		}
		Command =
			Commands::GetInstance().MakeCommand(current);

		current = wcstok_s(
			nullptr,
			DELIMITER,
			&context);
		if (current == nullptr) {
			Msg = COMMAND_NOT_ENOUGH_PARAMETERS;

			res = false;
			return res;
		}
		Target =
			Commands::GetInstance().MakeTarget(current);

		current = wcstok_s(
			nullptr,
			DELIMITER,
			&context);

		// Handle extra parameter
		if (current == nullptr) {
			break;
		}
		ExtraParameter =
			std::wstring(current);

		/*
		* If there is more parameters than 
		* Needed the function returns an error
		*/
		current = wcstok_s(
			nullptr,
			DELIMITER,
			&context);
		if (current != nullptr) {
			Msg = TO_MATCH_PARAMETERS;

			res = false;
			return res;
		}

	} while (false);
	
	return res;
}

bool CLI::ProccessCommand(
	_In_ std::shared_ptr<Command> Command,
	_In_ CommandTarget& Target,
	_In_ std::wstring ExtraParameter,
	_Out_ std::wstring& Msg) {

	bool res = true;
	do {
		res = Command->IsDefinedCommand();
		if (!res) {
			Msg = COMMAND_IS_UNDEFINED;
			break;
		}

		res =
			Command->Execute(
				Target, 
				ExtraParameter.c_str(),
				Msg
			);
		if (!res) {
			Msg = COMMAND_EXECUTION_FAILED;
			break;
		}
	} while (false);

	return res;
}