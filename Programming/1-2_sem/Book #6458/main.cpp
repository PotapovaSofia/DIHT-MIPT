#include "book.h"
#include "book.cpp"
#include <iostream>

int main() 
{
	Book b;
	b.set_author("Bulgakov");
	b.set_name("Heart of a Dog");
	b.set_publish("Harcourt Brace");
	b.set_age(1925);
	b.set_pages(80);
	char** cont;
		cont[0] = "chapter 1";
		cont[1] = "chapter 2";
		cont[2] = "chapter 3";
		cont[3] = "chapter 4";
		cont[4] = "chapter 5";
	b.set_content(cont, 5);
	b.get_book(b);
	b.clear(b);
	return 0;
}