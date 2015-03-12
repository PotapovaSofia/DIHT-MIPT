#include <stdio.h>
int k,m;
char *p,j;
char str[70];
int main()
{
	printf("Choose type of count,if float - push1,if double - push2,if int - push 3");
	scanf("%i/n",&k);
	if (k==1)
	{
		float f;
		scanf("%f",&f);
	p=(char *)&f;
	m=0;
	for (k=0;k<4;++k)
	{j=1;
	while(j!=0)
	{
		if(*p&j) str[m++]='1'; 
		else str[m++]='0'; 
		j=j<<1;
	}
	p++;
	}
	}
	if (k==2)
	{
		double i;
		scanf("%le",&i);
        p=(char *)&i;
        m=63;
        for(k=0; k<8; k++)
		{
			j=1;
			while (j!=0)
			{
				if(*p&j) str[m--]='1'; 
				else str[m--]='0'; 
				j=j<<1; 
			}
			p++;
		}

	}
	if (k==3)
	{
		int d;
		scanf("%i",&d);
	p=(char *)&d;
        m=0;
        for(k=0; k<4; k++)
		{
			j=1;
			while (j!=0)
			{
				if(*p&j) str[m++]='1'; 
				else str[m++]='0'; 
				j=j<<1; 
			}
			p++;
		}

	}

printf("%s",str);
return 0; 
}
