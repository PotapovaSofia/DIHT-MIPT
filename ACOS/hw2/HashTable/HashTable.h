#include <stdlib.h>
#include <stdio.h>

#define CHECK_NOT_NULL(ptr, return_value)\
	do {if(ptr==NULL) return return_value;} while(0)

#define comp(a,b) (a==b)

typedef struct Node_
{
	void* data;				             
	char* key;
	struct Node_ *next;			            
} Node;

typedef struct
{
	Node** table;
	size_t table_size;
	size_t capacity;
	size_t size_of_data;
} HashTable;

HashTable* htb_create(size_t x);
void htb_destroy(HashTable* htb);
int hash(char* key, size_t N);
int htb_insert(HashTable* htb, void* data, char* key);
int htb_delete(HashTable* htb, char* key);          
void* htb_find(HashTable* htb, char* key);                    
