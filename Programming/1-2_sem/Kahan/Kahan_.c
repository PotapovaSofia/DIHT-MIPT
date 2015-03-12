#include <stdio.h>
#include <stdlib.h>
int Kahan_Sum(double* Arr, int n)
{
    double sum = 0.0;
    double c = 0.0;
    int i;
    double y, t;
    for (i=0; i<n; i++)
    {
        y = Arr[i] - c;
        t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    printf("Pravilnoe Summirovanie: %.20lf \n", sum);
    return 0;
}

int main()
{
    int n;
    printf("Number of elements:\n");
    scanf("%d", &n);
    double* Arr;
    Arr = (double*)malloc(n*sizeof(double));
    double K;
    double Sum = 0.0;
    int i;
    for (i = 0; i<n; i++)
    {
        scanf("%lf", &K);
        Arr[i]=K;
        Sum = Sum + K;

  }
    printf("Summirovanie v lob:     %.20lf\n", Sum);
    /*double sum = 0.0;
    double c = 0.0;
    int i;
    double y, t;
    for (i=0; i<n; i++)
    {
        y = Arr[i] - c;
        t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    printf("Pravilnoe Summirovanie: %.10lf \n", sum);*/
    Kahan_Sum(Arr, n);

    free (Arr);

    return 0;
}
