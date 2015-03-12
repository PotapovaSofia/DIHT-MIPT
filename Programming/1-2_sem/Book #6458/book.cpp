#include "book.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

Book::Book()
{
	author = "I";
	name = "My Book";
	publish = "House";
	age = 2000;
	pages = 0;
	content_size = 0;
	content = (char**)malloc( content_size*sizeof(char*) );
	for(int i = 0; i < content_size; ++i)
		content[i] = "";
}

Book::~Book()
{
	clear(*this);
}

void Book::set_author(char* author)
{
	memcpy(Book::author, author, strlen(author));
}
void Book::set_name(char* name)
{
	memcpy(Book::name , name, strlen(name));
}
void Book::set_publish(char* publish)
{
	memcpy(Book::publish , publish, strlen(publish));
}

void Book::set_age(int age)
{
	Book::age = age;
}
void Book::set_pages(int pages)
{
	Book::pages = pages;
}

void Book::set_content(char** content, int content_size)
{
	for(int i = 0; i < content_size; ++i)
		memcpy(Book::content[i], content[i], strlen(content[i]));
}

void Book::get_book(Book b)
{
	cout << "Author: " << b.author << endl;
	cout << "Name: " << b.name << endl;
	cout << "Publishing house: " << b.publish << endl;
	cout << "Released in " << b.age << " year" << endl;
	cout << "Number of pages: " << b.pages << endl;
	cout << "Content: " << endl;
	for(int i = 0; i < content_size; ++i)
		cout << b.content[i] << endl;
}

void Book::clear(Book b)
{
	free(b.author);
	free(b.name);
	free(b.publish);
	for(int i = 0; i < content_size; ++i)
		free(b.content[i]);
	b.age = 0;
	b.pages = 0;
	b.content_size = 0;
}