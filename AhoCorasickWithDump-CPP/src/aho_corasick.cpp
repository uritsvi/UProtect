#include <string.h>

#include "../include/aho_corasick.h"
#include "../include/hash_table.h"
#include "../include/lib.h"

#define MAX_WCHAR_VALUE (1 << 16)

typedef struct {
    FullContext* FullContex;

    int TravsFromRoot;
    size_t CurrentMaxLen;
}WordContext;

typedef struct {
    ListEntry Entry;
    wchar_t Prefix[1];
} Prefix;

typedef struct {
    ValueEntry Entry;
    BuildTrieEntry* TrieEntry;
} MapValue;

static int calc_node_size(_In_ WCharRange* Range) {
    return (sizeof(FinalTrieEntry) +
        ((Range->Max - Range->Min + 1) * sizeof(Leaves)));
}

static void handle_word(
    _In_ wchar_t* Buffer,
    _In_ int Index,
    _In_ BuildTrieEntry* Trie,
    _In_ WordContext* WordContext,
    _In_ int WordIndex,
    _Out_ WCharRange* WCharRange) {

    WordContext->TravsFromRoot++;

    BuildTrieEntry* target_entry =
        (BuildTrieEntry*)Trie->Leaves[Buffer[Index]];

    if (target_entry == NULL) {
        if (Buffer[Index] > WCharRange->Max) {
            WCharRange->Max = Buffer[Index];
        }
        if (Buffer[Index] < WCharRange->Min) {
            WCharRange->Min = Buffer[Index];
        }


        target_entry =
            (BuildTrieEntry*)
            lib_malloc(sizeof(BuildTrieEntry));

        memset(
            target_entry,
            0,
            sizeof(BuildTrieEntry));

        init_list(&target_entry->Prefixes);

        target_entry->C = Buffer[Index];
        target_entry->TravsFromRoot = WordContext->TravsFromRoot;
        target_entry->OutputNode = false;
        target_entry->Index = WordContext->FullContex->NumOfIndexes++;


        int c = WordContext->TravsFromRoot - 1;
        do {
            Prefix* prefix = (Prefix*)lib_malloc(
                sizeof(Prefix) +
                (WordContext->TravsFromRoot) * sizeof(wchar_t)
            );

            memset(
                prefix,
                0,
                sizeof(Prefix) +
                (WordContext->TravsFromRoot) * sizeof(wchar_t));

            lib_copy_memory((void*)prefix->Prefix,
                Buffer + 1,
                (c) * sizeof(wchar_t));

            push_front(
                &target_entry->Prefixes,
                &prefix->Entry);


        } while (--c >= 0);


        MapValue* value = (MapValue*)lib_malloc(
            sizeof(MapValue)
        );

        value->TrieEntry = target_entry;

        push_value_to_map(
            &WordContext->FullContex->Prefixes,
            Buffer,
            WordContext->TravsFromRoot,
            &value->Entry);

        Trie->Leaves[Buffer[Index]] = (struct BuildTrieEntry*)target_entry;
    }

    if (Index == WordContext->CurrentMaxLen - 1) {
        target_entry->WordIndex = WordIndex;
        target_entry->OutputNode = true;
        return;
    }

    handle_word(
        Buffer,
        Index + 1,
        target_entry,
        WordContext,
        WordIndex,
        WCharRange);
}

static bool handle_prefix(
    _In_ FullContext* Context,
    _In_ ListEntry* Head,
    _In_ Prefix* Current,
    _Inout_ BuildTrieEntry* CurrentLeaf) {

    if (Current != NULL) {
        bool res = handle_prefix(
            Context,
            Head,
            reinterpret_cast<Prefix*>(iter_list_next(Head)),
            CurrentLeaf);
        if (res) {
            return res;
        }

        MapValue* entry = NULL;
        res = query_map_value(
            &Context->Prefixes,
            Current->Prefix,
            wcslen(Current->Prefix),
            (ValueEntry**)&entry);

        if (!res) {
            return false;
        }

        CurrentLeaf->FailureLink =
            (struct BuildTrieEntry*)entry->TrieEntry;

        return true;
    }

    return false;
}

static void handle_make_faliure_links(
    _In_ BuildTrieEntry* Trie,
    _In_ FullContext* Context) {

    ListEntry* prefixes = NULL;
    for (int i = 0; i < MAX_KEYS; i++) {
        if (Trie->Leaves[i] == NULL) {
            continue;
        }

        BuildTrieEntry* current =
            (BuildTrieEntry*)Trie->Leaves[i];

        prefixes = &current->Prefixes;

        handle_prefix(
            Context,
            prefixes,
            (Prefix*)start_iter_list(prefixes),
            current);

        handle_make_faliure_links(
            (BuildTrieEntry*)Trie->Leaves[i],
            Context);

    }

    if (prefixes == NULL) {
        return;
    }

}

void init_aho_corasick(
    _In_ MallocFunction Malloc,
    _In_ FreeFunction Free,
    _In_ CopyMemoryFunction CopyMemory) {

    init_lib(
        Malloc,
        Free,
        CopyMemory);
}

void init_full_context(_Out_ FullContext* Context) {
    Context->NumOfIndexes = 1;

    init_map(
        &Context->Prefixes,
        true);
}

/*
* ';' Seperator between words
*/
void add_leaves(
    _In_ const wchar_t* Words,
    _Inout_ BuildTrieEntry* TrieEntry,
    _Out_ int* NumOfTries,
    _Out_ WCharRange* WCharRange,
    _In_ FullContext* Context) {

    *NumOfTries = 0;

    size_t words_buffer_len =
        (wcslen(Words) + 1);

    wchar_t* words_buffer =
        (wchar_t*)lib_malloc(words_buffer_len * sizeof(wchar_t));

    wcscpy_s(
        words_buffer,
        words_buffer_len,
        Words);



    wchar_t* wcsstokContext = { 0 };

    wchar_t* current = lib_wcstok(
        words_buffer,
        L";",
        &wcsstokContext);

    int c = 0;
    while (current != NULL)
    {
        WordContext word_context = { 0 };
        word_context.FullContex = Context;
        word_context.CurrentMaxLen = wcslen(current);

        handle_word(
            current,
            0,
            TrieEntry,
            &word_context,
            c++,
            WCharRange
        );

        current = lib_wcstok(
            NULL,
            L";",
            &wcsstokContext);
    }

    handle_make_faliure_links(
        TrieEntry,
        Context);

    TrieEntry->Root = true;
    *NumOfTries = Context->NumOfIndexes;

}

static void convert_node_to_final(
    _In_ BuildTrieEntry* BuildEntry,
    _Out_ void* Buffer,
    _In_ WCharRange* Range) {

    int range_size =
        (Range->Max - Range->Min);
    int entry_size = calc_node_size(Range);

    FinalTrieEntry* entry =
        (FinalTrieEntry*)(
            (char*)Buffer +
            (BuildEntry->Index * entry_size));

    for (int i = 0; i <= range_size; i++) {
        entry->Leaves[i] = INT_NULL_VALUE;
    }

    BuildTrieEntry* failure =
        (BuildTrieEntry*)BuildEntry->FailureLink;

    entry->C = BuildEntry->C;
    entry->OutputNode = BuildEntry->OutputNode;
    entry->Root = BuildEntry->Root;

    entry->FailureLink = 0;
    if (failure != NULL) {
        entry->FailureLink = failure->Index;

    }

    for (int i = Range->Min; i <= Range->Max; i++) {
        if (BuildEntry->Leaves[i] == NULL) {
            continue;
        }

        BuildTrieEntry* leaf =
            (BuildTrieEntry*)BuildEntry->Leaves[i];

        entry->Leaves[i - Range->Min] = (short)leaf->Index;

        convert_node_to_final(
            (BuildTrieEntry*)BuildEntry->Leaves[i],
            Buffer,
            Range);


    }


}

void make_final_trie(
    _In_ BuildTrieEntry* BuildEntry,
    _In_ int NumOfNodes,
    _In_ WCharRange* WCharRange,
    _Out_ FinalTrieEntry** FinalEntry,
    _Out_ int* BufferSize) {

    int rangeSize = WCharRange->Max - WCharRange->Min;
    if (rangeSize < 0) {
        *FinalEntry = NULL;
        *BufferSize = 0;
        return;
    }

    *BufferSize =
        NumOfNodes * calc_node_size(WCharRange);

    *FinalEntry =
        reinterpret_cast<FinalTrieEntry*>(lib_malloc(*BufferSize));

    memset(
        *FinalEntry,
        0,
        *BufferSize);

    convert_node_to_final(
        BuildEntry,
        *FinalEntry,
        WCharRange);



}

bool match(
    _In_ FinalTrieEntry* BaseAddr,
    _In_ FinalTrieEntry* Entry,
    _In_ const wchar_t* Str,
    _In_ int Len,
    _In_ MatchContext* Context,
    _In_ WCharRange* WCharRange) {

    while (true)
    {
        if (Context->I >= Len || Entry == nullptr || WCharRange == nullptr) {
            return false;
        }

        int entry_size =
            calc_node_size(WCharRange);

        if (Entry == nullptr) {
            return false;
        }

        int next_index = INT_NULL_VALUE;
        if (Str[Context->I] >= WCharRange->Min &&
            Str[Context->I] <= WCharRange->Max)
        {
            next_index = Entry->Leaves[Str[Context->I] - WCharRange->Min];
        }

        FinalTrieEntry* next;


        if (next_index >= 0) {

            next =
                (FinalTrieEntry*)(
                    (char*)BaseAddr + (next_index * entry_size));

            if (next->OutputNode == true) {
                return true;
            }

            Context->I++;

        }
        else {
            next_index =
                Entry->FailureLink;
            next =
                (FinalTrieEntry*)(
                    (char*)BaseAddr +
                    (next_index * entry_size));

            if (Entry->Root) {

                Context->I++;
                next = Entry;
            }
        }

        Entry = next;

    }
}

void make_wchar_range(_Out_ WCharRange* WCharRange) {
    if (WCharRange == 0) {
        return;
    }

    WCharRange->Min = MAX_WCHAR_VALUE;
    WCharRange->Max = 0;

}