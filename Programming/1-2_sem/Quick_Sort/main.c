#include <stdio.h>
#include <stdlib.h>

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


void swap (int* a, int* b)
{
    int buf;
    buf = *a;
    *a = *b;
    *b = buf;
}
void QuickSort (int* Arr, int first, int last)
{
   int i, j, x;
   i = first;
   j = last;
   x = Arr[(first + last) / 2];

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

int main()
{
    int n;
    printf("Number\n");
    scanf("%d", &n);

    int* Arr;
    Arr = (int*) malloc(n*sizeof(int));

    generation(Arr, n);

    QuickSort(Arr, 0, n-1);

    out_Console(Arr, n);



    return 0;
}
