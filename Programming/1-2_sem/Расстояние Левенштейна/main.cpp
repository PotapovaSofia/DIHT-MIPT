#include <cstdio>
#include <cstdlib>
#include <string.h>

int min (int a,int b,int c){
    int mi = a;
	if (a > b)  mi = b;
	if ((c < b) && (c < a)) mi = c;
	return mi;
}
int majority(int a, int b)
{
	if (a == b) return 0;
	return 1;
}

int main()
{
    int n, m, i, j;

    char a[1000], b[1000];

    scanf("%s", a);
    scanf("%s", b);

    n = strlen(a);
    m = strlen(b);

    /*int k=0;
    if (m>n){
        k= m-n;
        m=n;
    }
    if (m<n){
        k=n-m;
        n=m;
    }*/

    int** l = (int**)malloc((n+1)*sizeof(int*));
    	for (int i=0;i<n+1;i++){
		l[i] = (int*)malloc((m+1)*sizeof(int));
	}

	/*l[n][m] = 0;
	for (i = 0; i<n; i++)
        l[i][m]= n-i;
    for (j = 0; j<n; j++)
        l[n][j] = m-j;

	for (i = n-1; i >=0; i--)
	    for (j = m-1; j >=0; j--){
            if (a[i]==b[j])
                l[i][j] = l[i+1][j+1];
            else
                l[i][j] = min(l[i][j+1]+1, l[i+1][j]+1, l[i+1][j+1]+1);
	    }
	    */

    for (int i = 0; i < n-1; ++i)
		for (int j = 0; j < m-1;++j)
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
                l[i][j] = min(l[i][j - 1] + 1, l[i - 1][j] + 1, l[i - 1][j - 1] + majority(a[i-1], b[j-1]));
		}

     printf("%d", l[n][m]);

     for (int i = n-1; i < n-1; ++i)
	{
		free(l[i]);
	}
	free(l);
    return 0;
}
