#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    //printf("Number:");
    scanf("%d", &n);
    //printf("\n");

    int Sum;
    Sum = 0;
    int min;
    int max;
    min = 1000;
    max = 0;

    float q = 0.0;
    //q = q + min*min;

    int i = 0;
    while (i < n);
    {
        int f;
        scanf("%d", &f);
        Sum = Sum + f;
        /*if (min > f)
            min = f;
        if (max < f)
            max = f;
        q+=f*f;*/
        i++;
    }

    float Sr;
    Sr = (float)Sum/(float)n;

    q = sqrt((q+Sr*Sr*n - 2*Sum*Sr)/(n-1));

    printf("Summa: %f\n", Sum);
    printf("Srednee: %f\n", Sr);
    printf("Standartnoe otklonenie: %f\n", q);
    printf("Maximum: %d\n", max);
    printf("Minimum: %d\n", min);

    return 0;
}
