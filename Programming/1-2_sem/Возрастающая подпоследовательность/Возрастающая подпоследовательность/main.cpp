#include <stdio.h>
#include <stdlib.h>
int main()
{
	int n;
	int i;
	int j;
	printf("Number: ");
	scanf("%d",&n);
	int* last = (int*) malloc(n*sizeof(int));
	if (last == NULL)
		return NULL;
	int* a = (int*) malloc(n*sizeof(int));
	if (a == NULL)
		return NULL;
    printf("Write equence   ");
	for (i=0;i<n;i++)
		scanf("%d",&a[i]);
	int n1=0;
	int k=1;
	last[0] = a[0];
	while (n1<n-1)
	{
		n1++;
		i=0;j=k+1;
		while ((j - i) != 1 )
		{
  			int s = i + (j-i)/2 - 1;
  			if (last[s] >= a[n1])
			{
				j=s+1;
			}
			else
			{
				i=s+1;
			}
		}
  if (i == k)
  {
	  k++;
	last[k-1] = a[n1];
  }
  else
  {
   last[i++] = a[n1];
  }
}
	printf("The length is %d\n",k);
	printf("The equence is\n");
	for (i=0;i<k;i++)
	{
		printf(" %d ",last[i]);
	}
	return 0;
}
