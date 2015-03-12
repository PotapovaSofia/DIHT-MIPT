#include "book.h"
#include "book.cpp"

class Works {
public:
	Works();
	Works(int size);
	~Works();
	void set(Works &a);
	void get(Works a);
	bool delete_book(int i);
	bool delete_book(char* name);
	bool add_book(Book &a);
private: 
	Book* work;
	int size;
	char* author;
	char* name;
}