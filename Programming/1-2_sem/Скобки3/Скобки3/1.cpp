#include <stdio.h>
#include <stdlib.h>
struct Node;
struct List;
struct Node
{
	Node* next;
	char key;
};
struct List
{
	Node* head;
	int count;
};
List* list_new()
{
	List* a=(List*)malloc(sizeof(List));
	if (a==NULL) 
		return NULL;
	a->head=NULL;
	a->count=0;
	return a;
}	
void push(List* a, char key)
{
	Node* b=(Node*)malloc(sizeof(Node));
	if (b==NULL) 
		return;
	if ((a->head)==NULL)
	{
		b->next=NULL;
		a->head=b;
	}
	else
	{
		(b->next)=a->head;
		a->head=b;
	}
	a->count++;
	a->head->key=key;
	return;
}
void deleten(List* a)
{
	Node* qwe=a->head;
	a->head=a->head->next;
	a->count--;
	free(qwe);
	return;
}	
void list_delete(List* a)
{
	while (a->count!=0)
	{
		Node* b=(a->head);
		a->head=a->head->next;
		a->count--;
		free(b);
	}
	free(a);
}
void delete_string_array(char** b,int n)
{
	int i;
	for (i=0; i<n; i++)
	{
		free(b[i]);
	}
	free(b);
}
int main ()
{
	List* a;
	char c=1;
	int n;
	int i;
	int j;
	int l;
	int k=0;
	int t=0;
	int e=0;
	a = list_new();
	printf("Input number of skobki type\n");
	scanf("%d",&n);
	char** sk = (char**)malloc(n*sizeof(char*));
	for (j=0;j<n;j++)
	{
		sk[j] = (char*)malloc(3*sizeof(char));
		if (sk[j]==NULL)
			return -1;
	}
	if (sk==NULL)
		return -1;
	printf("Input type of open skobka and close skobka\n");
	for (j=0;j<n;j++)
	{
		scanf("%s\n",sk[j]);
	}
	while(c!='\n')
	{
		if(t==0)
		{
			scanf("%c",&c);
			for (l=0;l<n;l++)
			{
				for (k=0;k<2;k++)
				{
					if (c==sk[l][k])
					{
						t++;
						e++;
					}
				}
			}	
		}
		if (t>0)
		{
			j=0;
			while(j<n)
			{
				if (c==sk[j][0])
				{
					push(a,c);
					t=0;
				}
				if (c==sk[j][1])
				{
					if (a->count==0 &&  c==sk[j][1])
					{
						printf("no");
						delete_string_array(sk,n);
						list_delete(a);
						return 0;
					}
					else
					{
						if (c==sk[j][1] && (a->head->key==sk[j][0]))
							deleten(a);
						else
						{
							printf("no");
							list_delete(a);
							delete_string_array(sk,n);
							return 0;
						}
					}
				}
			j++;
			}
		}
		t=0;
	}
	if (e==0)
	{
		printf("Error");
	}
	else 
	{
		if (a->count==0)
			printf("yes");
		else
			printf("no");
		list_delete(a);
		delete_string_array(sk,n);
	}
	return 0;
}