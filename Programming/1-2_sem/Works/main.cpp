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
    void cout_name();
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
    cout << "\nNumber of copies: " << b_number_of_copies;
    cout << "\n";
}
void Book::cout_name()
{
    cout << "'" << name << "'";
}
bool operator==(const Book &a, const Book &b)
{
    if ((a.author == b.author)&&(a.contents == b.contents)&&(a.name == b.name)&&(a.year == b.year)&&(a.publish == b.publish)&&(a.page_count==b.page_count))
        return true;
    else return false;
}
class Works///: public Book
{
public:
    string name_of_work;
    string author;
    vector<Book> volumes;
    int count_of_volumes;
    int w_number_of_copies;

    Works();
    Works(const Works &t);
    ~Works();
    void cin_();
    void cout_();
    void add_book(const Book &t);
    void delete_book(const Book &t);
    friend bool operator==(const Works &a, const Works &b);

};
Works::Works()
{
    string name_of_work = "";
    string author = "";
    vector<Book> volumes;
    int count_of_volumes = 0;
    w_number_of_copies = 0;
}
Works::Works(const Works &t)
{
    name_of_work = t.name_of_work;
    author = t.author;
    volumes = t.volumes;
    count_of_volumes = t.count_of_volumes;
    w_number_of_copies = t.w_number_of_copies;
}
Works::~Works()
{

}
void Works::cin_()
{
    cout << "Write the author\n";
    string a;
    cin >> a;
    author = a;

    cout << "Write the name of work\n";
    string b;
    cin >> b;
    name_of_work = b;

}
void Works::cout_()
{
    cout << "\nWork:             " << name_of_work;
    cout << "\nAuthor:           " << author;
    cout << "\nCount of volumes: " << volumes.size();
    cout << "\nNumber of copies: " << w_number_of_copies;
    cout << "\nBooks: \n";
    for (int i = 0; i < volumes.size(); i++)
    {
        cout << i+1 << ".";                                                         ///   ????????????????????????????????????????
        cout << volumes[i].name;
        cout << "\n";
    }
}
void Works::add_book(const Book &t)
{
    volumes.push_back(t);
    count_of_volumes++;
}
void Works::delete_book(const Book &t)
{
    for (int i = 0; i < volumes.size(); i++)
    {
        if (t == volumes[i])
            volumes.erase(volumes.begin() + i);
    }
    count_of_volumes = volumes.size();
}
bool operator==(const Works &a, const Works &b)
{
    if ((a.author == b.author) && (a.name_of_work == b.name_of_work) && (a.volumes == b.volumes))
        return true;
    else return false;
}
int main()
{
    Works w = Works();
    w.cin_();
    cout << "How many books do you want to add?\n";
    int l;
    cin >> l;
    for (int i = 0; i< l; i++)
    {
        Book k = Book();
        k._cin();
        w.add_book(k);
    }
    w.cout_();

    return 0;
}
