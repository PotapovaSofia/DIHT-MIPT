#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generation(int* Arr, int n)
{
	int i;
	srand(time(NULL));
	for ( i=0;i<n;++i)
	{
		Arr[i] = rand()%100 - 50;
		//printf("%d ", Arr[i]);
	}
	//printf("\n");
}

int out_Console(int* Arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d ", Arr[i]);
    }
    return 0;
}

void swap (int* a, int* b)
{
    int buf;
    buf = *a;
    *a = *b;
    *b = buf;
}

void ShellSort(int* a, size_t n, size_t h)
{
    int i, j, t;
    for (i = h; i < n; ++i)
    {
        j = i;
        t = a[i];
        while (j >= h && t < a[j - h])
        {
            a[j] = a[j - h];
            j -= h;
        }
        a[j] = t;
    }
}

void Bubble_Sort (int* Arr, int n)
{
    char flag = 0;
    int i;
    while (flag == 0)
    {
        flag = 1;
        for (i = 0; i < n - 1; i++)
            if (Arr[i] > Arr[i+1])
            {
                flag = 0;
                swap (&Arr[i], &Arr[i+1]);
            }
    }
}



void Heapify(int* Arr, int i, int n)
{
    int left, right;
    left = 2*i+1;
    right = 2*i+2;

    int largest;
    largest = i;

    if ((left <= n-1) && (Arr[left]>Arr[i]))
        largest = left;

    if ((right <= n-1) && (Arr[right]>Arr[largest]))
        largest = right;
    if (largest != i)
    {
        swap(&Arr[i], &Arr[largest]);
        Heapify(Arr, largest, n);
    }
}

void Heap_Build (int*Arr, int n)
{
    int i;
    for(i = n/2 - 1; i >=0; i--)
        Heapify(Arr, i, n);
}

void HeapSort (int* Arr, int n)
{
    int i;
    i = n-1;
    while(i>=0)
    {
        //i = n-1;
        swap(&Arr[0], &Arr[i]);
        n--;
        Heapify(Arr, 0, n);

        i--;
    }
}

void QuickSort (int* Arr, int first, int last)
{
   int i, j, x;
   i = first;
   j = last;
   x = Arr[first];

    do {
        while (Arr[i] < x) i++;
        while (Arr[j] > x) j--;

        if(i <= j) {
            if (i < j) swap(&Arr[i], &Arr[j]);
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
        QuickSort(Arr, i, last);
    if (first < j)
        QuickSort(Arr, first,j);
}

void Merge (int* Arr, int left, int mid, int right)
{
    int i, j, k;
    i = left;
    j = mid +1;
    k = 0;


   int Brr[right-left + 1];

    while ((i <= mid) && (j <= right))
    {
        if (Arr[i]<Arr[j])
        {
            Brr[k++]=Arr[i++];
        }

        else
        {
            Brr[k++]=Arr[j++];
        }
    }

    while (i <= mid)
         {
            Brr[k++]=Arr[i++];
        }
    while (j <= right)
        {
            Brr[k++]=Arr[j++];
        }

    int p;
    for (p = 0; p < right-left + 1; p++)
    {
        int p1 = p + left;
        Arr[p1]=Brr[p];
    }
    free(Brr);
}

void MergeSort (int* Arr, int left, int right)
{
    if (left < right)
    {
        int mid;
        mid = (left + right)/2;
        MergeSort(Arr, left, mid);
        MergeSort(Arr, mid + 1, right);
        Merge(Arr, left, mid, right);
    }
}

void QuickSort_opt (int* Arr, int first, int last)
{
   if((last-first)>30){
    int i, j, x;
   i = first;
   j = last;
   int key = rand() % (last - first) + first;
   int key1 = Arr[key];

    do {
        while (Arr[i] < key1) i++;
        while (Arr[j] > key1) j--;

        if(i <= j) {
            if (i < j) swap(&Arr[i], &Arr[j]);
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
        QuickSort_opt(Arr, i, last);
    if (first < j)
        QuickSort_opt(Arr, first,j);
   }
}

void InsertSort(int*Arr, int left, int right)
{
    int i;
    for(i = right; i > left; i--)
        if (Arr[i]< Arr[i-1])
            swap (&Arr[i], &Arr[i-1]);
    for (i = 1; i <=right; i++)
    {
        int j = i;
        int v = Arr[i];
        while (v<Arr[j-1])
        {
            Arr[j]=Arr[j-1];
            j--;
        }
        Arr[j]=v;
    }

}

int* copy(int* arr, int arr_size) {
	int* b = (int*)malloc(sizeof(int)* arr_size);

	int i;
	for (i = 0; i < arr_size; ++i) {
		b[i] = arr[i];
	}

	return b;
}

int Sort(int* Arr, int n)
{
    int i;
    for (i = 1; i < n; i++)
        if (Arr[i]<Arr[i-1])
            return 0;
    return 1;
}

int main()
{
    int n;
    printf("Number: ");
    scanf("%d", &n);

    int* Arr;
    Arr = (int*)malloc(n*sizeof(int));
    //printf("Array:        ");
    generation (Arr, n);
    int* Brr = copy(Arr,n);
    int* Crr = copy(Arr,n);
    int* Drr = copy(Arr,n);
    int* Err = copy(Arr,n);
    int* Frr = copy(Arr,n);

    //Shell
    int i = 0;
    int* h = (int*)malloc(n*sizeof(int));
    h[i] = 1;
    while (h[i] <= n)
    {
        i++;
        h[i] = h[i - 1] * 2 + 1;
    }
    int j;
    clock_t t = clock();
    for (j = i - 1; j >= 0; --j)
         ShellSort(Arr, n, h[j]);
    t = clock() - t;
	printf("Time for ShellSort:        ");
	printf("%lf\n", ((double)t)/CLOCKS_PER_SEC);

	if(Sort(Arr, n))
        printf("Sorting works properly\n");
    else
        printf("ERROR\n");


    //Heap

    Heap_Build(Brr, n);

    clock_t t1 = clock();
    HeapSort(Brr, n);
    clock_t t2 = clock();
    t = t2 - t1;
	printf("Time for HeapSort:         ");
	printf("%lf\n", ((double)t)/CLOCKS_PER_SEC);

    if(Sort(Brr, n))
        printf("Sorting works properly\n");
    else
        printf("ERROR\n");


    //Qsort

    t = clock();
    QuickSort(Crr, 0, n-1);
    t = clock() - t;
    printf("Time for QuickSort:        ");
	printf("%lf\n", ((double)t)/CLOCKS_PER_SEC);

    if(Sort(Crr, n))
        printf("Sorting works properly\n");
    else
        printf("ERROR\n");


    //Qsort_opt

    t = clock();
    QuickSort_opt(Drr, 0, n-1);
    InsertSort(Drr, 0, n-1 );
    t = clock() - t;
    printf("Time for QuickSortOpt:     ");
	printf("%lf\n", ((double)t)/CLOCKS_PER_SEC);

    if(Sort(Drr, n))
        printf("Sorting works properly\n");
    else
        printf("ERROR\n");

    //Merge

    if(n<100000){
    t = clock();
    MergeSort(Err, 0, n-1);
    t = clock() - t;
    printf("Time for MergeSort:        ");
	printf("%lf\n", ((double)t)/CLOCKS_PER_SEC);

    if(Sort(Err, n))
        printf("Sorting works properly\n");
    else
        printf("ERROR\n");
    }

    //Bubble

    if (n<10000){
    t = clock();
    Bubble_Sort(Frr, n);
    t = clock() - t;
    printf("Time for BubbleSort:       ");
	printf("%lf\n", ((double)t)/CLOCKS_PER_SEC);

    if(Sort(Frr, n))
        printf("Sorting works properly\n");
    else
        printf("ERROR\n");
    }





    printf("\n\nQsort on bad values: \n");
    t = clock();
    QuickSort(Arr, 0, n-1);
    t = clock() - t;
   	printf("%lf\n", ((double)t)/CLOCKS_PER_SEC);

	printf("QsortOpt on bad values: \n");
	t = clock();
    QuickSort_opt(Arr, 0, n-1);
    InsertSort(Arr, 0, n-1 );
    t = clock() - t;
    printf("%lf\n", ((double)t)/CLOCKS_PER_SEC);




    free(Arr);
    free(Brr);
    free(Crr);
    free(Drr);
    free(Err);
    free(Frr);


    return 0;
}
