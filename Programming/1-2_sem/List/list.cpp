#include <cstdio>
#include <stdlib.h>

struct Node;
struct List;
struct Node
{
	Node* prev;
	Node* next;
	int key;
};
struct List
{
	Node* first;
	Node* last;
	int count;
};
void swap (Node* &a, Node* &b)
{
	Node* t = a;
	a = b;
	b = t;
}
List* list_new()
{
    List *p = (List *)malloc(sizeof(List));
	if (p == NULL)
		return NULL;
    p->first = NULL;
	p->last = NULL;
	p->count = 0;
    return p;
}
void list_delete(List* a)
{
	if (a != NULL)
	{
		Node* t = a->first;
		while (a->count != 0)
		{
			Node* r = t->next;
			free(t);
			t = r;
		}
		free(a);
	}
}
bool push(List *a,int &k)
{
	if (a == NULL)
		return false;
	Node *t = (Node*)malloc(sizeof(Node));
	if (t == NULL)
		return false;
	if (a->count == 0)
	{
		a->first = t;
		a->last = t;
		t->next = NULL;
		t->prev = NULL;
		t->key = k;
		a->count = 1;
	}
	else
	{
		t->prev = a->last;
		a->last = t;
		t->next = NULL;
		t->key = k;
		a->count++;
	}
		return true;
}
bool pop(List* l, int &x)
{
	if (l->first==NULL)
		return false;
	Node *b = l->last;
	x = b->key;
	if(b == l->first)
	{
		free(b);
		l->first = NULL;
		l->last = NULL;
		l->count = 0;
	}else
	{	
		Node *t = (l->last)->prev;
		free(l->last);
		l->last = t;
		t->next = NULL;
		l->count--;
		}
	return true;
}
bool unshift(List *l, int a)
{
	Node* b = (Node*)malloc(sizeof(Node));
	if (b == NULL)
		return false;
	if ((l->first) == NULL)
	{    
		b->next = NULL;
		l->first = b;
		l->last = b;
		b->key = a;
		l->count = 1;
	}else
	{
		Node* n = l->first;
		b = l->first;
		b->prev = NULL;
		b->next = n;
		b->key = a;
		n->prev = b;
		l->count++;
	}
	 return true;
}
bool shift(List* l, int &x)
{
	if(l->first == NULL)
		return false;
	Node *b = l->first;
	x = b->key;
	if(b == l->last)
	{
		free(b);
		l->first = NULL;
		l->last = NULL;
		l->count = 0;
	}else
	{	
		Node *t= b->next;
		free(b);
		l->first = t;
		t->prev = NULL;
		l->count--;
	}
	return true;
}
bool reverse(List *l)
{
	if (l->first == NULL)
		return false;
	if (l->first == l->last)
		return true;
	Node *a = l->first;
	Node *b = l->last;
	for (int i = 0; i < (l->count)/2; ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			a = a->next;
			b = b->prev;
		}
		swap(a, b);
	}
	swap(l->first, l->last);
	return true;
}
int main()
{
	int n;
	scanf("%d", &n);
	List *l = list_new();
	return 0;
}
