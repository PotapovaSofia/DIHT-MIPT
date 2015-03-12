#include <stdio.h>
#include <stdlib.h>
void ShellSort(int* a, size_t n, size_t h)
{
    int i, j, t;
    for (i = h; i < n; ++i)
    {
        j = i;
        t = a[i];
        while (j >= h && t < a[j - h])
        {
            a[j] = a[j - h];
            j -= h;
        }
        a[j] = t;
    }
}

int main()
{
    int n;
    scanf("%d", &n);
    int* a = (int*)(malloc(sizeof(int) * n));
    int* h = (int*)(malloc(sizeof(int) * n));
    int i;
    for (i = 0; i < n; ++i)
    {
        scanf("%d", &a[i]);
    }
    i = 0;
    h[i] = 1;
    while (h[i] <= n)
    {
        i++;
        h[i] = h[i - 1] * 2 + 1;
    }
    int j;
    for (j = i - 1; j >= 0; --j)
    {
        printf("%d ", h[j]);
        ShellSort(a, n, h[j]);
    }
    printf("\n");
    for (i = 0; i < n; ++i)
    {
        printf("%d ", a[i]);
    }
    free(a);
    return 0;
}
