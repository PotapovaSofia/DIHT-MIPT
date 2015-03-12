#include <cstdio>
#include <time.h>
#include <cstdlib>
#include <conio.h>
#include <string.h>
int average(int a, int b, int c)
{
if ( ((a > b) && (a < c)) || ((a > c) && (a < b)) )
	return a;
if ( ((b > a) && (b < c)) || ((b > c) && (b < a)) )
	return b;
if ( ((c > b) && (c < a)) || ((c > a) && (c < b)) )
	return c;
return a; // if a = b = c
}

void qs(int* arr, int first, int last)
{
	int i,j,temp,p;
	i = first;
	j = last;
	while(j > first)
	{
		i = first;	
		j = last;
		p = average(arr[first], arr[ (first + last) / 2 ], arr[last]); 
		do 
		{ 
			while ( arr[i] < p ) 
				i++;
			while ( arr[j] > p ) 
				j--;

			if (i <= j) {
				temp = arr[i]; 
				arr[i++] = arr[j]; 
				arr[j--] = temp;
			}
		} while (i <= j); 

		if (i <  last)
			qs(arr, i, last);
		last = j;
	}
}

bool qsort(int n, const int* ar1, int* ar2) // check input data and write random array
{ 
	int i;
if (n > 0 && n <= 100) 
{  
	for (i = 0; i < n; ++i)
		ar2[i] = ar1[i];
	printf("Random array: ");
	for (i = 0; i < n; ++i) {
		printf("%d ", ar1[i]);
	}
	printf("\n");
	qs(ar2, 0, n-1); // we call QuickSort with n-1 + 1 = n elements

	return true;
} else
	return false;
}

int main() 
{
int i,n;
bool c;
	srand( time(NULL) );
	printf("Write the numbers of elements: ");
	scanf("%d", &n);
int* a=(int*)malloc(n*sizeof(int));
int* b=(int*)malloc(n*sizeof(int)); 
	for (i = 0; i < n; ++i){
		a[i] = rand()%100; // random array
	}

	c = qsort(n, a, b); // we start to sort

	if (c == true) {
		printf("Sorted array: ");
		for (i = 0; i < n; ++i)
			printf("%d ", b[i]);
	} else
		printf("Uncorrect n , write 0 < n < 101");
getch();
return 0;
} 