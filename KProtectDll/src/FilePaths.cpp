#include "..\include\FilePaths.h"

FilePaths::FilePaths() {
}
FilePaths::~FilePaths() {
}

bool FilePaths::Init() {
	bool load = true;
#ifdef _DEBUG
	load = false;
#endif

	bool res = m_AhoCorasick.Init(
		MINIFILTER_INFO_ROOT_PATH,
		load
	);
	return res;
}

bool FilePaths::Protect(_In_ std::wstring Key) {
	return m_AhoCorasick.AddPath(Key);
}
bool FilePaths::Apply() {
	return m_AhoCorasick.Save();
}

bool FilePaths::TestAhoCorsickMatch(
	_In_ const WCHAR* Path,
	_Out_ bool& Found) {
	
	m_AhoCorasick.TestAhoCorsickMatch(
		Path, 
		Found
	);

	return true;
}