#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct Athlete
{
	int weight;
	int strength;
};

void qsort(Athlete *a,int first, int last)
{
	int i, j, p;
	Athlete temp;
	i = first; 
	j = last;
	p = a[ (first + last)/2 ].strength;
	while (i <= j)
	{ 
	    while ( a[i].strength < p ) 
			i++;
		while ( a[j].strength > p ) 
			j--;
		if (i <= j) 
		{
			temp = a[i]; 
			a[i++] = a[j]; 
			a[j--] = temp;
		}
	} 

	if (i < last) 
		qsort(a, i, last);
	if (first < j) 
		qsort(a, first, j);
}

int main ()
{
    int n;
	int i;
	scanf("%d",&n);
	Athlete *a = (Athlete*)malloc(n * sizeof(Athlete));
	for (i = 0; i < n; ++i)
		scanf("%d %d",&a[i].weight, &a[i].strength);
	printf("\n");

    qsort(a, 0, n-1);

    int sum = 0;
	int kol = 0;
    for (i = 0; i < n; i++)
    {
        if (a[i].strength >= sum )
        {
			for (int j = i; j < n; ++j)
				if (a[i].strength == a[j+1].strength)
					qsort(a, 0, j+1-i);
			sum += a[i].weight;
			kol++;
        }
    }
	printf("%d", kol);
	getch();
    return 0;
}
