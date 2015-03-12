#include <cstdio>
#include <cstdlib>
#include <conio.h>

int *merge(int *a, int *b, int l1, int l2)
{
    int *ab=(int*)malloc( (l1+l2)*sizeof(int) );
    int n = 0;
    while (l1 && l2)
    {
        if (*a < *b)
        {
            ab[n] = *a;
            a++;
            l1--;
        }
        else
        {
            ab[n] = *b;
            b++;
            l2--;
        }
        n++;
    }

    if (l1 == 0)
    {
        for (int i = 0; i < l2; ++i)
        {
            ab[n++] = *b;
			++b;
        }
    }

    else
    {
        for (int i = 0; i < l1; ++i)
        {
            ab[n++] = *a;
			++a;
        }
    }
    return ab;
}

void mergesort(int *a, int len) 
{
	int n = 1, l, ost;
    int *a1;
    while (n < len)
    {
        l = 0;
        while (l < len)
        {
            if (l + n >= len) 
				break;
            ost = (l+n*2 > len) ? (len - (l+n)) : n;
            a1 = merge(a + l, a +l + n, n, ost);
            for (int i = 0; i < n+ost; i++) 
				a[l+i] = a1[i];
            free(a1);
            l += n*2;
        }
        n *= 2;
    }
}

int main() 
{
	int n;
	printf("Write the numbers of elements: ");
	scanf("%d", &n);
	if ((n > 0) && (n < 100))
	{
		int* a=(int*)malloc(n*sizeof(int)); 
		for (int i = 0; i < n; ++i)
			scanf("%d", &a[i]);

		mergesort(a, n);

		for (int i = 0; i < n; ++i)
			printf("%d ", a[i]);
	} else
		printf("Uncorrect n , write 0 < n < 101");
getch();
return 0;
} 