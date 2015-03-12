#include <cstdio>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

struct Student
{
	char family [32];
	int mark;
};

void sort(Student* s, int n) //bubblesort of students
{
	int i, j;
	char t[50]; 
	Student a;
	for (i = 1; i < n; ++i)  
	{
		a = s[i];
		for (j = i - 1; ( j >= 0 ) && ( strcmp(s[j].family,a.family) > 0 );  --j)
		{
			s[j+1] = s[j];
		} 
		s[j+1] = a;
	}
}

int binSearch(Student *s, char* key, int l, int r) // main search
{
	int index;	// were is the student
	int k = 0;
	int i;
	int n = strlen(key);
	if (r < l)		// becouse l < r
	{
		return -1;
	}
	if (strcmp(s[r].family,key) == 0) // we found it
	{
		index = r;				
	}
	else
	{
		while (l <= r)
		{
			int mid = l + (r-l)/2;			// not to have big numbers
			if (s[mid].family[k] == key[k]) 
			{
				for (i = k+1; i < n; ++i)
				{
					if (s[mid].family[i] == key[i])
						++k;
					else 
						break;
				}
				if ( (k == (n-1)) && (k == strlen(s[mid].family)-1) ) // we don't break and the length of s[mid].family is length of n
				{
					index = mid; // we found index
					break;
				}
				else 
				{
					++k;
					r = mid;	// new supreme
				}
			}
			else
			{
				if (s[mid].family[k] < key[k])
				{
					if (l != mid)
					{	
						l = mid;	// new infimum
					}
					else		// we don't found this student
					{
						index = -1;
						break;
					}
				}
				else 
				{
					r = mid;	// new supreme
				}
			}
			if (l == r) // we don't found this student
			{
				index = -1;
				break;
			}
		}
	}
	return index;
}

int main()
{
	int n, m;
	char key[28];
	int num = 0;

	printf("Number of students: ");
	scanf("%d/n", &n);
	printf("/n");
 if (n > 0 && n < 100)
 {
	Student* s=(Student*) malloc(n*sizeof(Student));
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &num); // we can forget them
		scanf("%s", &s[i].family);
		scanf("%d" ,&s[i].mark);
	}
	sort(s, n);

	printf("number of requests: ");
	scanf("%d", &m);
	printf("\n");
   if ( m > 0 && m < 100)
   {
	for (int j = 0; j < m; ++j)
	{
		scanf("%s",&key);
		int i = binSearch(s,key,0,n-1);
		if(i != -1) 
			printf("%i\n", s[i].mark);
		else
			printf("\nStudent wasn't found");
	}
   } else
	   printf("Write 0 < number of requests < 100");
  } else
	  printf("Write 0 < number of students < 100");
	getch();
	return 0;
}