#include <cstdio>
#include <stdlib.h>
#include <conio.h>

int Kahan(long double *a,int n)
{
	long double sum,y,t,c;
	 sum = 0.0;
	 c = 0.0;
	for (int i = 0; i < n; ++i)
	{
	 y = a[i] - c;   
     t = sum + y;       
     c = (t - sum) - y;   
     sum = t;        
	}
	printf("%lf", sum);
return 0;
}

int main()
{
	int n;
	long double *a;
	long double sum = 0.0;
	printf("Number of elements : ");
	scanf("%i", &n);
	if (n > 0 && n<1000)
{
	a = (long double*) malloc(sizeof(long double)*n);
	for (int i = 0; i < n; ++i)
		scanf("%lf",&a[i]);
	for (int i = 0 ; i < n; ++i)
		sum += a[i];
	printf("Sum v lob - ");
	printf("%lf\n", sum);
	printf("Kahan sum - ");
	Kahan(a,n);
} 
	else
		printf("Unexpected number, write 0 < number < 1000");
	getch();
	return 0;
}
