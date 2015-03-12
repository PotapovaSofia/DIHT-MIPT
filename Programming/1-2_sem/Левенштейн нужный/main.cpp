#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int min(int a, int b, int c)
{
	int mi = a;
	if (a > b)  mi = b;
	if ((c < b) && (c < a)) mi = c;
	return mi;
}

int mistic(int a, int b)
{
	if (a == b) return 0;
	return 1;
}

int main()
{
	int n, m;

	char a[1000], b[1000];

	gets(a);
	n = strlen(a)+1;

	gets(b);
	m = strlen(b)+1;


	int** l = (int**)malloc(n*sizeof(int*));
	for (int i = 0; i < n; ++i){
		l[i] = (int*)malloc(m*sizeof(int));
	}
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m;++j)
		{
			if ((i == 0) && (j == 0))
			{
				l[i][j] = 0;
				continue;
			}
			if ((i > 0) && (j == 0))
			{
				l[i][j] = i;
				continue;
			}
			if ((i == 0) && (j > 0))
			{
				l[i][j] = j;
				continue;
			}
			if ((i>0) && (j>0))
                l[i][j] = min(l[i][j - 1] + 1, l[i - 1][j] + 1, l[i - 1][j - 1] + mistic(a[i-1], b[j-1]));
		}
	printf("%d", l[n - 1][m - 1]);
	for (int i = n; i < n; ++i)
	{
		free(l[i]);
	}
	free(l);
	return 0;
}
