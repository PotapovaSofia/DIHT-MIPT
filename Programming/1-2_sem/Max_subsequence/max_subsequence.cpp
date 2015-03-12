#include <cstdio>
#include <stdlib.h>
#include <conio.h>

void printroute(int *arr, int *l, int *h, int max)
{
	if ( l[max] > 1 )
	{
		printroute(arr, l, h, h[max]);
		printf("%d ", arr[max]);
	} else
		if ( l[max] == 1)
			printf("%d ", arr[max]);
}

int main()
{
	int i, n, j;
	scanf("%d",&n);
	int *arr = (int*)malloc( n * sizeof(int) );
	int *l = (int*)malloc( n * sizeof(int) );
	int *h = (int*)malloc( n * sizeof(int) );
	for (i = 0; i < n; ++i)
		scanf("%d", &arr[i]);
	l[0] = 1;
	h[0] = 0;
	for (j = 1; j < n; ++j)
	{
		l[j] = 1;
		h[j] = j;
		for (i = 0; i < j; ++i)
			if (l[i] + 1 > l[j] && arr[i] < arr[j])
			{
				l[j] = l[i] +1;
				h[j] = i;
			}
	}
	int max = 0;
	for (i = 0; i < n; ++i)
		if (l[i] > l[max])
			max = i;
	printroute(arr, l, h, max);
	getch();
	return 0;
}
