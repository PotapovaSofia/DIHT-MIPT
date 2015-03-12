#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int main()
{
	int n,i;
	printf("Write n: ");
	scanf("%d", &n);
    if (n>0 && n<200)
{
	long int *a=(long int*)malloc(n*sizeof(long int));
	printf("Write array:\n");
	for (i=0; i<n; i++)
{
		scanf("%u", &a[i]);
}

int min=a[1];
int max=a[1];
double mean=0;
double dev=0;
double disp=0;

for (i=0; i<n; i++)  // min and max
{
	if ( a[i] < min )
		min = a[i];
	if ( a[i] > max )
		max = a[i];
	 mean = mean + (double)(a[i])/n; // not to create one more var sum
}

for (i = 0; i < n; ++i) { // dispersion and deviation
	dev = dev + (a[i] - mean)/n;
	disp = disp + (a[i] - mean)*(a[i] - mean)/n;
}

	printf("min: %d", min);
	printf("\nmax: %d", max);
	printf("\narithmetical mean: %f", mean);
	printf("\ndeviation: %f", dev);
	printf("\ndispersion: %f", disp);
} else
    printf("Uncorrect n");
 getch();
return 0;
}
