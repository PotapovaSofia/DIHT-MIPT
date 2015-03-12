#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE  1000

struct Queue
{
    int value;
    int priority;
};

void swap (struct Queue* a, struct Queue* b)
{
    struct Queue t;
    t = *a;
    *a = *b;
    *b = t;
}

struct Queue* Queue_Build(struct Queue*Q, int n)
{
    int i;
    for (i = n/2-1; i >= 0; i--)
        Heapify_Down(Q, i, n);
};

void Heapify_Down(struct Queue* Q, int i, int n)
{
    int left, right;
    left = 2*i+1;
    right = 2*i+2;

    int largest;
    largest = i;

    if ((left <= n-1) && (Q[left].priority>Q[i].priority))
        largest = left;

    if ((right <= n-1) && (Q[right].priority>Q[largest].priority))
        largest = right;
    if (largest != i)
    {
        swap(&Q[i], &Q[largest]);
        Heapify_Down(Q, largest, n);
    }
}
void Heapify_Up(struct Queue*Q, int v, int pr, int n)
{

    Q[n].priority=pr;
    Q[n].value=v;

    int i = n;
    while (i>0 && Q[i].priority> Q[(i-1)/2].priority)
    {
        swap (&Q[i], &Q[(i-1)/2]);
        i=(i-1)/2;
    }
}

void delete_Largest (struct Queue* Q, int n)
{
    swap(&Q[0],&Q[n-1]);
    //n--;
    Heapify_Down(Q, 0, --n);
}

int main()
{
    printf("Number\n");
    int n;
    scanf("%d", &n);



    struct Queue Q [MAX_SIZE];
    int i, v, pr;

    for (i = 0; i < n; i++)
        {
            scanf("%d", &pr);
            Q[i].priority=pr;
            scanf("%d", &v);
            Q[i].value=v;
            printf("\n");
        }

    Queue_Build(Q, n);

    for (i=0; i<n; i++){
        printf("%d - ", Q[i].priority);
        printf("%d \n", Q[i].value);
        }

    printf("Write more numbers:");  ///добавить в очередь элементы с заданными приоритетами
    int n1;
    scanf("%d", &n1);
    if (n+n1>MAX_SIZE)
        printf("Queue is full");
    else {
        for (i=0; i< n1; i++)
        {
            scanf("%d", &pr);
            scanf("%d", &v);
            Heapify_Up(Q, v, pr, n);
            n++;

        }}

    printf("\n");

    for (i=0; i<n; i++){
        printf("%d - ", Q[i].priority);
        printf("%d \n", Q[i].value);
    }

    printf("Size of queue: %d\n", n);                    /// Получить размер очереди
    printf("The largest priority: %d\n", Q[0].value);    ///Получить элемент  наибольшим приоритетом

    delete_Largest(Q, n);                                /// Удалить верхний элемент очереди
    n--;

    for (i=0; i<n; i++){
        printf("%d - ", Q[i].priority);
        printf("%d \n", Q[i].value);
    }


    return 0;
}



