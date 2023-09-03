#ifndef __AHO__CORASICK__
#define __AHO__CORASICK__

#include <sal.h>
//#include <stdbool.h>

#include "..\..\AhoCorasickWithDump-CPP\include\list.h"
#include "..\..\AhoCorasickWithDump-CPP\include\lib.h"


#define MAX_LEAVES 1 << 16

#define INT_NULL_VALUE -100


typedef short Leaves[1];

struct BuildTrieEntry {
	struct BuildTrieEntry* Leaves[MAX_LEAVES];

	bool Root;

	wchar_t C;
	ListEntry Prefixes;
	struct BuildTrieEntry* FailureLink;

	short Index;
	int TravsFromRoot;

	bool OutputNode;
	int WordIndex;


};

struct FinalTrieEntry {
	int FailureLink;

	wchar_t C;

	bool OutputNode;
	bool Root;

	Leaves Leaves;
};


struct MatchContext {
	int I;
	int C;

};

struct WCharRange {
	int Min;
	int Max;
};

void make_wchar_range(_Out_ WCharRange* WCharRange);

void init_aho_corasick(
	_In_ MallocFunction Malloc,
	_In_ FreeFunction Free,
	_In_ CopyMemoryFunction);

void add_leaves(
	_In_ const wchar_t* Words,
	_Inout_ BuildTrieEntry* TrieEntry,
	_Out_ int* NumOfTries,
	_Out_ WCharRange* Range);

void make_final_trie(
	_In_ BuildTrieEntry* BuildEntry,
	_In_ int NumOfNodes,
	_In_ WCharRange* WCharRange,
	_Out_ FinalTrieEntry** FinalEntry,
	_Out_ int* BufferSize);

bool match(
	_In_ FinalTrieEntry* BaseAddr,
	_In_ FinalTrieEntry* Entry,
	_In_ const wchar_t* Str,
	_In_ MatchContext* Context,
	_In_ WCharRange* WCRange);



#endif