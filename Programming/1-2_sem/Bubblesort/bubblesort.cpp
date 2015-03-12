#include <cstdio>
#include <conio.h>
#include <stdlib.h>

typedef int myint;

void random_ar(int *a, int n)
{
	srand(500);
	for(int i = 0; i < n; ++i){
		a[i] = rand();
	}
}

void standart_ar(int *a, int n)
{
	for(int i = 0; i < n; ++i)
		scanf("%d", &a[i]);
}

void file_ar(int *a, int n)
{
	FILE *f=fopen("input.txt","r");
	for(int i = 0; i < n; ++i)
		scanf("%d", &a[i]);
}
void standart_out(int *a, int n)
{
	for (int i = 0; i < n; ++i)
		printf("%d ",a[i]);
}

void file_out(int *a, int n)
{
	FILE *f=fopen("output.txt","w");
	for (int i = 0; i < n; ++i)
		fprintf(f,"%d ",a[i]);
}

bool compare (void * a, void * b)
{
  return a > b;
}

void sort(void *a, int n, bool (*cmp)(void *, void *))						// bublesort
{
	myint *b = (myint *)a;
	int i, j, k;
		for (i = 0; i < n-1; ++i)
		for (j = 0; j < n-i-1; ++j){
			if ( cmp( &b[j], &b[j + 1]) ) {
				myint k = b[j];
				b[j] = b[j + 1];
				b[j + 1] = k;
			}
		}
}


int main()
{
	int choose;
	int n;
	printf("How you want to create array? Write: \n1 - random \n2 - standart \n3 - from file\n");
	 scanf("%d", &choose);
	printf("Numbers of elements in array: ");		//file have only elements of array
	 scanf("%d", &n);

if (n>0 && n<10000)									//check
{

	int *a=(int *)malloc(n*sizeof(int));
	switch (choose)													// in
	{
		case 1:
			 random_ar(a,n);
			break;

		case 2:
			 standart_ar(a,n);
			break;

		case 3:
			 file_ar(a,n);
			break;

		default:
			printf("Error");
	}

	sort(a, n, compare);

	printf("Choose the way of output: \n1-standart \n2-file\n");  // out
	scanf("%d", &choose);
	switch (choose)
	{
		case 1:
			standart_out(a,n);
			break;

		case 2:
			file_out(a,n);
			break;

		default:
			printf("Error");
	}

}
	else
		printf("Unexpected number");
getch();
}
