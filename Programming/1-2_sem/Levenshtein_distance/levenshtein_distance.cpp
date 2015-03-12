#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm> 
#include <queue> 
using namespace std;
int min3(int a, int b, int c)
{
	if ( (a < b) && (a < c) )
		return a;
	if ( (b < a) && (b < c) )
		return b;
	if ( (c < a) && (c < b) )
		return c;
return a;
}

int levdis(char *s1, char *s2)
{
	int i, j, c;
	int m = strlen(s1);
	int n = strlen(s2);

	if (m == 0)
		return n;
	if (n == 0)
		return m;      

    int **a =(int**)malloc( (m+1)*sizeof(int*) );
    for (i = 0; i <= m; ++i) {
		a[i] = (int*)malloc( (n+1)*sizeof(int) );
		a[i][0] = i;
	}
    for (j = 0; j <= n; ++j) 
		a[0][j] = j;
 
    for (i = 1; i <= m; ++i)
		for (j = 1; j <= n; ++j)
        {
			c = ( s1[i-1] == s2[j-1] ) ? 0 : 1;
			a[i][j]=min3( a[i-1][j]+1,
						  a[i][j-1]+1,
						  a[i-1][j-1]+c );
		}
	return a[m][n];                
}
template<class T>
void my_swap(T &a, T &b)
{
	T tmp(move(a));
	a = move(b);
	b = move(tmp);
}

struct classcomp {
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};

int main()
{
	vector<vector<int>> a;
	vector<vector<int>> b(5, vector<int>(5,-1));
	a = move(b);
	/*int* arr = (int*)malloc(sizeof(int)*2);
	arr[0] = 1;
	arr[1] = 2;
	arr = (int*)realloc(arr, sizeof(int)*3);

	char s1[255]; 
	char s2[255];
	char c;
	printf("Write 2 strings (length < 255): ");
	gets(s1);
	gets(s2);
	int k = levdis(s1, s2);
	printf("%d", k);*/
	getch();
	return 0;
}