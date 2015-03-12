#include <stdlib.h>
#include <stdio.h>

void swap (int* a, int* b)
{
    int buf;
    buf = *a;
    *a = *b;
    *b = buf;
}
void QuickSort (int* Arr, int* Brr, int first, int last)
{
   int i, j, x;
   i = first;
   j = last;
   x = Arr[(first + last) / 2];

    do {
        while (Arr[i] < x) i++;
        while (Arr[j] > x) j--;

        if(i <= j) {
            if (i < j) {
                    swap(&Arr[i], &Arr[j]);
                    swap(&Brr[i], &Brr[j]);
            }
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
        QuickSort(Arr, Brr, i, last);
    if (first < j)
        QuickSort(Arr, Brr, first,j);
}

int main()
{
    int n;
    printf("Write the number: ");
    scanf("%d", &n);

    int* S = (int*)malloc(n*sizeof(int));
    int* F = (int*)malloc(n*sizeof(int));
    int* A = (int*)malloc(n*sizeof(int)); //массив, запоминающий подходящие отрезки


    int i;
    for(i = 0; i<n; i++){
        scanf("%d", &S[i]);
        scanf("%d", &F[i]);
        printf("\n");
    }

    QuickSort(F, S, 0, n-1);


    A[0] = 0;
    int k = 1;
    int j = 0;
    for(i = 1; i<n; i++)
        if (S[i]>=F[j]){
            A[k++] = i;
            j = i;
        }
    for(i = 0; i<k; i++){
        printf("%d - ", S[A[i]]);
        printf("%d\n", F[A[i]]);
    }

    free(A);
    free(S);
    free(F);

    return 0;
}
