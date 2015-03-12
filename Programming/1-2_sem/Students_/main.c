#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Student
{
    int number;
    char surname[28];
    char name [4];
    int mark;
};

void swap (struct Student* a, struct Student* b)
{
    struct Student buf;
    buf=*a;
    *a=*b;
    *b=buf;
};

void Bubble_Sort(struct Student* Arr, int n)
{
    int fl  = 0;
    int i;
    //struct Student buf;
    while (fl==0)
    {
        fl = 1;
        for (i = 0; i<n-1; i++)
            if (strcmp(Arr[i].surname, Arr[i+1].surname)>0)
            {
                fl=0;
                swap(&Arr[i], &Arr[i+1]);
                /*buf=a;
                a=b;
                b=buf;*/
            }
    }

};

/*void insertion_sort(struct Student* Arr, int n)
{
	int i, j,k;
	char t[50];
	struct Student a;
	for (i = 1; i < n; ++i)
	{
		a = Arr[i];
		for (j = i - 1; j>=0 && strcmp(Arr[j].surname,a.surname)>0;  j--)
		{
			Arr[j+1] = Arr[j];
		}
		Arr[j+1] = a;
	}
}*/
int binSearch (struct Student* Arr, char* key, int n)
{
    int cond;
    int low, high, mid;
    low = 0;
    high = n-1;

    while (low<=high)
    {
        mid = (low + high)/2;
        if ((cond = strcmp(key,Arr[mid].surname))<0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
};


int main()
{
    int n;
    printf("Number of students:");
    scanf("%d", &n);
    int i;
    struct Student Arr[n];
    //Arr=(struct Student*)malloc(n*sizeof(struct Student));
    for (i = 0; i<n; i++)
    {
        //scanf("%d, %s, %s, %d",&Arr[i].number, Arr[i].surname, Arr[i].name, &Arr[i].mark);
        scanf("%d",&Arr[i].number);
        scanf("%s",Arr[i].surname);
        scanf("%s",Arr[i].name);
        scanf("%d",&Arr[i].mark);
        printf("\n");
    }

    Bubble_Sort(Arr, n);

    for (i=0; i<n; i++)
        printf("%s, %s\n", Arr[i].surname, Arr[i].name);

    int m;
    printf("Number of requests");
    scanf("%d", &m);

    char* key;
    int j;
    for (j=0; j<m;j++)
    {
        scanf("%s", key);
        int p = binSearch(Arr, key, n);
        if (p == -1)
            printf("Student %s not found", key);
        else printf("The mark of the student %s is %d", key, Arr[p].mark);

    }

    return 0;
}
