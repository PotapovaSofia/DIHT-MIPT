#include <stdio.h>
#include <math.h>

int F_prime(int Number)
{
    int flag = 1;
    int i = 2;
    double P = sqrt((double) Number);

    while (i <= P)
    {
        if (Number % i == 0)
            flag = 0;
        i++;
    }
    return flag;
};
int main()
{
    int n;
    scanf("%d", &n);

    if (F_prime(n) == 1)
        printf("The number is prime\n");
    else
        printf("The number is not prime\n");

    return 0;
}
