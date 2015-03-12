#include <stdio.h>
#include <conio.h>

int main()
{
	int sum = 0;
	char c;
	bool b = true;

	do
{
	    c=getchar();
		switch(c) 
		{
			case '(':
				++sum;
				break;
			case ')':
				--sum;
				break;
			case '\n':
				break;
			default: 
				printf("Unexpected symbol: %c",c);
				b = false;
		}
	if (sum < 0)
		break;
} while (c != '\n');

if (b)
{
	if (sum == 0)
		printf("YES");
	else
		printf("NO");
}
getch();
return 0;
}