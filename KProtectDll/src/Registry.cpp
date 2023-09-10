#include "..\include\Registry.h"
#include "..\..\Common\Common.h"

Registry::Registry() {
}
Registry::~Registry() {
}

bool Registry::Init() {
	bool load = true;
#ifdef _DEBUG
	load = false;
#endif

	return m_AhoCorasick.Init(
		REG_INFO_ROOT_PATH, 
		load
	);
}

bool Registry::Protect(_In_ std::wstring Key) {
	return m_AhoCorasick.AddPath(Key);
}
bool Registry::Apply() {
	return m_AhoCorasick.Save();
}

bool Registry::TestAhoCorsickMatch(
	_In_ const WCHAR* Path,
	_Out_ bool& Found) {

	return m_AhoCorasick.TestAhoCorsickMatch(
		Path, 
		Found
	);
}
