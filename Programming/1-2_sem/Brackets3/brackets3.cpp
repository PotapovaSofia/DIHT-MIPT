#include <cstdio>
#include <stdlib.h>
#include <conio.h>

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

int main()
{
	int i, key, key2, n = 0;
	char c, a[255];
	bool b;
	while( (c = getchar()) != '\n' )
		a[n++] = c;
	if (n % 2 == 1)
	{
		printf("NO");
		getch();
		return 0;
	}
	List *l = list_new();
	for(i = 0; i < n; ++i)
	{
		switch ( a[i] )
		{
			case '(': key = 1;
				break;
			case '{': key = 2;
				break;
			case ')': key = -1;
				break;
			case '}': key = -2;
				break;
			default: printf("Unexpected simbol");
					 getch();
					 return 0;
		}
		if (i == 0) {
			if (key < 0)	
			{
				printf("NO");
				getch();
				return 0;
			} else
				b = push(l, key);
				if (b == false) {	
					printf("no");
					getch();
					return 0;
				}
		}
		else {
			if (key > 0) {
				b = push(l, key);
				if (b == false) {	
					printf("no");
					getch();
					return 0;
				}
			}
			if (key < 0)
			{
				b = pop(l, key2);
				if (b == false) {	
					printf("no");
					getch();
					return 0;
				}
				if (key2 != (-1)*key)
				{
					printf("NO");
					getch();
					return 0;
				}
			}
		}
	}
	if (l->first != NULL) {
		printf("NO");
		getch();
		return 0;
	} else printf("YES");
	getch();
	return 0;
}
