#include "HashTable.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    HashTable* htb = htb_create(sizeof(char*));
    htb_insert(htb, "774-70-86", "Sofia");
    htb_insert(htb, "771-89-56", "Kate");
    htb_insert(htb, "891-23-90", "Maksim");
    htb_insert(htb, "716-34-23", "Igor");
    htb_insert(htb, "790-81-90", "Andrew");

    char* res;
    res = htb_find(htb, "Kate");
    printf("Kate's phone number is: %s\n", res);
    //htb_delete(htb, "Kate");
    //res = htb_find(htb, "Kate");
    htb_destroy(htb);

/////////////////////////////////////////////////////

    HashTable* tb = htb_create(sizeof(int));
    htb_insert(tb, 5, "Sofia");
    htb_insert(tb, 3, "Kate");
    htb_insert(tb, 4, "Maksim");
    htb_insert(tb, 2, "Igor");
    htb_insert(tb, 1, "Andrew");

    int res1;
    res1 = htb_find(htb, "Kate");
    printf("Kate's mark is: %d\n", res1);
    //htb_delete(tb, "Kate");
    //res = htb_find(tb, "Kate");
    htb_destroy(tb);
    return 0;
}
