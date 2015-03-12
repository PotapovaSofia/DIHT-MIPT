#include <stdio.h>
#include <stdlib.h>
int main()
{
	int k,m;
	char *p,j;
	double i;
	printf("Write a count - \n");
    scanf("%lf",&i);
    p=(char*)&i;
	char* str = (char*) malloc((sizeof(i)*sizeof(i))*sizeof(char));
    m=sizeof(i)*sizeof(i) - 1;
    for(k=0; k<sizeof(i); k++)
	{
		j=1;
		while (j!=0)
		{
			if (*p&j) str[m--]='1'; 
			else str[m--]='0'; 
			j=j<<1; 
		}
		p++;
	}
	for (int j=0;j<(sizeof(i)*sizeof(i)-1);j++)
	{
		for (int i=j;i<j+8;i++)
		{
			printf("%c",str[i]);
		}
		printf(" ");
		j+=7;
	}
return 0; 
}
