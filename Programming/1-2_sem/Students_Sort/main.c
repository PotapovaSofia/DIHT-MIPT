#include "stdio.h"
#include "stdlib.h"

struct Students
{
    int studak;
    int value;
    char name[32];
};

int out_Console(struct Students* Arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d ", Arr[i].studak);
        printf("%s ", Arr[i].name);
        printf("%d ", Arr[i].value);
        printf("\n");
    }
    return 0;
}

void radix_sort(struct Students *Arr, int n) {
 int i;
 struct Students* Brr;
 Brr = (struct Students*)malloc(n*sizeof(struct Students));

 int* Box;
 Box = (int*)malloc(4*sizeof(int));
 for (i = 0; i < 4; i++)
    Box[i] = 0;
  for (i = 0; i < n; i++)
   Box[Arr[i].value - 2]++;
  for (i = 0; i < 4; i++)
   Box[i] += Box[i - 1];
  /*for (i = n - 1; i >= 0; i--){
    Brr[Box[Arr[i].value - 2]] = Arr[i];
    --Box[Arr[i].value - 2];
  }*/
  for (i=0; i<n; i++)
   {
       Brr[Box[Arr[i].value - 2]] = Arr[i];
       Box[Arr[i].value - 2]++;
   }
  for (i = 0; i < n; i++)
   Arr[i] = Brr[i];

 free(Box);
 free(Brr);
}


int main() {

 int i, n;

 printf("\nEnter total elements : ");
 scanf("%d", &n);

 struct Students* Arr;
 Arr = (struct Students*)malloc(n*sizeof(struct Students));


 for (i = 0; i < n; i++)
{
    scanf("%d ", &Arr[i].studak);
    scanf("%s ", Arr[i].name);
    scanf("%d ", &Arr[i].value);

};


 radix_sort(Arr, n);

 out_Console(Arr, n);

 free(Arr);

 return 0;
}
