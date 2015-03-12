//#include <cstdio>
//#include <stdlib.h>
//#include <conio.h>
//
//void heapify (int *a, int k, int n)
//{
// int new_el = a[k];
// int child;
// while (k <= n/2)
// {	
//	 child = 2*k;
//	if ( (child < n) && (a[child] < a[child + 1]) )
//		++child;
//	if (new_el >= a[child])
//		break;
//	a[k] = a[child];
//	k = child;
// }
// a[k] = new_el;
//}
//void Build_heap (int* a, int n)
//{
//	int i;
//	for (i = n/2 - 1; i >= 0; --i)
//		heapify(a, i, n-1);
//	for (i = n - 1; i > 0; --i)
//	{
//		int t = a[i];
//		a[i] = a[0];
//		a[0] = t;
//		heapify(a, 0, i-1);
//	}
//}
//int main()
//{
//	int n;
//	printf("Write number of elements in your array: ");
//	scanf("%d", &n);
//	int *a = (int*)malloc(n*sizeof(int));
//	int i;
//	for (i = 0; i < n; ++i)
//		scanf("%d", &a[i]);
//	Build_heap (a, n);
//	for (i = 0; i < n; ++i)
//		printf("%d ", a[i]);
//	getch();
//	return 0;
//}