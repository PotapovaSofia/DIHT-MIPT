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
    cout << "\nNumber of copies: " << b_number_of_copies;
    cout << "\n";
}
bool operator==(const Book &a, const Book &b)
{
    if ((a.author == b.author)&&(a.contents == b.contents)&&(a.name == b.name)&&(a.year == b.year)&&(a.publish == b.publish)&&(a.page_count==b.page_count))
        return true;
    else return false;
}
class Works: public Book
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

    cout << "Write the count of volumes";
    int c;
    cin >> c;
    count_of_volumes = c;

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
class Journal
{
public:
    string name;
    string period;
    int j_number_of_copies;

    Journal();
    Journal(const Journal &t);
    ~Journal();
    void j_cin();
    void j_cout();
    friend bool operator==(const Journal &a, const Journal &b);;
};
Journal::Journal()
{
    string name = "";
    string period = "";
    j_number_of_copies = 0;
}
Journal::Journal(const Journal &t)
{
    name = t.name;
    period = t.period;
    j_number_of_copies = t.j_number_of_copies;
}
Journal::~Journal()
{

}
void Journal::j_cin()
{
    cout << "Write the name of journal:\n";
    string a;
    cin >> a;
    name = a;

    cout << "Write the period of journal\n";
    string b;
    cin >> b;
    period = b;
}
void Journal::j_cout()
{
    cout << "Name of Journal: " << name << "\n";
    cout << "Period:          " << period << "\n";
    cout << "Number of copies: " << j_number_of_copies;
    cout << "\n";
}
bool operator==(const Journal &a, const Journal &b)
{
    if ((a.name == b.name) && (a.period == b.period))
        return true;
    else return false;
}
class Library
{
public:
    vector<Book> books;
    vector<Works> works;
    vector<Journal> journals;

    Library();
    ~Library();
    void add_book(const Book &t);
    void add_works(const Works &t);
    void add_journal(const Journal &t);
    void lib_find_and_give();
    void lib_get();
    void lib_delete();
    void lib_look();

};
Library::Library()
{
    vector<Book> books;
    vector<Book> books_on_hands;
    vector<Works> works;
    vector<Works> works_on_hands;
    vector<Journal> journals;
    vector<Journal> journals_on_hands;
}
Library::~Library()
{

}
void Library::add_book(const Book &t)
{
    Book B = t;
    bool flag = false;
    for (int i = 0; i < books.size(); i++)
    {
        if (books[i] == B)
        {
            flag = true;
            books[i].Book::b_number_of_copies++;
        }
    }
    if (flag == false)
    {
        books.push_back(t);
        books[books.size()-1].b_number_of_copies++;
    }
}
void Library::add_works(const Works &t)
{
    Works W = t;
    bool flag = false;
    for (int i = 0; i < works.size(); i++)
    {
        if (works[i] == W)
        {
            flag = true;
            works[i].w_number_of_copies++;
        }
    }
    if (flag == false)
    {
        works.push_back(W);
        works[works.size()-1].w_number_of_copies++;
    }
}
void Library::add_journal(const Journal &t)
{
    Journal J = t;
    bool flag = false;
    for (int i = 0; i < journals.size(); i++)
    {
        if (journals[i] == J)
        {
            flag = true;
            journals[i].j_number_of_copies++;
        }
    }
    if (flag == false)
    {
        journals.push_back(J);
        journals[journals.size()-1].j_number_of_copies++;
    }
}
void Library::lib_find_and_give()
{
    Library lib = *this;
    cout << "What do yo want to get: book, journal or works?";
    string a, b, c, d, e, answer;
    cin >> a;
    if (a == "book")
    {
        vector<Book> B = lib.books;
        cout << "What information do you know about the book?\n";
        cout << "Author:  \n";
        cin >> c;
        if (c != "-")
        {
            for(int i = 0; i < B.size(); i++)
            {
                if(B[i].author != c)
                   B.erase(B.begin()+i);
            }
        }
        cout << "Name:  \n";
        cin >> b;
        if (b != "-")
        {
            for(int i = 0; i < B.size(); i++)
            {
                if(B[i].name != b)
                   B.erase(B.begin()+i);
            }

        }
        cout << "Year:  \n";
        cin >> d;
        if (d != "-")
        {
            for(int i = 0; i < B.size(); i++)
            {
                if(B[i].year != d)
                   B.erase(B.begin()+i);
            }
        }
        cout << "Publish House:  \n";
        cin >> e;
        if (e != "-")
        {
            for(int i = 0; i < B.size(); i++)
            {
                if(B[i].publish != e)
                   B.erase(B.begin()+i);
            }
        }
        if (B.size() == 0)
            cout << "Sorry, we haven't this book";
        else
        {
            for (int i = 0; i < B.size(); i++)
            {
                cout << "Book #" << i+1 << "\n";
                B[i].Book::_cout();
                cout << "\n";
            }
            cout << "Do you want to get one of this books?\n";
            cin >> answer;
            if (answer == "yes")
            {
                cout << "Which one?(#)\n";
                int n;
                cin >> n;
                n--;
                if (B[n].b_number_of_copies == 0)
                    cout << "Sorry, this book has been taken\n";
                else
                {
                    cout << "Get your book\n";
                    B[n].b_number_of_copies--;
                }
            }
            if ((answer != "yes") && (answer != "no"))
                cout << "Error\n";
        }

    }
    else if (a == "journal")
    {
        vector<Journal> J = lib.journals;
        cout << "What information do you know about the journal?\n";
        cout << "Name:  \n";
        cin >> b;
        if (b != "-")
        {
            for(int i = 0; i < J.size(); i++)
            {
                if(J[i].name != b)
                   J.erase(J.begin()+i);
            }

        }
        cout << "Period:  \n";
        cin >> c;
        if (c != "-")
        {
            for(int i = 0; i < J.size(); i++)
            {
                if(J[i].period != c)
                   J.erase(J.begin()+i);
            }
        }
        if (J.size() == 0)
            cout << "Sorry, we haven't this journal";
        else
        {
            for (int i = 0; i < J.size(); i++)
            {
                cout << "Journal #" << i+1 << "\n";
                J[i].Journal::j_cout();
            }
            cout << "Do you want to get one of this journals?\n";
            cin >> answer;
            if (answer == "yes")
            {
                cout << "Which one?(#)\n";
                int n;
                cin >> n;
                n--;
                if (J[n].j_number_of_copies == 0)
                    cout << "Sorry, this journal has been taken\n";
                else
                {
                    cout << "Get your journal\n";
                    J[n].j_number_of_copies--;
                }
            }
            if ((answer != "yes") && (answer != "no"))
                cout << "Error\n";
        }

    }
    else if (a == "works")
    {
        vector<Works> W = lib.works;
        cout << "What information do you know about the work?\n";
        cout << "Author:  \n";
        cin >> c;
        if (c != "-")
        {
            for(int i = 0; i < W.size(); i++)
            {
                if(W[i].author != c)
                   W.erase(W.begin()+i);
            }
        }
        cout << "Name:  \n";
        cin >> b;
        if (b != "-")
        {
            for(int i = 0; i < W.size(); i++)
            {
                if(W[i].name_of_work != b)
                   W.erase(W.begin()+i);
            }

        }
        cout << "Count of volumes:  \n";
        cin >> d;
        if (d != "-")
        {
            for(int i = 0; i < W.size(); i++)
            {
                if(W[i].volumes.size() != atoi(d.c_str()))
                   W.erase(W.begin()+i);
            }
        }
        if (W.size() == 0)
            cout << "Sorry, we haven't this journal";
        else
        {
            for (int i = 0; i < W.size(); i++)
            {
                cout << "Work #" << i+1 << "\n";
                W[i].Works::cout_();
            }
            cout << "Do you want to get one of this works?\n";
            cin >> answer;
            if (answer == "yes")
            {
                cout << "Which one?(#)\n";
                int n;
                cin >> n;
                n--;
                if (W[n].w_number_of_copies == 0)
                    cout << "Sorry, this works has been taken\n";
                else
                {
                    cout << "Get your works\n";
                    W[n].w_number_of_copies--;
                }
            }
            if ((answer != "yes") && (answer != "no"))
                cout << "Error\n";
        }
    }
    else
        cout << "Error\n";
}
void Library::lib_get()
{
    Library lib = *this;
    cout << "What do you want to pass? book/journal/work\n";
    string a;
    cin >> a;
    if (a == "book")
    {
        Book B = Book();
        B._cin();
        for (int i = 0; i < lib.books.size(); i++)
        {
            if (B == lib.books[i])
            {
                cout << "Thank you. Your book is passed.\n";
                lib.books[i].b_number_of_copies++;
            }
            else
                cout << "This book isn't registered in our library\n";
        }
    }
    else if (a == "journal")
    {
        Journal J = Journal();
        J.j_cin();
        for (int i = 0; i < lib.journals.size(); i++)
        {
            if (J == lib.journals[i])
            {
                cout << "Thank you. Your journal is passed.\n";
                lib.journals[i].j_number_of_copies++;
            }
            else
                cout << "This journal isn't registered in our library\n";
        }
    }
    else if (a == "work")
    {
        Works W = Works();
        W.cin_();
        for (int i = 0; i < lib.works.size(); i++)
        {
            if (W == lib.works[i])
            {
                cout << "Thank you. Your work is passed.\n";
                lib.works[i].w_number_of_copies++;
            }
            else
                cout << "This work isn't registered in our library\n";
        }
    }
    else
        cout << "Error\n";
}
void Library::lib_delete()
{
    Library lib = *this;
    cout << "What do you want to delete? book/journal/work\n";
    string answer;
    cin >> answer;
    if (answer == "book")
    {
        cout << "What book do you want to delete?\n";
        Book B = Book();
        B._cin();
        bool flag = false;
        for (int i = 0; i < lib.books.size();i++)
        {
            if (B == lib.books[i])
            {
                lib.books.erase(lib.books.begin()+i);
                flag = true;
            }
        }
        if (flag == false)
            cout << "This book isn't registered in our library";
    }
    else if (answer == "journal")
    {
        cout << "What journal do you want to delete?\n";
        Journal J = Journal();
        J.j_cin();
        bool flag = false;
        for (int i = 0; i < lib.journals.size();i++)
        {
            if (J == lib.journals[i])
            {
                lib.journals.erase(lib.journals.begin()+i);
                flag == true;
            }
        }
        if (!flag)
            cout << "This journal isn't registered in our library";
    }
    else if (answer == "work")
    {
        cout << "What work do you want to delete?\n";
        Works W = Works();
        W.cin_();
        bool flag = false;
        for (int i = 0; i < lib.works.size();i++)
        {
            if (W == lib.works[i])
            {
                lib.works.erase(lib.works.begin()+i);
                flag == true;
            }
        }
        if (!flag)
            cout << "This work isn't registered in our library";
    }
    else
        cout << "Error\n";
}
void Library::lib_look()
{
    Library lib = *this;
    cout << "\n";
    cout << "Books: \n\n";
    for (int i = 0; i < lib.books.size(); i++)
    {
        cout << "Book #" << i+1 << "\n";
        lib.books[i]._cout();
    }
    cout << "\n";
    cout << "Journals: \n\n";
    for (int i = 0; i < lib.journals.size(); i++)
    {
        cout << "Journal #" << i+1 << "\n";
        lib.journals[i].j_cout();
    }
    cout << "\n";
    cout << "Works: \n\n";
    for (int i = 0; i < lib.works.size(); i++)
    {
        cout << "Work #" << i+1 << "\n";
        lib.works[i].cout_();
    }
    cout << "\n\n";
}

int main()
{
    Library L = Library();

    cout << "\nAdd the book: \n";
    Book B = Book();
    B._cin();
    L.add_book(B);

    cout << "\nAdd the work: \n";
    Works W = Works();
    W.cin_();
    L.add_works(W);


    cout << "\nAdd the journal: \n";
    Journal J = Journal();
    J.j_cin();
    L.add_journal(J);

    L.lib_look();

    L.lib_find_and_give();

    L.lib_get();

    L.lib_delete();

    L.lib_look();


    return 0;
}

