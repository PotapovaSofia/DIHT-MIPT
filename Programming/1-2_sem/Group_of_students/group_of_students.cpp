#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct Student
{
	int number;
	char name[32];
	int mark;
};

void swap(Student &a,Student &b)
{
	Student x = a;
	a = b;
	b = x;
}

 void CountingSort (Student *a, int n, int *b, int k)
 {
	Student *c = (Student*)malloc(n*sizeof(Student));
	int i, j;
	for (i = 2; i <= k; ++i)
		 b[i] = 0;
	for (j = 0; j < n; ++j)
		 ++b[a[j].mark];
	int *d = (int*)malloc(4*sizeof(int));
	for(i = 2; i < k; ++i)
		d[i] = b[i];
	d[3] = b[2];
	d[2] = 0;
	for (i = 4; i <= k; ++i)
		 d[i] = d[i-1] + b[i-1];
	for (j = 0; j < n; ++j)
	{
		c[ d[a[j].mark] ] = a[j];
		++d[a[j].mark];
	}
	for (i = 0; i < n; ++i)
		a[i] = c[i];
 }

int main()
{
	int n;
	printf("Number of students: ");
	scanf("%d", &n);
	Student *s = (Student*)malloc(n*sizeof(Student));
	for (int i = 0; i < n; ++i)
	{
		scanf("%i",&s[i].number);
		scanf("%s",&s[i].name);
		scanf("%i",&s[i].mark);
	}
	int *a = (int*)malloc(4*sizeof(int));
    CountingSort(s,n,a,5);
	for (int i = 0; i < n; ++i)
	{
		printf("%d ",s[i].number);
		printf("%s ",s[i].name);
		printf("%i ",s[i].mark);
		printf("\n");
	}
	getch();
	return 0;
}