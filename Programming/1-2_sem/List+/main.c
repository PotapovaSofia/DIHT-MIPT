#include <stdio.h>
#include <stdlib.h>

struct List
{
  struct Link* head;
  struct Link* tail;
};

struct Link
{
    struct Link* next;
    struct Link* prev;
    int value;
};

struct List* list_new(void)
{
    struct List* lst = (struct List*)malloc(sizeof(struct List));
    if (!lst)
    {
        fprintf(stderr," ");
        return NULL;
    }
    lst -> head = NULL;

    lst -> tail = NULL;
    return lst;
};

void list_delete (struct List* lst)
{
    struct Link* b = lst ->head;
    while (b)
    {
        struct Link* t;
        t = b ->next;
        free(b);
        b = t;
    }
}

void push(struct List* lst, int v)
{
    struct Link* p = (struct Link*)malloc(sizeof (struct Link));
    p -> value=v;
    struct Link* b = lst ->tail;
    if (b) b ->next = p;
    else lst ->head = p;
    p -> next= NULL;
    p -> prev = b;
    lst ->tail = p;

    printf("%d ->", p->value);

}

int pop (struct List* lst)
{
    struct Link* p = lst ->tail;
    if(!lst->head) {printf ("Error. List is empty"); return;}
    int x;
    x=p->value;
    if (p==lst ->head)
        {lst ->head=lst ->tail=NULL;}
    else
    {
        p ->prev ->next = NULL;
        lst ->tail = p ->prev;
    }
    free(p);
    return x;
}

void unshift (struct List* lst, int v)
{
    struct Link* p = (struct Link*)malloc(sizeof(struct Link));
    p ->value = v;
    struct Link* b = lst ->head;
    if (b)
    {
        b -> prev = p;
    }
    else
    {
        lst ->tail = p;
    }
    lst ->head = p;
    p -> prev = NULL;
    p ->next = b;
}

int shift(struct List* lst)
{
    struct Link* p = lst ->head;
    if (!p) {printf ("Error"); return;}
    int x;
    x = p ->value;
    if (lst ->tail == p)
    {
        lst->head=lst ->tail = NULL;
    }
    else
    {
        p ->next ->prev = NULL;
        lst ->head = p ->next;

    }

    free(p);
    return x;
}
void swap (struct Link **a, struct Link **b)
{
	struct Link *t=*a;
	*a=*b;
	*b=t;
}
int reverse(struct List* lst)
{
	if (!lst->head)
		return 0;
	if (lst->head==lst->tail)
		return 1;
	struct Link* b=lst->head;
	while (b)//(b != lst->tail)
	{
		swap(&(b->prev), &(b->next));
		b=(b->prev);
	}
	//swap(b->prev, b->next);
	swap(&(lst->head), &(lst->tail));


	return 1;
}




int main()
{
    int n;
    printf("Number:");
    scanf("%d", &n);
     int i;

    struct List* lst = list_new();
     for (i = 0; i  < n; i++)
     {
         int v;
         scanf("%d", &v);
         //printf ("%d ", v);
         push(lst, v);

    }

    printf("\n");

     reverse(lst);
     unshift(lst, 5);

    //for (i = 0; i  < n+1; i++)
    while(lst->head)
     {
         int v;
         v = shift(lst);
         printf ("%d ->", v);
     }
    list_delete(lst);
     return 0;
}




