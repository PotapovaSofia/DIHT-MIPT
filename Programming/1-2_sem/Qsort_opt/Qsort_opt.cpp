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
		printf("%d ", Arr[i]);
	}
	printf("\n");
}

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

void QuickSort ( void* Arr, int first, int last, size_t size, int(*cmp)(void*, void*))
{
   if ((last - first) > 10)
   {
        int i, j;
        i = first;
        j = last;
        int key = (rand() % last + first) % last;
        char *buf = (char*)malloc(size);
        memcpy(buf, (char*)Arr + key*size, size);

        do {

           while (cmp((char*)Arr + i*size, buf) < 0) i++;
           while (cmp((char*)Arr + j*size, buf) > 0) j--;

            if(i <= j) {
                if (i < j) swap((char*)Arr + i*size, (char*)Arr + j*size, size);
                i++;
                j--;
            }
        } while (i <= j);

        if (i < last)
            QuickSort(Arr, i, last, size, cmp);
        if (first < j)
            QuickSort(Arr, first,j, size, cmp);
    }
}

void InsertSort(void* a, int n, size_t size, int(*cmp)(void*, void*))
{
	int i;
	for (i = 1; i < n; ++i)
	{
		char* key = (char*)(malloc(size));
		memcpy(key, (char*)a + i* size, size);
		int j = i - 1;
		while ((j >= 0) && (cmp((char*)a + j * size, key) > 0))
		{
			swap((char*)a + j*size, (char*)a + (j + 1) * size, size);
			j--;
		}
		memcpy((char*)a + (j + 1) * size, key, size);
	}
}
int main()
{
	int i, n;
	printf("Number: ");
	scanf("%d", &n);
	printf ("\n");

	int* Arr = (int*)(malloc(sizeof(int)*n));

	generation(Arr, n);

	int(*p)(void*, void*);
	p = cmp_;

	QuickSort(Arr, 0, n - 1, sizeof(int), p);

	InsertSort(Arr, n, sizeof(int), p);

	for (i = 0; i < n; ++i)
	{
		printf("%d ", Arr[i]);
	}
	free(Arr);
	return 0;
}
