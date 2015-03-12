#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Student
{
	int number;
	char surname[28];
	char name[5];
	int mark;
};
void swap(Student& a,Student& b)
{
	Student x;
	x = a;
	a = b;
	b = x;
}
 void CountingSort (Student* a,int n,int* b,int k)
 {
	 Student* c;
	 c = (Student*)malloc(n*sizeof(Student));
	 int i,j;
	 for (i=2;i<=k;i++)
	 {
		 b[i] = 0;
	 }
	 for (j=0;j<n;j++)
	 {
		 b[a[j].mark]++;
	 }
	 for (i=3;i<=k;i++)
	 {
		 b[i] = b[i] + b[i-1];
	 }
	for (j = n - 1;j>=0;j--)
	{
		c[b[a[j].mark]-1] = a[j];
		b[a[j].mark]--;
	}
	for (i=0;i<n;i++)
	{
		a[i] = c[i];
	}
 }
int main()
{
	int n,m;
	printf("Number of students - ");
	scanf("%d/n",&n);
	Student* s=(Student*) malloc(n*sizeof(Student));
	for (int i=0; i<n; i++)
	{
		scanf("%i",&s[i].number);
		scanf("%s",&s[i].surname);
		scanf("%s",&s[i].name);
		scanf("%i",&s[i].mark);
	}
	int* a = (int*) malloc(4*sizeof(int));
    CountingSort(s,n,a,5);
	for (int i=0;i<n;++i)
	{
		printf("%d ",s[i].number);
		printf("%s ",s[i].surname);
		printf("%s ",s[i].name);
		printf("%i ",s[i].mark);
		printf("\n");
	}
	return 0;
}