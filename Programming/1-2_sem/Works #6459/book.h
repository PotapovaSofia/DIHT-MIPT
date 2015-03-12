#pragma once
class Book
{
private:
	char* author;
	char* name;
	char* publish;
	int age;
	int pages;
	char** content;
	int content_size;
public:
	Book();
	~Book();
	void set_author(char* author);
	void set_name(char* name);
	void set_publish(char* publish);
	void set_age(int age);
	void set_pages(int pages);
	void set_content(char** content, int content_size);
	void get_book(class Book b);
	void clear(class Book b);
};