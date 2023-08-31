#ifndef __LIST__  
#define __LIST__  
   
 #include <sal.h>  

#ifdef __cplusplus
struct ListEntry {
    struct ListEntry* Next;
    struct ListEntry* IterCurrent;

    // Is used just with lists heads  
    int Len;
 };
#else
typedef struct{  
 	struct ListEntry* Next;  
 	struct ListEntry* IterCurrent;  
   
 	// Is used just with lists heads  
 	int Len;   
 }ListEntry;
#endif 
  
 void init_list(_Out_ ListEntry* Head);  
 void push_front(  
 	_In_ ListEntry* Head,  
 	_In_ ListEntry* Entry);  
   
 void* start_iter_list(_In_ ListEntry* Head);  
 void* iter_list_next(_In_ ListEntry* Head);  
   
 #endif  
 