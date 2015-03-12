#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

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

void push(List* a, char key) ///вставка дл€ char
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

void deleten(List* a) ///удаление элемена дл€ char;
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

int main ()
{
	char* pol=(char*)malloc(sizeof(char)*512);
	if (pol==NULL)
		return -1;
	printf("Write the string: ");

	List* a;
	char c='a';
	int i = 0;
	char first[3] = "*/";
	char second[4] = "+-%";
	char third[2] = "^";
	a = list_new();
	while (c != '\n')
	{
		scanf("%c",&c);
		if (isdigit(c)) ///дес€тичное число
		{
			while((isdigit(c))||(c=='.')){
				pol[i]=c;
				scanf("%c", &c);
				i++;
			}
			pol[i]=' ';
			i++;
		}
		else{
			char s[2];
			s[1]='\0';
			s[0]=c;
        }
		switch(c){
		case '(':
			push(a,c);
			break;
		case ')':
			if (a->count==0){
				printf("Error");
				return -1;
			}
			while(a->head->key!='('){
					pol[i]=a->head->key;
					i++;
					pol[i]=' ';
					i++;
					deleten(a);
					if (a->count==0)
					{
						printf("Error");
						return -1;
					}
			}
			deleten(a);
			break;
		case '+':
		case '-':
		case '%':
			if (a->count==0)
			{
				push(a,c);
				break;
			}
			char s[2];
			s[0]=a->head->key;
			s[1]='\0';
			while ((strstr(first,s)||strstr(second,s)||strstr(third,s))&& a->count!=0){
				pol[i]=a->head->key;
				deleten(a);
				i++;
				pol[i]=' ';
				i++;
				if (a->count!=0)
				s[0]=a->head->key;
			}
			push(a,c);
			break;
		case '*':
		case '/':
			if (a->count==0){
				push(a,c);
				break;
			}
			s[0]=a->head->key;
			while (strstr(first,s)){
				pol[i]=a->head->key;
				deleten(a);
				i++;
				pol[i]=' ';
				i++;
				s[0]=a->head->key;
			}
			push(a,c);
			break;
		case '^':
		push(a,c);
		break;
		default:
			break;
		}
	}
	char s[2];
	s[0]=a->head->key;
	s[1]='\0';
	while (a->count!=0){
		if (!(strstr(first,s)||strstr(second,s)||strstr(third,s))){
			printf("Error");
			return -1;
		}
		pol[i]=a->head->key;
		deleten(a);
		i++;
		pol[i]=' ';
		i++;
		if (a->count!=0)
		s[0]=a->head->key;
	}
	list_delete(a);
	pol[i]='\0';
	printf("%s",pol);
	return 0;

	free(pol);
}
