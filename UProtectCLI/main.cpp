#include <iostream>
#include <string>

#include "..\Log\Log.h"
#include "..\KProtectDll\include\KProtectDll.h"
#include "..\Common\Common.h"

#pragma comment(lib, "KProtectDll.lib")

int main() {


	bool res =
		InitKProtectInteface();

	while (true) {


		std::wcout << Log::White << L"Enter Command:" << std::endl;

		std::wstring command;
		std::getline(std::wcin, command);

		wchar_t* context = { 0 };
		wchar_t* current = wcstok_s(
			const_cast<wchar_t*>(command.data()),
			DELIMITER,
			&context);

		if (current == nullptr) {
			goto invalid_command;
		}

		if (wcscmp(current, PROTECT_COMMAND) == 0) {
			current = wcstok_s(
				nullptr,
				DELIMITER,
				&context);

			if (current == nullptr) {
				goto invalid_command;
			}

			if (wcscmp(current, REGISTRY) == 0) {
				current = wcstok_s(
					nullptr,
					DELIMITER,
					&context);

				if (current == nullptr) {
					goto invalid_command;
				}

				AddRegistryPathToProtect(current);

				std::wcout
					<< Log::Green
					<< L"Added registry path to protect \"" << current << L"\"" << std::endl;


				continue;

			}

		}
		else if (wcscmp(current, APPLY_COMMAND) == 0) {
			current = wcstok_s(
				nullptr,
				DELIMITER,
				&context);

			if (current == nullptr) {
				goto invalid_command;
			}


			if (wcscmp(current, REGISTRY) == 0) {
				current = wcstok_s(
					nullptr,
					DELIMITER,
					&context);

				ApplayRegistryPaths();

				std::wcout
					<< Log::Green
					<< "Apply reg paths" << std::endl;
				continue;

			}

			goto invalid_command;
		}
		else if (wcscmp(current, SAVE_COMMAND) == 0) {
			current = wcstok_s(
				nullptr,
				DELIMITER,
				&context);

			if (wcscmp(current, REGISTRY) == 0) {
				SaveBuildPaths();
			}

			std::wcout <<
				Log::Green <<
				"Save regitry paths build" << std::endl;
			continue;


		}
		else if (wcscmp(current, LOAD_COMMAND) == 0) {
			current = wcstok_s(
				nullptr,
				DELIMITER,
				&context);

			if (wcscmp(current, REGISTRY) == 0) {
				bool res =
					LoadRegBuildPaths();
				if (res) {
					std::wcout <<
						Log::Green <<
						"Load regitry paths" << std::endl;
					continue;
				}
			}
		}

	invalid_command:
		std::wcout
			<< Log::Red
			<< "\"" << command << "\" "
			<< "Invalid command" << std::endl;


	}
	return 0;

}