
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

typedef struct {
    char** storage;
    int capacity;
    int size;
} words_t;

int words_init(words_t* words) {
    words->storage = malloc(sizeof(char*) * 4);
    assert(words->storage);
    words->capacity = 4;
    words->size = 0;
    return 0;
}

int read_word(FILE* f, words_t* words)
{
    int i = 0;
    int j = 0;
    int bufsize = 32;
    char* buf = malloc(sizeof(char) * bufsize);
    assert(buf);
    buf[0] = fgetc (f);
    while (buf[0] == ' ' || buf[0] == '\n' || buf[i] == '\r')
    {
        buf[0] = fgetc (f);
    }
    int flag = 1;  
    while ((int)(buf[i])!= EOF)
    {
	    if (buf[i] == ' ' || buf[i] == '\n' || buf[i] == '\r')
            {
                if(flag == 1)
                {
                    if((j+1) > words->capacity)
                    {
                        words->capacity = words->capacity * 2;
                        words->storage = realloc(words->storage, sizeof(char*) * words->capacity);
                        assert(words->storage);
                    }
                    buf[i] = '\0';
                    words->storage[j] = buf;
		   
                    buf = 0;
                    buf = malloc(sizeof(char) * 32);
                    assert(buf);
                    i = 0;
		    j +=1;
                }
		flag = 0;
            }
            else
            {
                flag = 1;
                if (buf[i]!='.' && buf[i]!= ',' && buf[i]!= '!' && buf[i]!= '?' && buf[i]!= '\t')
                    i+=1;
            }
	    if(i == bufsize -1)
	    {
		bufsize *=2; 
		buf = realloc(buf, sizeof(char)*bufsize);
		assert(buf);
	    }
            buf[i] = fgetc (f);
    }
    if (flag == 1)
    {
        if((j+1) > words->capacity)
        {
            (words->capacity)++;
            words->storage = realloc(words->storage, sizeof(char*) * words->capacity);
            assert(words->storage);
        }
        buf[i] = '\0';
        words->storage[j] = buf;
	buf = 0;
	j +=1;
    }
	words->size = j;
	free(buf);
	return 0;
}
void words_cout(words_t* words)
{
    int i = 0;
    while(i < words->size)
        {
            printf("%s", words->storage[i]);
	    i+=1;
	}
}
int words_code(char* A)
{
    int code = 0;
    int j;
    for(j = 0;j< strlen(A);++j)
    {
       code +=(int)(A[j])*(j+1);
    }
    return code;
}

int words_sort(words_t* words) 
{
    int* arr = malloc(sizeof(int) * (words->size + 1));
    assert(arr);
    int code, j, i, count = 0;
    for(i = 0;i < words->size;++i)
    {
       arr[i] = words_code(words->storage[i]);
    }
    for(i = 0;i < words->size;++i)
        for(j = i+1; j < words->size;++j)
        {
            if((arr[i] == arr[j]) && (arr[i] != -1))
            {
                if((strcmp(words->storage[i], words->storage[j])) == 0)
                  arr[j] = -1;
            }
        }
    for(i = 0;i < words->size;++i)
        if(arr[i] != -1)
            count++;
    free(arr);
    return count;
}
void words_count(words_t* words)
{
    printf("\nCount: %d\n", words->size);
}

void words_destroy(words_t* words)
{
	free(words->storage);
}

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        printf("You forgot to enter a file name\n");
        exit(1);
    }
    FILE* f = fopen(argv[1], "r");
    assert(f);
    words_t words;
    words_init(&words);
    read_word(f, &words);
    close(f);
    
   
//    words_cout(&words);
    words_count(&words);
    int res = words_sort(&words);
    printf("Different words count: %d\n", res);
    words_destroy(&words);
    return 0;
}



