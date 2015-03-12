#include <stdio.h>
#include <conio.h>
#include <math.h>

int main()
{
	long int n;
	long double d;
	bool prime = true;
	scanf("%ld", &n);

	if (n < 0 || n > 2147483000)
		printf("Unexpected n, write 0 < n < 2 147 483 000");
	else
	{
		d = sqrt( long double(n) );
		for (int i = 2; i <= d; ++i)
			if (n % i == 0) 
			{
				prime = false;
				break;
			}
		if (prime)
			printf("It is prime");
		else
			printf("It is not prime");
	}

 getch();
return 0;
}