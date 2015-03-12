#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int (*compFunc)(const void* a,const void* b);
struct Athlete
{
	int weight;
	int strength;
};
void swap(void* a, void* b, size_t size)
{
	char* t = (char*)(malloc(size));
	memcpy(t,a, size);
	memcpy(a, b, size);
	memcpy(b, t, size);
	free(t);
}
int compare_w(const void* a,const void* b)
{
  return (((Athlete*)a)->weight) - (((Athlete*)b)->weight);
}
int compare_s(const void* a,const void* b)
{
  return (((Athlete*)a)->strength) - (((Athlete*)b)->strength);
}
size_t partition(void* base, size_t num, size_t size, compFunc comp)
{
	void* key = (char*) base;
	void* i   = (char*) base - size/sizeof(char);
	void* j   = (char*) base + (num* size)/sizeof(char);

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
void QuickSort(void* base, size_t num, size_t size, compFunc t)
{
	size_t k = partition(base, num , size, t);
	if (k != 0)
	QuickSort(base, k + 1, size, t);
	if ((num-k-1)>0)
	QuickSort((char*)base + ((k+1)*size)/sizeof(char), num - k - 1, size, t);
}

int main ()
{
    int n;
	int i;
	scanf("%d",&n);
	struct Athlete* a = (struct Athlete*)malloc(n*sizeof(struct Athlete));
	for (i=0;i<n;i++)
	{
		scanf("%d %d",&a[i].weight,&a[i].strength);
	}
	printf("\n");
    QuickSort(a,n,sizeof(Athlete),compare_s); ///упорядочиваем атлетов по силе
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
					QuickSort(a,j+1-i,sizeof(Athlete),compare_w); ///упорядочиваем по весу тех, у кого совпала сила
				}
			}
				sum += a[i].weight;
				kol++;
        }
    }
	printf("%d",kol);
    return 0;
}
