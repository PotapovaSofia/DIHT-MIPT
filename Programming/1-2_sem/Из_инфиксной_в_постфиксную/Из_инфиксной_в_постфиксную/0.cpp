#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
/*struct Node;
struct Node1;
struct List;
struct List1;*/
struct Node
{
	Node* next;
	char key;
};
struct Node1
{
	Node1* next;
	int key;
};
struct List
{
	Node* head;
	int count;
};
struct List1
{
	Node1* head;
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
List1* list_new1()
{
	List1* a=(List1*)malloc(sizeof(List1));
	if (a==NULL)
		return NULL;
	a->head=NULL;
	a->count=0;
	return a;
}
void push(List* a, char key) ///вставка для char
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
void push1(List1* a, int key) /// вставка для int
{
	Node1* b=(Node1*)malloc(sizeof(Node1));
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
void deleten(List* a) ///удаление элемена для char;
{
	Node* qwe=a->head;
	a->head=a->head->next;
	a->count--;
	free(qwe);
	return;
}
void deleten1(List1* a) ///удаление элемента для int
{
	Node1* qwe=a->head;
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
/*int polish(int& i, char* pol,char* items)
{
	char c='a';
	List* a;
	if (pol==NULL)
		return -1;
	char first[3] = "*//*";
	char second[4] = "+-%";
	char third[2] = "^";
	a = list_new();
	while (c != '\n')
	{
		scanf("%c",&c);
		if (isdigit(c)) ///десятичное число
		{
			while((isdigit(c))||(c=='.'))
			{
				pol[i]=c;
				scanf("%c", &c);
				i++;
			}
			pol[i]=' ';
			i++;
		}
		else
		{
			char s[2];
			s[1]='\0';
			s[0]=c;
			/*if (isalpha(c) && strstr(items,s))  ///с - символ и  s является частью items
			{
				pol[i]=c;
				scanf("%c", &c);
				i++;
				pol[i]=' ';
				i++;
			}*/
		/*}/*
		switch(c)
		{
		case '(':
			push(a,c);
			break;
		case ')':
			if (a->count==0)
			{
				printf("Error");
				return -1;
			}
			while(a->head->key!='(')
			{
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
			while ((strstr(first,s)||strstr(second,s)||strstr(third,s))&& a->count!=0)
			{
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
			if (a->count==0)
			{
				push(a,c);
				break;
			}
			s[0]=a->head->key;
			while (strstr(first,s))
			{
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
	while (a->count!=0)
	{
		if (!(strstr(first,s)||strstr(second,s)||strstr(third,s)))
		{
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
}*/
int main ()
{
	char* pol=(char*)malloc(sizeof(char)*512);
	if (pol==NULL)
		return -1;
	printf("Write the string: ");
	//scanf("%s", pol);
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
		if (isdigit(c)) ///десятичное число
		{
			while((isdigit(c))||(c=='.'))
			{
				pol[i]=c;
				scanf("%c", &c);
				i++;
			}
			pol[i]=' ';
			i++;
		}
		else
		{
			char s[2];
			s[1]='\0';
			s[0]=c;
        }
		switch(c)
		{
		case '(':
			push(a,c);
			break;
		case ')':
			if (a->count==0)
			{
				printf("Error");
				return -1;
			}
			while(a->head->key!='(')
			{
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
			while ((strstr(first,s)||strstr(second,s)||strstr(third,s))&& a->count!=0)
			{
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
			if (a->count==0)
			{
				push(a,c);
				break;
			}
			s[0]=a->head->key;
			while (strstr(first,s))
			{
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
	while (a->count!=0)
	{
		if (!(strstr(first,s)||strstr(second,s)||strstr(third,s)))
		{
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





	/*int i=0;
	int t,p,m;
	int kol;
	char c;
	List1* a;
	a = list_new1();
	char* str=(char*)malloc(sizeof(char)*512);
	if (str==NULL)
		return -1;
	char* sub_str;
	sub_str = (char*)malloc(sizeof(char)*512);
	if (sub_str==NULL)
		return -1;
	char oper[7]="*//*+-^%";
	char* items = (char*)malloc(512*sizeof(char));
	if (items==NULL)
		return -1;
	printf("Input items\n");
	scanf("%s",items);
	int* value = (int*)malloc(strlen(items)*sizeof(int));
	if (value==NULL)
		return -1;
	int* buf = (int*)malloc(sizeof(int)*strlen(items));
	if (buf==NULL)
		return -1;
	printf("Input value of items\n");
	for (i=0;i<strlen(items);i++)
	{
		scanf("%d\n",&value[i]);
	}
	i=0;
	while (items[i]!='\0')
	{
		buf[items[i]]=value[i];
		i++;
	}
	i=0;
	if(polish(i,str,items)==-1)
	{
		free(str);
		free(sub_str);
		return 0;
	}
	else
	{
		sub_str=strtok(str, " ");
		while (sub_str!=NULL)
		{
			if (!((strstr(oper, sub_str)||strstr(items,sub_str))))
			{
				if (!(strstr(oper,sub_str)))
				{
					sscanf(sub_str,"%d",&t);
					push1(a,t);
				}
				else
				{
					sscanf(sub_str,"%c",&c);
					push1(a,buf[c]);
				}
			}
			else
			{
				switch (sub_str[0])
				{
					case '+':
						if ((a->count)<2)
						{
							printf("Error");
							return 0;
						}
						p = a->head->key;
						deleten1(a);
						m = a->head->key;
						deleten1(a);
						push1(a,(p+m));
						break;
					case '-':
						if ((a->count)<2)
						{
							printf("Error");
							return 0;
						}
						p = a->head->key;
						deleten1(a);
						m = a->head->key;
						deleten1(a);
						push1(a,(m-p));
						break;
					case '*':
						if ((a->count)<2)
						{
							printf("Error");
							return 0;
						}
						p = a->head->key;
						deleten1(a);
						m = a->head->key;
						deleten1(a);
						push1(a,(p*m));
						break;
					case '/':
						if ((a->count)<2)
						{
							printf("Error");
							return 0;
						}
						p = a->head->key;
						deleten1(a);
						m = a->head->key;
						deleten1(a);
						push1(a,(m/p));
						break;
					case '^':
						if ((a->count)<2)
						{
							printf("Error");
							return 0;
						}
						p = a->head->key;
						deleten1(a);
						m = a->head->key;
						deleten1(a);
						push1(a,pow((double)m,p));
						break;
					case '%':
						if ((a->count)<2)
						{
							printf("Error");
							return 0;
						}
						p = a->head->key;
						deleten1(a);
						m = a->head->key;
						deleten1(a);
						push1(a,(m^p));
						break;
				}
		}
		sub_str = strtok(NULL, " ");
	}
	printf("\n");
	printf("%d",a->head->key);
	free(sub_str);
	free(str);
	return 0;
	}*/
}
