#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
int main()
{
	int i, j, n, k;
    scanf("%d",&n);
	scanf("%d",&k);
	char* a = (char*)malloc(n*sizeof(char));
	char* b = (char*)malloc(k*sizeof(char));
	int** l;
	l = (int**)calloc((n+1),sizeof(int*));
	for (int i=0;i<n;i++)
	{
		l[i] = (int*)calloc((k+1),sizeof(int));
	}
    scanf("%s", a);
    scanf("%s", b);
	for (i = 0; i <= n; i++)
	{

	    for (j = 0; j <= k; j++)

	    {
			if (a[i] == '\0' || b[j] == '\0')
			l[i][j] = 0;
			else 
				if (a[i] == b[j])
				l[i][j] = 1 + l[i-1][j-1];
				else 
				l[i][j] = MAX(l[i-1][j], l[i][j-1]);
	    }
    }
	for (i=0;i<n;i++)
	{
		for (j=0;j<k;j++)
		{
			printf("%d",l[i][j]);
		}
		printf("\n");
	}
    printf( "%d\n", l[n][k] );
	return 0;
}