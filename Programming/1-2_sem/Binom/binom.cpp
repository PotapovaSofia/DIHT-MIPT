#include <stdio.h>
#include <conio.h>

double C(int n, int k)
{
	if ( n < 0 || k < 0 || k > n) 
		return -1;

	double f;
	if (n == 0 || n == k)
		f = 1;
	else
		f = ( n/(n - k) ) * C(n - 1, k);
	return f;
}

int main()
{
	int n, k;
	double bin = 0;

	printf("Write n:\n");
	 scanf("%d", &n);

	printf("Write k:\n");
	 scanf("%d", &k);

	bin = C(n,k);
	if ( bin == -1 )
		printf("Uncorrect number");
	else
		printf("The binomial factor = %f", bin);

	getch();
return 0;
}