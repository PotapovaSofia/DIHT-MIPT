#include <stdio.h>
#include <stdlib.h>
#include <time.h>



void swap (int* a, int* b)
{
    int buf;
    buf = *a;
    *a = *b;
    *b = buf;
}
void Bubble_Sort (int* Arr, int n)
{
    char flag = 0;
    int i;
    while (flag == 0)
    {
        flag = 1;
        for (i = 0; i < n - 1; i++)
            if (Arr[i] > Arr[i+1])
            {
                flag = 0;
                swap (&Arr[i], &Arr[i+1]);
            }
    }
}

void generation(int* Arr, int n)
{
	int i;
	srand(time(NULL));
	for ( i=0;i<n;++i)
	{
		Arr[i] = rand()%100;
		printf("%d ", Arr[i]);
	}
	printf("\n");
}

int in_file (int* Arr, int n)
{
    FILE* f = fopen("input.txt", "rt");
    int i;
    for (i = 0;i < n; i++)
    {
        fscanf(f, "%d", Arr + i);
    };
    fclose(f);
    return 0;
}

void in_console (int* Arr, int n)
{
    int i;

    for (i = 0;i < n; i++)
    {
        scanf("%d", &Arr[i]);
    }
}

int out_File (int* Arr, int n)
{
	FILE* f = fopen("output.txt","w");
	int i;
	for (i=0;i<n;++i)
	{
		fprintf(f,"%d ",Arr[i]);
	}
	fclose(f);
	return 0;

}

int out_Console(int* Arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d ", Arr[i]);
    }
    return 0;
}

int main()
{
    int in_type;
    int n;


    printf("What do you want to use? \nIf in_file - put 1;if in_console - put 2; \nif generation of massive, put 3\n");
    scanf("%d", &in_type);

    printf("Write a number of massive elements ");
        scanf("%d", &n);

    int* Arr;
    Arr = (int*)malloc(n*sizeof(int));

    if (in_type == 1)
        in_file(Arr, n);

    if (in_type == 2)
    {
        in_console(Arr, n);
    };
    if (in_type == 3)
    {
        generation(Arr, n);
    };


    Bubble_Sort(Arr, n);

    int out_type;
    printf("What do you want to use? \nIf out_file - put 1;if out_console - put 2\n");
    scanf("%d", &out_type);

    if (out_type == 1)
    {
        out_File(Arr, n);
    }

    if (out_type == 2)
    {
        out_Console(Arr, n);
    }

    free(Arr);


    return 0;
}
