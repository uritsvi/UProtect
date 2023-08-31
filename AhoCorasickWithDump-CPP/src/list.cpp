#include <stddef.h>

#include "../include/list.h"
   
 void init_list(_Out_ ListEntry* Head) {  
 	Head->Next = NULL;  
 	Head->IterCurrent = NULL;  
 	Head->Len = 0;  
 }  
   
 void push_front(  
 	_In_ ListEntry* Head,  
 	_In_ ListEntry* Entry) {  
   
 	struct ListEntry* temp = Head->Next;  
   
 	Entry->Next = NULL;  
 	Entry->IterCurrent = NULL;  
   
 	Head->Next = (struct ListEntry*)Entry;  
 	Entry->Next = temp;  
   
 	Head->Len++;  
 }  
   
 void* start_iter_list(_In_ ListEntry* Head) {  
 	Head->IterCurrent = Head->Next;  
 	return Head->IterCurrent;  
 }  
 void* iter_list_next(_In_ ListEntry* Head) {

     ListEntry* iterCurrent = 
         (ListEntry*)Head->IterCurrent;

 	Head->IterCurrent = iterCurrent->Next;
 	return Head->IterCurrent;  
 }

 wchar_t* wcschr(const wchar_t* s, wchar_t c) {
     for (;;) {
         if (*s == c)
             return (wchar_t*)s;
         if (*s++ == L'\0')
             return NULL;
     }
 }

 wchar_t* lib_wcstok(wchar_t* s1, const wchar_t* s2, wchar_t** ptr) {
     wchar_t* p;

     if (s1 == NULL)
         s1 = *ptr;
     while (*s1 && wcschr(s2, *s1))
         s1++;
     if (!*s1) {
         *ptr = s1;
         return NULL;
     }
     for (p = s1; *s1 && !wcschr(s2, *s1); s1++)
         continue;
     if (*s1)
         *s1++ = L'\0';
     *ptr = s1;
     return p;
 }
