#include <cstdio>
#include <string.h>
#include <cstdlib>
#include <conio.h>

int in = -1;

char **split(const char *str, const char *delim)
{
	char **ar=(char **)malloc(in * sizeof(char *));

	int k = 0;
	int l = 0;
	for (int i = 0; i < in; ++i){
		for (int j = 0; j < in; ++j)
			if (str[j] != delim[i])
			{
				ar[i] = (char *)malloc(in*sizeof(char)); 
				char t = str[j];
				ar[i++][k++] = str[j];
			}
	}
	return ar;
}

void delete_string_array(char **str)
{
	for (int i = 0;  i < in; ++i)
		free(str[i]);
	free(str);
}

int main()
{
	char str[255];
	char delim[255];
	char c;
	printf("Write your string: ");
	while ( ( c = getchar()) != '\n' )
		str[++in] = c;
	++in;
//	realloc(str, in*sizeof(char));
	printf("Write your delim: ");
	gets(delim);
//	realloc(delim, in*sizeof(char));
	char **arr=(char **)malloc(in * sizeof(char **));
	for(int j = 0; j < in; ++j)
		for(int k = 0; k < in; ++k) 
			arr[j,k] = "";
	arr = split(str, delim);
	for (int i = 0; i < in; ++i)
		for (int j = 0; j < in; ++j)
			printf(arr[i,j]);
	delete_string_array(arr);
	getch();
	return 0;
}