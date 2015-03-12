#include <stdio.h>
#include <stdlib.h>

void generation(int* Arr, int n)
{
	int i;
	srand(time(NULL));
	for ( i=0;i<n;++i)
	{
		Arr[i] = rand()%100;
		printf("%d ", Arr[i]);
	}
	printf("\n");
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
void in_console (int* Arr, int n)
{
    int i;

    for (i = 0;i < n; i++)
    {
        scanf("%d", &Arr[i]);
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

int main()
{
    int n;
    printf("Number: ");
    scanf("%d", &n);

    int* Arr;
    Arr = (int*)malloc(n*sizeof(int));

    printf("Array:        ");
    generation (Arr, n);

    Heap_Build(Arr, n);

    printf("Binary heap:  ");
    out_Console(Arr, n);
    printf("\n");

    free(Arr);

    return 0;
}
