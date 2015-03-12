#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

struct dList {
	int val;
	struct dList* next;
	struct dList* prev;
};

struct myList {
	int sz;
	struct dList* first;
	struct dList* second;
}; 

myList* newList(const int& val);
void push(myList* q, const int& a);
void pop(myList* q);
void sizeoflist(myList* q, int* a);
int sizeoflist(myList* q);
int seetop(myList* q);
int seetop2(myList* q);

#endif // LIST_H_INCLUDED