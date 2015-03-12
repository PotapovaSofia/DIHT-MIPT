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
   int i, j, key;
   i = first;
   j = last;
   key = first;
   char *buf = (char*)malloc(size);
   memcpy(buf, (char*)Arr + key*size, size);

   //int key = (rand() % last + first) % last;

    do {

        while (cmp((char*)Arr + i*size, buf) < 0) i++;
        while (cmp((char*)Arr + j*size, buf) > 0) j--;

        if(i <= j) {
            if (i < j) swap((char*)Arr + i*size, (char*)Arr + j*size, size);
            i++;
            j--;
        }
    } while (i <= j);

    free(buf);
    if (i < last)
        QuickSort(Arr, i, last, size, cmp);
    if (first < j)
        QuickSort(Arr, first,j, size, cmp);
}


/*int Partition(void* a, int l, int r, size_t size, int(*cmp)(void*, void*))
{
	int key = (rand() % r + l) % r;
	int i = l - 1;
	int j;



	for (j = l; j < r; ++j)
	{
		if (cmp((char*)a + j * size, (char*)a + key * size) < 0)
		{
			++i;
			swap((char*)a + i * size, (char*)a + j * size, size);
		}
	}
	swap((char*)a + (i + 1) * size, (char*)a + key * size, size);
	return i + 1;
}
void Qsort(void* a, int l, int r, size_t size, int(*cmp)(void*, void*))
{
	if (l < r)
	{
		int q = Partition(a, l, r, size, cmp);
		Qsort(a, l, q - 1, size, cmp);
		Qsort(a, q + 1, r, size, cmp);
	}
}*/

int main()
{
	srand(time(NULL));
	int i, n;
	printf("Number:");
	scanf("%d", &n);
	printf("\n");

	int* Arr = (int*)(malloc(sizeof(int)*n));
	/*for (i = 0; i < n; ++i)
	{
		scanf("%d", &Arr[i]);
	}*/
	generation(Arr, n);

	int(*p)(void*, void*);
	p = cmp_;

	QuickSort(Arr, 0, n - 1, sizeof(int), p);

	for (i = 0; i < n; ++i)
	{
		printf("%d ", Arr[i]);
	}

	free(Arr);
	return 0;
}
