#include "HashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


HashTable* htb_create(size_t x)              
{
    	HashTable* htb = (HashTable*) malloc(sizeof(HashTable));
    	CHECK_NOT_NULL(htb, NULL);
    	htb->size_of_data = x;
    	htb->capacity = 16;
    	htb->table_size = 0;
    	htb->table = malloc(sizeof(Node*)*(htb->capacity));
    	CHECK_NOT_NULL(htb->table, NULL);
    	size_t i;
    	for(i = 0; i < htb->capacity; ++i)
    	{
        	htb->table[i] = NULL;
    	}
    	return htb;
}

void htb_destroy(HashTable* htb)
{
    	Node* p;
    	Node* p0;
    	size_t i;
    	for(i = 0; i < htb->table_size; ++i)   // delete all links
    	if(htb->table[i] != NULL)
    	{
        	p0 = htb->table[i];
        	while(p0)
        	{
            		p = p0->next;
            		free(p0);
            		p0 = p;
        	}
    	}
    	free(htb->table);
    	free(htb);
}

int hash(char* key, size_t N) 
{
	int asciisum = 0;
	int i;
    	for (i = strlen(key)-1; i >= 0; --i) 
    	{
        	asciisum = (int)(key[i]) + 2*asciisum;
    	}
    	return asciisum % (int)N;
}

int htb_insert(HashTable* htb,  void* data, char* key)
{

	Node* p;
	Node* p0;
	int bucket;
	/*allocate node for data and insert in table*/
	/*insert node at the beggining of list*/
	bucket = hash(key, htb->capacity);
	p = malloc(sizeof(Node));
	CHECK_NOT_NULL(p, "out of memory\n");
	p0 = htb->table[bucket];
	if(!p0)
    	{
        	if((htb->table_size +1) > (htb->capacity)/2)
        	{
            		htb->capacity *=2;
            		Node** ntable = realloc(htb->table, (htb->capacity)*sizeof(Node*));
            		CHECK_NOT_NULL(ntable, NULL);
            		htb->table = ntable;
        	}
        	htb->table_size +=1;
    	}
	htb->table[bucket] = p;
	p->next = p0;
	p->data = malloc(htb->size_of_data);
	CHECK_NOT_NULL(p->data, NULL);
	p->data = data;
	p->key = key;
	return 0;
}

int htb_delete(HashTable* htb, char* key)
{
	Node* p;
	Node* p0;
	int bucket;
	/*delete node containing data from table*/
	/*find node*/
	p0 = NULL;
	bucket = hash(key, htb->capacity);
	p = htb->table[bucket];
	while(p && !comp(p->key, key))
	{
		p0 = p;
		p = p->next;
	}
	if(!p) return 0; //didn't find
	/*p designates node to delete, remove if from list*/
	if(p0)
		p0->next = p->next;
		/*not first node, p0 points to previous node*/
	else
		htb->table[bucket] = p->next;
	free(p);
	return 0;
}

void* htb_find(HashTable* htb, char* key)
{
	Node* p;
    	int bucket;
	bucket = hash(key, htb->capacity);
	p = htb->table[bucket];
	while(p && !comp(p->key, key))
		p = p->next;
	if(!p)
    	{
        printf("Can't find\n");
        return -1;
	}
	return p->data;
}
