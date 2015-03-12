#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cmp_(void* a, void* b){
	return (*(int*)a - *(int*)b);
}

void swap(void* a, void* b, size_t size)
{
	char* x = (char*)(malloc(size));
	memcpy(x, a, size);
	memcpy(a, b, size);
	memcpy(b, x, size);
	free(x);
}

void Merge (void* Arr, int left, int mid, int right, size_t size, int(*cmp)(void*, void*))
{
    int i, j, k;
    i = left;
    j = mid +1;
    k = 0;


    void* Brr = malloc((right-left + 1)*size);

    while ((i <= mid) && (j <= right))
    {
        if (cmp((char*)Arr + i*size, (char*)Arr + j*size)<0)
        {
            memcpy((char*)Brr + k*size, (char*)Arr  + i*size, size);
            k++;
            i++;
        }

        else
        {
            memcpy((char*)Brr + k*size, (char*)Arr  + j*size, size);
            k++;
            j++;
        }
    }

    while (i <= mid)
         {
            memcpy((char*)Brr + k*size, (char*)Arr  + i*size, size);
            k++;
            i++;
        }
    while (j <= right)
        {
            memcpy((char*)Brr + k*size, (char*)Arr  + j*size, size);
            k++;
            j++;
        }

    int p;
    for (p = 0; p < right-left + 1; p++)
    {
        int p1 = p + left;
        memcpy((char*)Arr + p1*size, (char*)Brr + p*size, size);

    }
    free(Brr);
}

void MergeSort (void* Arr, int left, int right, size_t size, int(*cmp)(void*, void*))
{
    if (left < right)
    {
        int mid;
        mid = (left + right)/2;
        MergeSort(Arr, left, mid, size, cmp);
        MergeSort(Arr, mid + 1, right, size, cmp);
        Merge(Arr, left, mid, right, size, cmp);
    }
}

void generation(int* Arr, int n)
{
	int i;
	srand(time(NULL));
	for ( i=0;i<n;++i)
	{
		Arr[i] = rand()%100 - 50;
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


int main()
{
   	int i, n;
	printf("Number:");
	scanf("%d", &n);
	printf("\n");

	int* Arr = (int*)(malloc(sizeof(int)*n));

	generation(Arr, n);

	int(*ptr)(void*, void*);
	ptr = cmp_;

	MergeSort(Arr, 0, n - 1, sizeof(int), ptr);

	out_Console(Arr, n);

	free(Arr);
	return 0;
}
