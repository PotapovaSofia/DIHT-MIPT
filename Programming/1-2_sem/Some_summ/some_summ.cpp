#include <cstdio>
#include <math.h>
#define pi 3.1415//acos(-1.0)

int main()
{
	long int n;
	float sumf = 0.0;
	double sumd = 0.0;
	printf("Write n: ");
	scanf("%d", &n);
  if (n > 0)
  {
	for (int i = 1; i <= n; ++i) //increase
	{
		sumf = sumf + float( 1.0 / (i*i) );
		sumd = sumd + double( 1.0 / (i*i) );
	}
	printf("In increasing order:\n float = %f\n double = %f ",sumf, sumd);
	sumf = 0.0;
	sumd = 0.0;
	for (int i = n; i > 0; --i) //decrease
	{
		sumf = sumf + float( 1.0 / (i*i) );
		sumd = sumd + double( 1.0 / (i*i) );
	}
	printf("\nIn decreasing order:\n float = %f\n double = %f ",sumf, sumd);

	long double s = pi*pi / 6.0;
	printf("\nExact summ: %f", s);
  } else
	  printf("Unexpected n");
  getchar();
}
