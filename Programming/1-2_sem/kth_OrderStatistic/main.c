#include <stdio.h>
#include <stdlib.h>


void swap (int* a, int* b)
{
    int buf;
    buf = *a;
    *a = *b;
    *b = buf;
};

int kth(int* Arr, int l,int r,int k)
{
    int x = Arr[(l+r) / 2];
    int i=l,j=r;
    while(i<=j)
    {
        while(Arr[i] < x) i++;
        while(Arr[j] > x) j--;

        if(i<=j)
        {
            swap(&Arr[i],&Arr[j]);
            i++;
            j--;
        }
    }
   if(l<=k && k<=j)
        return kth(Arr, l,j,k);
    if( i<=k && k<=r)
        return kth(Arr, i,r,k);
    printf("%d", Arr[k-1]);
    return Arr[k-1];
};

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

void in_console (int* Arr, int n)
{
    int i;

    for (i = 0;i < n; i++)
    {
        scanf("%d", &Arr[i]);
    }
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
    int n;
    printf("Number:\n");
    scanf("%d", &n);
    printf("\n");

    int* Arr;
    Arr = (int*) malloc(n*sizeof(int));

    generation(Arr, n);

    int k;
    printf("Write k: ");
    scanf("%d", &k);
    printf("\n");

    kth(Arr, 0, n-1, k);






    return 0;
}
