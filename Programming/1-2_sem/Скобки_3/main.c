#include <stdio.h>
#include <stdlib.h>

#define END 10
#define OPEN_BRACKET_1 40
#define CLOSE_BRACKET_1 41
#define OPEN_BRACKET_2 123
#define CLOSE_BRACKET_2 125


struct List
{
  struct Link* head;
};

struct Link
{
    struct Link* next;
    int value;
};

struct List* list_new(void)
{
    struct List* lst = (struct List*)malloc(sizeof(struct List));
    if (!lst)
    {
        fprintf(stderr," ");
        return NULL;
    }
    lst -> head = NULL;
    return lst;
};

void unshift (struct List* lst, int v)
{
    struct Link* p = (struct Link*)malloc(sizeof(struct Link));
    p ->value = v;
    struct Link* b = lst ->head;

    lst ->head = p;
    p ->next = b;
}

int shift(struct List* lst)
{
    struct Link* p = lst ->head;
    if (!p)
        return 0;
    int x;
    x = p ->value;
    lst ->head = p ->next;
    free(p);
    return x;
}

int read_first(struct List* lst)
{
    struct Link* p = lst ->head;
    if (!p)
        return 0;
    int x;
    x = p ->value;
    return x;
}
int full_(struct List* lst){
    if (lst ->head==NULL)
        return 0;
    else
        return 1;
}

int main()
{
    struct List* lst = list_new();

    /*char symb = '(';
    int open_bracket1 = (int)(symb);
    symb = ')';
    int close_bracket1 = (int)(symb);
    symb = '{';
    int open_bracket2 = (int)(symb);
    symb = '}';
    int close_bracket2 = (int)(symb);
    printf("( - %d; ) - %d; { - %d; } - %d", open_bracket1, close_bracket1, open_bracket2, close_bracket2);*/

    char charT;
    int i=1;

    while(i==1){
            charT = getchar();
            int TT = (int)(charT);
            switch (TT) {
            case OPEN_BRACKET_1:
                unshift(lst, OPEN_BRACKET_1);
                break;
            case CLOSE_BRACKET_1:
                if (read_first(lst)==OPEN_BRACKET_1)
                    shift(lst);
                else{
                    printf("Structure is incorrect");
                    return 0;    }
                break;
            case OPEN_BRACKET_2:
                unshift(lst, OPEN_BRACKET_2);
                break;
            case CLOSE_BRACKET_2:
                if (read_first(lst)==OPEN_BRACKET_2)
                    shift(lst);
                else{
                    printf("Structure is incorrect");
                    return 0; }
                break;
            case END:
                i = 0;
                break;

            default:
                printf("Unexpected symbol: %c", charT);
                return 0;
                break;


            }
    }
    if (full_(lst)==1)
        printf("Structure is incorrect");
    else
        printf("Structure is correct");



    return 0;
}

