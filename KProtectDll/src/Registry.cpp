#include <string>

#include "..\include\Registry.h"
#include "..\..\Common\Common.h"
#include "..\include\RAIIRegistry.h"

Registry::Registry() {
	auto registry = 
		std::make_shared<RAIIReigstryKey>(
			HKEY_LOCAL_MACHINE, 
			REG_INFO_ROOT_PATH, 
			KEY_ALL_ACCESS, 
			FALSE);

	m_AhoCorasick = 
		std::make_shared<AhoCorasick>(
			registry
	);

}
Registry::~Registry() {
}

bool Registry::Init() {
	bool loadBuiledPaths = true;
#ifdef _DEBUG
	loadBuiledPaths = false;
#endif
	
	return m_AhoCorasick->Init(loadBuiledPaths);
}

bool Registry::Protect(std::wstring Key) {
	return m_AhoCorasick->AddPath(Key);
}
bool Registry::Apply() {
	return m_AhoCorasick->Save();
}

bool Registry::TestAhoCorsickMatch(
	_In_ const WCHAR* Path,
	_Out_ bool& Found) {

	Found = false;
	return m_AhoCorasick->TestAhoCorsickMatch(
		Path, 
		Found
	);
}