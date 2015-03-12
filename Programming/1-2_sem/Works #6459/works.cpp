#include "works.h"
#include <string.h>

Works::Works(int size = 1)
{
	work = (Book*)malloc(size * sizeof(Book));
	for(int i = 0; i < size; ++i)
		work[i] = Book();
	Works.size = size;
}

Works::~Works()
{
	delete[] work;
}

void Works::set(Works &a)
{
	char* author;
	char* name;
	cout << "Author: ";
	cin >> author;
	strcpy(a.author, author);
	cout << "\nName of works: ";
	cin >> name;
	strcpy(a.name, name);
	cout << "\nNumbers of books: ";
	cin >> a.size;
}

bool Works::delete_book(int i)
{
	delete[i] work;
	return true;
}

bool Works::delete_book(char* a)
{
	int i;
	bool b = false;
	for (i = 0; i < size; ++i)
		if (name == a)
		{
			delete[i] work;
			b = true;
		}
	return b;
}

bool Works::add_book(Book &a)
{
	++size;
	work = (Book*)malloc(size * sizeof(Book));
	if (work == NULL)
		return false;
	work[size] = a;
	return true;
}
