#include <stdio.h>
#include <conio.h>

int main()
{
	int n;
	int time=0;
	unsigned long int f1 = 1, // f(n-2)
                      f2 = 1, // f(n-1)
                      f3 = 2; // f(n)
	scanf("%d", &n);

switch ( n )
{
	case 1:
		printf("1");
		break;
	case 2:
		printf("1");
		break;
	case 3:
		printf("2");
		break;
	default:
		for ( int i = 4; i <= n; ++i)
		{
			f1 = f2;
			f2 = f3;
			f3 = f1 + f2;
			++time;
		}
		printf("%u\ntime: %d", f3, time);
		break;
}
    getch();
return 0;	// with getchar() my programm show the result very fast...
}
