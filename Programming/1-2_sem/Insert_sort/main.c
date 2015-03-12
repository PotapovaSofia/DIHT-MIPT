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
void swap (int* a, int* b)
{
    int buf;
    buf = *a;
    *a = *b;
    *b = buf;
}

int main()
{
    int n;
    printf("Number\n");
    scanf("%d", &n);

    int* Arr;
    Arr = (int*) malloc(n*sizeof(int));

    generation(Arr, n);

    InsertSort(Arr, 0, n-1);

    out_Console(Arr, n);



    return 0;
}
