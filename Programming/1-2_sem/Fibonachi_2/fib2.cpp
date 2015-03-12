#include <iostream>
#include <conio.h>

int fn(int N, int &time1, long int *arr)  // recursion
{
	++time1;
	if (arr[N] != 0) return arr[N]; 
	else
	{
		int ans = fn(N-1, time1, arr) + fn(N-2, time1, arr);
		arr[N] = ans;
	}
	return arr[N];
}

int main()
{
	int n;
	int time1 = 0; // for recursion
	int time2 = 0; // without recursion
	long int fib = 0;
	long int f1 = 1,
			 f2 = 1,
			 f3 = 0;

		printf("n= ");
		scanf("%d", &n);
	long int *arr = (long int *) malloc( n * sizeof(long int) ); // dinamic arr
		arr[0] = 1;
		arr[1] = 1;
	for (int i = 2; i < n; ++i) 
		arr[i] = 0;

	if (n == 1)
	{ 
		printf("1");
		return 0;
	}
	if (n == 2)
	{
		printf("1");
		return 0;
	}

	if ( n < 80 && n>0)  	    // if n entered correctly 
{ 
		n -= 1;						// because arr starts with 0 number of element
		fib = fn(n, time1, arr);		// main function
		printf("\nF1= %ld", fib); 
		printf("\nTime with recursion: %d\n", time1);

		for ( int i = 3; i <= n+1 ; ++i ) // without recursion
		{
			f3 = f1 + f2;
			f1 = f2;
			f2 = f3;
			++time2;
		}
		printf("F2 = %ld\nTime without recursion: %d", f3, time2);

}
	else 
		printf("Uncorrect n, write 0 < n < 80");
 getch();			// with getchar() my programm show the result very fast...
return 0;
}