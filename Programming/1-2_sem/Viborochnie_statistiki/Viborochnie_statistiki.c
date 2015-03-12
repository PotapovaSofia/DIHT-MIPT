#include <stdio.h>
#include <stdlib.h>

/*int minimum(int* Arr, int n)
{
    int min;
    min = Arr[0];
    int i;
    for (i = 1; i < n;i++)
    {
        if (Arr[i] < min)
            min = Arr[i];
    }
    //printf("%d\n", min);
    return min;
}

int maximum(int* Arr, int n)
{
    int max;
    max = Arr[0];
    int i;
    for (i = 1; i < n;i++)
    {
        if (Arr[i] > max)
            max = Arr[i];
    }
    //printf("%d\n", min);
    return max;
}*/

void swap (int* a, int* b)
{
    int buf;
    buf = *a;
    *a = *b;
    *b = buf;
}

void Bubble_sort(int* Arr, int n)
{
    char flag = 0;
    int i;
    while (flag == 0)
    {
        flag = 1;
        for (i = 0; i < n - 1; i++)
            if (Arr[i] > Arr[i+1])
            {
                flag = 0;
                swap (&Arr[i], &Arr[i+1]);
            }
    }
}

int mediana(int* Arr, int n)
{
    int med;
    med = (n-1)/2;
    return Arr[med];
}

float srednee(int* Arr, int n)
{
    int i;
    float Sum = 0.0;
    for (i = 0; i < n; i++)
    {
        Sum+=Arr[i];
    }
    float Sr;
    Sr = Sum/(float)n;
    return Sr;

}

float dispertion(int* Arr, int n)
{
    float Sr;
    Sr = srednee(Arr, n);

    float SumK;
    SumK = 0.0;

    int i;
    for (i = 0; i < n; i++)
    {
        SumK+= ((float)Arr[i] - Sr)*((float)Arr[i] - Sr);
    }
    float disp;
    disp = SumK/(float)n;

    //printf("%f\n", SumK);
    return disp;
}

int main()
{
    int n;
    printf("The number of integers: ");
    scanf("%d", &n);

    int* Arr;
    Arr = (int*)malloc(n*sizeof(int));

    int i;
    for (i = 0; i < n; i++)
        scanf("%d", &Arr[i]);


    Bubble_sort(Arr, n);

    for (i = 0; i < n; i++)
        printf("%d ", Arr[i]);

    int med;
    med = mediana(Arr, n);

    float disp;
    disp = dispertion(Arr, n);


    printf ("\n");
    printf ("Srednee:                ");
    printf("%.3f\n", srednee(Arr, n));

    printf ("Minimalnoe:             ");
    printf("%d\n", Arr[0]);

    printf ("Dispertion:             ");
    printf ("%f\n", disp);


    printf ("Standartnoe otklonenie: ");
    printf ("%f\n", sqrt(disp));

    printf ("Otklonenie maximalnogo\n ot mediani:            ");
    printf ("%d\n", Arr[n-1] - med);

    printf ("Otklonenie minimalnogo\n ot mediani:            ");
    printf ("%d\n", med - Arr[0]);

    //printf ("Mediana: %d", mediana(Arr, n));


    free(Arr);

    return 0;
}
