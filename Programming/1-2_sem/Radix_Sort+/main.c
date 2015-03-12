#include "stdio.h"
#include "stdlib.h"

//#define MAX 1000
//#define SHOWPASS

int out_Console(int* Arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d ", Arr[i]);
    }
    return 0;
}

void generation(int* Arr, int n)
{
	int i;
	srand(time(NULL));
	for ( i=0;i<n;++i)
	{
		Arr[i] = rand()%1000;
		//printf("%d ", Arr[i]);
	}
	//printf("\n");
}
void radix_sort(int *Arr, int n) {
 int i, m = 0, exp = 1;
 int* Brr;
 Brr = (int*)malloc(n*sizeof(int));

 int* Box;
 Box = (int*)malloc(10*sizeof(int));

 for (i = 0; i < n; i++) {
  if (Arr[i] > m)
   m = Arr[i];
 }

 while (m / exp > 0) {
  for (i = 0; i < 10; i++)
    Box[i] = 0;
  for (i = 0; i < n; i++)
   Box[Arr[i] / exp % 10]++;
  for (i = 1; i < 10; i++)
   Box[i] += Box[i - 1];
  for (i = n - 1; i >= 0; i--)
   Brr[--Box[Arr[i] / exp % 10]] = Arr[i];
  for (i = 0; i < n; i++)
   Arr[i] = Brr[i];
  exp *= 10;

/*#ifdef SHOWPASS
  printf("\n\nPASS   : ");
  print(a, n);
#endif*/
 }
 free(Box);
 free(Brr);
}


int main() {

 int i, num;

 printf("\nEnter total elements : ");
 scanf("%d", &num);

 /*printf("\nEnter %d Elements : ", num);
 for (i = 0; i < num; i++)
  scanf("%d", &arr[i]);*/

 int* Arr;
 Arr = (int*)malloc(num*sizeof(int));

 generation(Arr, num);

 printf("\nARRAY  : ");
 out_Console(Arr, num);

 radix_sort(Arr, num);

 printf("\n\nSORTED  : ");
 out_Console(Arr, num);

 free(Arr);

 return 0;
}
