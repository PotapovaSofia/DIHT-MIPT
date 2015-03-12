#include <cstdio>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define max(a, b) ( (a > b) ? a : b )

int main()
{
	int i, j, n1, n2;
	 scanf("%d", &n1);
	 scanf("%d", &n2);
if (n1 > 0 && n2 > 0)
{
	char *a = (char*)malloc( n1 * sizeof(char) );
	char *b = (char*)malloc( n2 * sizeof(char) );
	int **t;
	t = (int**)malloc( (n1+1) * sizeof(int*) );
	for (i = 0; i <= n1; ++i)
		t[i] = (int*)malloc( (n2+1) * sizeof(int) );
	scanf("%s", a);
	scanf("%s", b);

	for(i = 0; i < n1; ++i)
		t[i][0] = 0;
	for(j = 0; j < n1; ++j)
		t[0][j] = 0;

	for (i = 1; i <= n1; ++i)
	    for (j = 1; j <= n2; ++j)
			if (a[i-1] == b[j-1])
				t[i][j] = 1 + t[i - 1][j - 1];
			 else 
				 t[i][j] = max( t[i - 1][j], t[i][j - 1] );

int length = t[n1][n2];

		for(i = 0; i < n1; ++i)
		{
			for(j = 0; j < n2; ++j)
				printf("%d ", t[i][j]);
			printf("\n");
		};

	i = n1;
    j = n2;
	int k = 0;
	char *c = (char*)malloc( length * sizeof(char) );
    while ( t[i][j] != 0 )
    {
		if ( t[i][j] > t[i-1][j] && t[i][j] > t[i][j-1])
		{
			--i; --j; --length;
			c[length] = a[i];
		} else
			if (t[i-1][j] >= t[i][j-1]) 
				--i;
			else --j;
	}
	printf("Length is %d\n", t[n1][n2]);
	for(i = 0; i < t[n1][n2]; ++i)
		printf("%c", c[i]);
}
	else
		printf("Unexpected length");
getch();
}
