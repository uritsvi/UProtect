#pragma once

// Registry keys
#define REG_INFO_ROOT_PATH L"SOFTWARE\\UProtect\\REG"
#define REG_BUILD_PATHS L"SOFTWARE\\UProtect\\REG\\Build"

#define REG_TRIE_SIZE_VALUE_NAME L"Size"
#define REG_TRIE_WCHAR_RANGE_VALUE_NAME L"Range"
#define REG_SUB_TRIE_INFO_NAME L"Info"

#define DEFAULT_BUFFER_SIZE (1 << 10) 


#define DELIMITER L" "

#define PROTECT_COMMAND L"protect"
#define APPLY_COMMAND L"apply"
#define SAVE_COMMAND L"save"
#define LOAD_COMMAND L"load"

#define REGISTRY L"reg"