#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <string.h>
#include <cmath>
#include <string>
#include <cstdlib>

using namespace std;
class Book
{
public:
    string author;
    string name;
    string publish;
    string contents;
    string year;
    string page_count;
    int b_number_of_copies;

    Book();
    Book(const Book &t);
    ~Book();
    void _cin();
    void _cout();
    friend bool operator==(const Book &a, const Book &b);
};
Book::Book()
{
    string author = "";
    string name = "";
    string publish = "";
    string contents = "";
    string year = "";
    string page_count = "";
    b_number_of_copies = 0;
}
Book::Book(const Book &t)
{
    author = t.author;
    name = t.name;
    publish = t.publish;
    contents = t.contents;
    year = t.year;
    page_count = t.page_count;
    b_number_of_copies = t.b_number_of_copies;
}
Book::~Book()
{

}
void Book::_cin()
{
    cout << "Write the author\n";
    string a;
    cin >> a;
    author = a;

    cout << "Write the name\n";
    string b;
    cin >> b;
    name = b;

    cout << "Write the Publish house\n";
    string c;
    cin >> c;
    publish = c;

    cout << "Write the contents\n";
    string d;
    cin >> d;
    contents = d;

    cout << "Write the year\n";
    string e;
    cin >> e;
    year = e;

    cout << "Write the count of pages\n";
    string f;
    cin >> f;
    page_count = f;
}
void Book::_cout()
{
    cout << "Author:           " << author;
    cout << "\nName:             " << name;
    cout << "\nPublish house:    " << publish;
    cout << "\nContents:         " << contents;
    cout << "\nYear:             " << year;
    cout << "\nCount of pages:   " << page_count;
   /// cout << "\nNumber of copies: " << b_number_of_copies;
    cout << "\n";
}
bool operator==(const Book &a, const Book &b)
{
    if ((a.author == b.author)&&(a.contents == b.contents)&&(a.name == b.name)&&(a.year == b.year)&&(a.publish == b.publish)&&(a.page_count==b.page_count))
        return true;
    else return false;
}
int main()
{
    Book B = Book();
    B._cin();
    B._cout();
    return 0;
}
