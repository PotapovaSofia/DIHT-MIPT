#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void swap(int &a, int &b)
{
	int t = a;
	a = b;
	b = t;
}

void bubblesort(int *a, int n)								// bublesort
{
	int i,j,k;
		for (i=0; i<n-1; i++)
		for (j=0; j<n-i-1; j++) 
			if (a[j]>a[j+1]) {
				k=a[j];
				a[j]=a[j+1];
				a[j+1]=k;
			}
}

void qsort(int* arr,int first, int last){					// quicksort
int i,j,temp,p;
i = first; 
j = last;
p = arr[ (first + last)/2 ]; // central element
while (i <= j)
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
} 

 if (i < last) 
	 qsort(arr, i, last);

 if (first < j) qsort(arr, first, j);
}

void heapify (int *a, int k, int n)							// heapsort
{
 int new_el = a[k];
 int child;
 while (k <= n/2)
 {	
	 child = 2*k;
	if ( (child < n) && (a[child] < a[child + 1]) )
		++child;
	if (new_el >= a[child])
		break;
	else {
		a[k] = a[child];
		k = child;
	}
 }
 a[k] = new_el;
}
void Build_heap (int* a, int n)
{
	int i;
	for (i = n/2 - 1; i >= 0; --i)
		heapify(a, i, n-1);
}

int *merge(int *a, int *b, int l1, int l2)					// mergesort
{
    int *ab=(int*)malloc( (l1+l2)*sizeof(int) );
    int n = 0;
    while (l1 && l2)
    {
        if (*a < *b)
        {
            ab[n] = *a;
            a++;
            l1--;
        }
        else
        {
            ab[n] = *b;
            b++;
            l2--;
        }
        n++;
    }

    if (l1 == 0)
    {
        for (int i = 0; i < l2; ++i)
        {
            ab[n++] = *b;
			++b;
        }
    }

    else
    {
        for (int i = 0; i < l1; ++i)
        {
            ab[n++] = *a;
			++a;
        }
    }
    return ab;
}
void mergesort(int *a, int len) 
{
	int n = 1, l, ost;
    int *a1;
    while (n < len)
    {
        l = 0;
        while (l < len)
        {
            if (l + n >= len) 
				break;
            ost = (l+n*2 > len) ? (len - (l+n)) : n;
            a1 = merge(a + l, a +l + n, n, ost);
            for (int i = 0; i < n+ost; i++) 
				a[l+i] = a1[i];
            free(a1);
            l += n*2;
        }
        n *= 2;
    }
}

int increment(int inc[], int size) {						// shellsort
  int p1 = 1, 
	  p2 = 1, 
	  p3 = 1, 
	  s = -1;
  do {
    if (++s % 2) {
      inc[s] = 8*p1 - 6*p2 + 1;
    } else {
      inc[s] = 9*p1 - 9*p3 + 1;
      p2 *= 2;
      p3 *= 2;
    }
	p1 *= 2;
  } while(3*inc[s] < size);  

  return s > 0 ? --s : 0;
}
void shell(int *a, int size) {
  int inc, j, i, b[30], s;
  s = increment(b, size);
  while (s >= 0) {
	inc = b[s--];
    for (i = inc; i < size; i++) {
      int temp = a[i];
      for (j = i-inc; (j >= 0) && (a[j] > temp); j -= inc)
        a[j+inc] = a[j];
      a[j+inc] = temp;
    }
  }
}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int main()
{
	int n, i;
	printf("Write number of elements: ");
	scanf("%d", &n);
	int *a = (int*)malloc(n*sizeof(int));
	srand(500);
	for(i = 0; i < n; ++i){
		a[i] = rand();
	}
	//for (i = 0; i < n; ++i) 
	//	printf("%d ",a[i]);
	printf("\n");
	int *b = (int*)malloc(n*sizeof(int));

		for (i = 0; i < n; ++i) 
			b[i] = a[i];
	clock_t t1, t2;
	t1 = clock();					// bubblesort
	bubblesort(b, n);
	t2 = clock();
	//for (i = 0; i < n; ++i) 
	//	printf("%d ",b[i]);
	printf("BubbleSort = %d clicks \n", t2 - t1);

		for (i = 0; i < n; ++i) 
			b[i] = a[i];
	t1 = clock();					// quicksort
	qsort(b, 0, n-1);
	t2 = clock();
	printf("QuickSort = %d clicks \n", t2 - t1);

		for (i = 0; i < n; ++i) 
			b[i] = a[i];
	t1 = clock();					// heapsort
	Build_heap (b, n);
	for(i = n-1; i > 0; --i)
	{
		swap(b[i], b[0]);
		heapify(b, 0 ,i-1);
	}
	t2 = clock();
	printf("HeapSort = %d clicks \n", t2 - t1);

		for (i = 0; i < n; ++i) 
			b[i] = a[i];
	t1 = clock();					// mergesort
	mergesort(b, n);
	t2 = clock();
	printf("MergeSort = %d clicks \n", t2 - t1);

		for (i = 0; i < n; ++i) 
			b[i] = a[i];
	t1 = clock();					// shellsort
	shell(b, n);
	t2 = clock();
	printf("ShellSort = %d clicks \n", t2 - t1);

		for (i = 0; i < n; ++i) 
			b[i] = a[i];
	t1 = clock();					// standart qsort
	qsort (b, n, sizeof(int), compare);
	t2 = clock();
	printf("QuickSortStandart = %d clicks \n", t2 - t1);

	getch();
	return 0;
}