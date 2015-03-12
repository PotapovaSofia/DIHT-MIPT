#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void generation(int* Arr, int n)
{
	int i;
	srand(time(NULL));
	for ( i=0;i<n;++i)
	{
		Arr[i] = rand()%100 - 50;
		printf("%d ", Arr[i]);
	}
	printf("\n");
}

int main()
{
   int n;
   printf("Numbers: ");
   scanf("%d", &n);


   int* d = (int*)malloc(n*sizeof(int));
   //int* p = (int*)malloc(n*sizeof(int));
   int* a = (int*)malloc(n*sizeof(int));

   int i;
   /*for (i = 0; i<n; i++)
    scanf("%d", &a[i]);*/
   generation(a, n);
   for (int i=0; i<n; ++i) {
        d[i] = 1;
        //p[i] = -1;
        for (int j=0; j<i; ++j)
            if (a[j] < a[i])
                if (1 + d[j] > d[i]) {
                    d[i] = 1 + d[j];
                    //p[i] = j;
                }
}

    int max = d[0],  pos = 0;
    for (int i=0; i<n; ++i)
        if (d[i] > max) { ///берем первый максимум, который нам встретился
            max = d[i];
            pos = i;
        }
    printf("length: %d\n", max);

    int* b = (int*)malloc(max*sizeof(int));
    int max1 = max;
    int k = max-1;
    for (i = pos; i >= 0;i--){
        if (d[i]==max){
            max--;
            b[k--] = a[i];
        }
    }

    for (i = 0; i < max1; i++)
        printf("%d  ", b[i]);

    return 0;
}
