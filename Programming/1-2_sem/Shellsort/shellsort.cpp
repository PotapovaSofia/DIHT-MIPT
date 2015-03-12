#include <cstdio>
#include <stdlib.h>
#include <conio.h>

int increment(int inc[], int size) {
  int p1 = 1, 
	  p2 = 1, 
	  p3 = 1, 
	  s = -1;
  do {
    if (++s % 2) {
      inc[s] = 8*p1 - 6*p2 + 1;
    } else {
      inc[s] = 9*p1 - 9*p3 + 1;
      p2 *= 2;
      p3 *= 2;
    }
	p1 *= 2;
  } while(3*inc[s] < size);  

  return s > 0 ? --s : 0;
}

void shell(int *a, int size) {
  int inc, j, i, b[30], s;
  s = increment(b, size);
  while (s >= 0) {
	inc = b[s--];
    for (i = inc; i < size; i++) {
      int temp = a[i];
      for (j = i-inc; (j >= 0) && (a[j] > temp); j -= inc)
        a[j+inc] = a[j];
      a[j+inc] = temp;
    }
  }
}

int main()
{
	int n;
	printf("Number of elements: ");
	scanf("%d", &n);
	if ( (n > 0) && (n < 100 ) ){
	int *a = (int*)malloc(n*sizeof(int));
	for (int i = 0; i < n; ++i)
		scanf("%i", &a[i]);
    shell(a, n);
	for (int i = 0; i < n; ++i)
		printf("%d ",a[i]);
	}else
		printf("Write 0 < number < 100");
	getch();
	return 0;
}