#include <stdio.h>


int main()
{
    int n;

    scanf("%d", &n);

    int t1 = 1;
    int t2 = 1;
    int t3 = 1;

    if (n == 0)
    {
        printf("0\n");
        return 0;
    }

    if (n <= 3)
    {
        printf("1\n");
        return 0;
    }

    int i = 4;
    int tI;
    while(i<=n)
    {
        tI = t1+t2+t3;
        t1 = t2;
        t2 = t3;
        t3 = tI;
        i++;
    }

    printf("%d\n", tI);

    return 0;
}
