#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
int LCS_length(char* a, char* b)
    {
		if (*a == '\0' || *b == '\0')
			return 0;
		else if (*a == *b)
			return 1 + LCS_length(a+1, b+1);
		else
			return MAX(LCS_length(a+1,b), LCS_length(a,b+1));
    }
int main()
{
	int n,k,l;
	scanf("%d",&n);
	scanf("%d",&k);
	char* a;
	char* b;
	a = (char*)malloc(n*sizeof(char));
	b = (char*)malloc(k*sizeof(char));
	scanf("%s",a);
	scanf("%s",b);
	printf("%s\n",a);
	printf("%s\n",b);
	l = LCS_length(a,b);
	printf("%d",l);
	return 0;

}