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

void ShellSort(void* Arr, size_t n, size_t h, size_t size, int(*cmp)(const void*, const void*))
{
	int i, j;
	char* t = (char*)(malloc(size));
	for (i = h; i < n; ++i)
	{
		j = i;
		memcpy(t, (char*)Arr + i * size, size);
		while (j >= h && cmp(t, (char*)Arr + (j - h) * size) < 0)
		{
			memcpy((char*)Arr + j * size, (char*)Arr + (j - h) * size, size);
			j -= h;
		}
		memcpy((char*)Arr + j * size, t, size);
	}
}

int main()
{
    int i, n;
	printf("Number:");
	scanf("%d", &n);
	printf("\n");

	int* Arr = (int*)(malloc(sizeof(int)*n));
	int* h = (int*)(malloc(sizeof(int)* n));

	generation(Arr, n);

	i = 0;
	h[i] = 1;
	while (h[i] <= n)
	{
		i++;
		h[i] = h[i - 1] * 2 + 1;
	}

    int(*p)(void*, void*);
    p = cmp_;

	int j;
	for (j = i - 1; j >= 0; --j)
	{
		ShellSort(Arr, n, h[j], sizeof(int), p);
	}


	//ShellSort(Arr, 0, n - 1, sizeof(int), p);

	for (i = 0; i < n; ++i)
	{
		printf("%d ", Arr[i]);
	}

	free(Arr);
	return 0;
}

