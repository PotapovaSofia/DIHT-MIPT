#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int (*compFunc)(const void* a,const void* b);
void* t;
struct Athlete
{
	int weight;
	int strength;
};
void swap(void* a, void* b, size_t size)
{
	memcpy(t,a, size);
	memcpy(a, b, size);
	memcpy(b, t, size);	
}
int compare_w(const void* a,const void* b)
{
  if ((((Athlete*)a)->weight) > (((Athlete*)b)->weight))
		return 1;
	if ((((Athlete*)a)->weight) < (((Athlete*)b)->weight))
		return -1;
	if ((((Athlete*)a)->weight) == (((Athlete*)b)->weight))
		return 0;
}
int compare_s(const void* a,const void* b)
{
  if ((((Athlete*)a)->strength) > (((Athlete*)b)->strength))
		return 1;
	if ((((Athlete*)a)->strength) < (((Athlete*)b)->strength))
		return -1;
	if ((((Athlete*)a)->strength) == (((Athlete*)b)->strength))
		return 0;
}
size_t partition(void* base, size_t num, size_t size, compFunc comp)
{
	void* key = (char*) base;
	void* i   = (char*) base - size/sizeof(char);
	void* j   = (char*) base + (num* size)/sizeof(char);
	if (t==NULL)
	{
		t = malloc(size);
	}
	while (true)
	{
		do
			{
				j = (char*)j - size;
				
			} while ((*comp)(j, key) > 0);
		
		do
			{
				i = (char*)i + size;
				
			} while ((*comp)(i, key) < 0);
		if (i < j)
			swap(i, j, size);
		else 
			return ((char*)j-(char*)base)/size;
	}
}
void quicksortinner(void* base, size_t num, size_t size, compFunc t)
{
	size_t k = partition(base, num , size, t);
	if (k != 0)
	quicksortinner(base, k + 1, size, t);
	if ((num-k-1)>0)
	quicksortinner((char*)base + ((k+1)*size)/sizeof(char), num - k - 1, size, t);
}
void quicksort(void* base, size_t num, size_t size, compFunc comp)
{
	if (t==NULL)
		t = malloc(size);
	quicksortinner(base,num, size, comp);
	free(t);
}
int main ()
{
    int n;
	int i;
	scanf("%d",&n);
	Athlete* a = (Athlete*)malloc(n*sizeof(Athlete));
	for (i=0;i<n;i++)
	{
		scanf("%d %d",&a[i].weight,&a[i].strength);
	}
	printf("\n");
    quicksort(a,n,sizeof(Athlete),compare_s);
    int sum = 0;
	int kol = 0;
    for (int i = 0; i < n; i++)
    {
        if (a[i].strength >= sum )
        {
			for (int j=i;j<n;j++)
			{
				if (a[i].strength==a[j+1].strength)
				{
					quicksort(a,j+1-i,sizeof(Athlete),compare_w);
				}
			}
				sum += a[i].weight;
				kol++;
        }
    }
	printf("%d",kol);
    return 0;
}
