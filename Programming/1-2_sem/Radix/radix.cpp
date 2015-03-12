#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

 void radix (int *a, int n, int range)
 {
	int *count1 = (int*)malloc(range*sizeof(int));
	int i,t;
	for (i = 0; i < range; ++i)
		 count1[i] = 0;
	for (i = 0; i < n; ++i)
		 ++count1[a[i]];
	int *count2 = (int*)malloc(range*sizeof(int));
	for(i = 0; i < range; ++i)
		count2[i] = count1[i];
	count2[1] = count1[0];
	count2[0] = 0;
	for (i = 2; i < range; ++i)
		 count2[i] = count2[i-1] + count1[i-1];
	free(count1);
	int *c = (int*)malloc(n*sizeof(int));
	for (i = 0; i < n; ++i)
	{
		t = a[i];
		c[ count2[t] ] = t;
		++count2[t];
	}
	for (i = 0; i < n; ++i)
		a[i] = c[i];
 }

int main()
{
	int n;
	printf("Number of elements: ");
	scanf("%d", &n);
	int *a = (int*)malloc(n*sizeof(int));
	for (int i = 0; i < n; ++i)
		scanf("%i", &a[i]);
    	radix(a, n, 10);
	for (int i = 0; i < n; ++i)
		printf("%d ",a[i]);
	getch();
	return 0;
}