#include <cstdio>
#include <cstdlib>

void Quick_Sort(double* bArray, double* cArray, int r, int l = 0)
{
    int i = l-1;
    int j = r+1;
    int v = bArray[r];
    while(i <= j)
    {
        while(bArray[++i] < v)
        {
        }
        while(bArray[--j] > v)
        {
        }
        if (i <= j)
        {
            double a = bArray[i];
            bArray[i] = bArray[j];
            bArray[j] = a;
            a = cArray[i];
            cArray[i] = cArray[j];
            cArray[j] = a;
        }
    }
    if (j > l)
    {
        Quick_Sort(bArray, cArray, j, l);
    }
    if (i < r)
    {
        Quick_Sort(bArray, cArray, r, i);
    }
}

int main()
{
    int n = 0;
    scanf("%d", &n);
    double* s = reinterpret_cast<double*>(malloc(n*(sizeof(double))));
    double* f = reinterpret_cast<double*>(malloc(n*(sizeof(double))));


    int mx = 10000;
    for (int i = 0; i < n; i++)
    {
        scanf("%lf %lf", &(s[i]), &(f[i]));
        if (f[i] < 0)
        {
            s[i] = mx+1;
            f[i] = mx+2;
        }
    }

    Quick_Sort(s, f, n-1);

    int y = 0;
    int dy = y;
    int j = 0;
    int i = -1;
    int k = 0;
    while ((y < mx) && (i < n))
    {
        while ((s[++i] <= y) && (i < n))
        {
            if (f[i] > dy)
            {
                dy = f[i];
            }
        }
        if (dy > y)
        {
            j++;
            i--;
            y = dy;
        }
        else
        {
            printf("Error");
            free(s);
            free(f);
            return 0;
        }
    }
    if ((i >= n) && (y < mx))
    {
        printf("Error");
        free(s);
        free(f);
        return 0;
    }

    printf("%d", j);

    free(s);
    free(f);
    return 0;
}
