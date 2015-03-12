#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max(a,b) ((a>b) ? a : b) 
int main()
{
    int i, j, n, k;
    scanf("%d",&n);
	scanf("%d",&k);
	char* a = (char*)malloc(n*sizeof(char));
	char* b = (char*)malloc(k*sizeof(char));
	int** l;
	l = (int**)calloc((n+1),sizeof(int*));
	for (int i=0;i<=n;i++)
	{
		l[i] = (int*)calloc((k+1),sizeof(int));
	}
    scanf("%s", a);
    scanf("%s", b);
    for( j = 0; j <=k ; j++ )
    {
       l[0][j] = 0;
    }
    for(i = 0; i <=n; i++)
    {
       l[i][0] = 0;
    }  
    for(i = 1; i <= n; i++)
    {
       for(j = 1; j <= k ; j++)
       {
          if( a[i-1] == b[j-1] )
             l[i][j] = l[i-1][j-1] + 1;
          else 
             l[i][j] = max( l[i-1][j], l[i][j-1] );
       }
    }
	for (i=0;i<=n;i++)
	{
		for (j=0;j<=k;j++)
		{
			printf("%d",l[i][j]);
		}
		printf("\n");
	}
	printf( "%d\n", l[n][k] );
	int cnt = l[n][k];
    char* result = (char*)calloc((cnt+1),sizeof(char));
	if (cnt==NULL)
		return -1;
	result[cnt] = '\0';
	i = n;
	j = k;
	while (l[i][j] != 0)
		{
			if (l[i][j] > l[i - 1][j] && l[i][j] > l[i][j - 1])
				{
					i--;
					j--;
					cnt--;
					result[cnt] = a[i];
				}
			else  if (l[i - 1][j] >= l[i][j - 1])
					i--;
						else j--;
		}
	printf ("%s",result);
	for (i=0;i<n;i++)
	free(l[i]);
	free(l);
	free(result);
    return 0;
}