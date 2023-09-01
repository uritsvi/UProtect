#include <string>

#include "..\include\Registry.h"
#include "..\..\Common\Common.h"

#include "..\include\RAIIRegistry.h"

#define BUILD_PATHS_VALUE_NAME L"Build"



bool Registry::Protect(std::wstring Key) {
	return true;
}
bool Registry::Apply() {
	return true;
}
bool Registry::Save() {
	return true;
}
bool Registry::Load() {
	return true;
}
