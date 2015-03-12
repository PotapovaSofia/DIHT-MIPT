#include <stdio.h>
#include <stdlib.h>

int Rec_(int n)
{
    if (n <= 3)
        return 1;
    else
    {
        return (Rec_(n - 1) + Rec_(n - 2) + Rec_(n- 3));
    }
}

int main()
{
    int N;
    int Trib;
    scanf("%d",&N);
    Trib=Rec_(N);
    printf("%d", Trib);
    return 0;
}
