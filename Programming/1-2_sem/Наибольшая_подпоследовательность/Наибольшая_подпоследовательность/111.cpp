#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max(a,b) ((a>b) ? a : b)
char* LCS(char* a, char* b)
{
	int l1, l2;
	int i;
	int j;
	l1 = strlen(a);//строки
	l2 = strlen(b);//столбцы
	if (l1 == 0 || l2 == 0)
		return NULL;
	int** m = (int**)malloc((l1 + 1) * sizeof(int*));
	if (m == NULL)
		return NULL;
	for (i = 0; i <= l1; ++i)
		m[i] = (int*)malloc((l2 + 1)*sizeof(int));

	for (i = 0; i <= l1; ++i)
		m[i][0] = 0;
	for (i = 1; i <= l2; ++i)
		m[0][i] = 0;

	for (i = 1; i <= l1; ++i)
		for (j = 1; j <= l2; ++j)
		{
			if (a[i - 1] == b[j - 1])
				m[i][j] = m[i - 1][j - 1] + 1;
			else m[i][j] = max(m[i - 1][j], m[i][j - 1]);
		}


	char* str = (char*)malloc(m[l1][l2]*sizeof(char));
	if (str == NULL)
		return NULL;
	i = l1;
	j = l2;
	int k = m[l1][l2] - 1;
	while (m[i][j] != 0)
		{
			if (m[i][j] > m[i - 1][j] && m[i][j] > m[i][j - 1])
				{
					--i;
					--j;
					str[k] = a[i];
					--k;
				}
			else  if (m[i - 1][j] == m[i][j - 1])
					--i;
					else if (m[i - 1][j] > m[i][j - 1])
							--i;
						else --j;
		}

	int h = m[1][l2];
	for (i = 0; i < 2; ++i)
		free(m[i]);
	free(m);
	return str;
}

int main()
{
	int i;
	char* str;
	char k;
	int l = 10;
	i = -1;
	do
	{
		++i;
		if (i == 0)
			{
				str = (char*)malloc(l * sizeof(char));
				if (str == NULL) return -1;
			}
		else
		   if (l <= i + 1)
			{

				str = (char*)realloc(str, (l + l)*sizeof(char));
			    if (str == NULL) return -1;
			    l = l + l;
			}
		scanf("%c", &str[i]);


	} while (str[i] != '\n');
	str[i] = '\0';

	char* str1;
	i = -1;
	l = 10;
		do
	{
		++i;
		scanf("%c", &k);
		if (i == 0)
			{
				str1 = (char*)malloc(sizeof(char));
				if (str1 == NULL) return -1;
			}
		else
			if (l < i)
			{
				str1 = (char*)realloc(str1, (l + l)*sizeof(char));
				if (str1 == NULL) return -1;
				l = l + l;
			}

		str1[i] = k;
	} while (str1[i] != '\n');
	str1[i] = '\0';
	char* a = LCS(str, str1);
	printf("%s", a);
	//free(a);
	return 0;
}//сдана
