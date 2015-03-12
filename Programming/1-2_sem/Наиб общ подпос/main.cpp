#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#define MAX(a, b) ((a) > (b) ? (a) : (b))


int main()
{
    int n, m, i, j;
    /*printf("First length: ");
    scanf("%d",&n);
    printf("\nSecond length: ");
	scanf("%d",&m);*/

    char* a = (char*)malloc(n*sizeof(char));
	char* b = (char*)malloc(m*sizeof(char));

    scanf("%s", a);
    scanf("%s", b);

    n = strlen(a);
    m = strlen(b);
    int** l = (int**)malloc((n+1)*sizeof(int*)); ///двумерный массив
	for (int i=0;i<n+1;i++){
		l[i] = (int*)malloc((m+1)*sizeof(int));
	}

    for (i = n; i >= 0; i--)
	    for (j = m; j >= 0; j--){
            if (a[i] == '\0' || b[j] == '\0')
                l[i][j] = 0;
            else if (a[i] == b[j])
                l[i][j] = 1 + l[i+1][j+1];
            else if (a[i] != b[j] && a[i] != '\0' && b[j] != '\0')
                l[i][j] = MAX(l[i+1][j], l[i][j+1]);
	    }

    i = 0;
    j = 0;
    while (i < n && j < m){
        if (a[i]==b[j]){
            printf("%c", a[i]);
            i++;
            j++;
        }
        else if (l[i+1][j] >= l[i][j+1])
            i++;
        else
            j++;
    }

    return 0;
}
