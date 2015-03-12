#include <iostream>
#include <cstring>
#include <string.h>
#include <stdio.h>

using namespace std;
class String_mine
{
public:
	int len;
	char* buf;
	int capacity;

	String_mine();
	String_mine(const string& s);
	String_mine(const char* str);
	String_mine(const String_mine& str);
	String_mine& operator=(const String_mine& src);
	~String_mine();
	int GetLength();
	int GetCapacity();
	const char* GetString()const;
	void DeleteInside(int index);
	void PushInside(char* str,int index);
	void DeleteLast(int index);
	void PushLast(char* str);
	void PushBack(char* s);
	char& operator[](int x);
	friend bool operator ==(const String_mine& str1,const String_mine& str2);
	friend bool operator !=(const String_mine& str1,const String_mine& str2);
	friend bool operator >(const String_mine& str1,const String_mine& str2);
	friend bool operator <(const String_mine& str1,const String_mine& str2);
	friend bool operator <=(const String_mine& str1,const String_mine& str2);
	friend bool operator >=(const String_mine& str1,const String_mine& str2);
	String_mine operator+(const String_mine& s);
	//String_mine& operator+=(const String_mine& str);
	friend ostream& operator<< (ostream& ostr,const String_mine& str);
	friend istream& operator>> (istream& istr,String_mine& str1);
};

String_mine::String_mine()
{
	len=0;
	capacity=16;
	buf=new char[capacity];
	buf[len]='\0';
}
String_mine::String_mine(const string& str):capacity(str.capacity()),len(str.length())
{
	buf = new char[capacity];
	strcpy(buf,str.data());
}
String_mine::String_mine(const char* str)
{
	len=strlen(str);
	capacity=2*strlen(str);
	buf = new char[capacity];;
	strcpy(buf,str);
	buf[len]='\0';
}
String_mine::String_mine(const String_mine &str)
{
	len=str.len;
	capacity=str.capacity;
	buf = new char[capacity];
	strcpy(buf,str.buf);
	buf[len]='\0';
}
String_mine& String_mine::operator =(const String_mine &src)
{
	if (this==&src)
		return (*this);
	len=src.len;
	capacity=src.capacity;
	delete buf;
	buf = new char[capacity];
	strcpy(buf,src.buf);
	buf[len]='\0';
	return *this;
}
String_mine::~String_mine()
{
	delete buf;
}
int String_mine::GetLength()
{
	return len;
}
int String_mine::GetCapacity()
{
	return capacity;
}
const char* String_mine::GetString()const
{
	return buf;
}
void String_mine::DeleteInside(int index) ///удаление из середины
{
	if (index>=len)
		return;
	memmove(&buf[index],&buf[index+1],sizeof(char)*(len-index-1));
	len--;
	buf[len]='\0';
}
void String_mine::PushInside(char* str,int index) ///вставка в середину
{
	String_mine S;
	int k = strlen(str);
	S.buf = new char[capacity*2];
	memcpy(&S.buf[0],&buf[0],index);
	memcpy(&S.buf[index],&str[0],k);
	memcpy(&S.buf[index+k],&buf[index],len-index);
	len+=k;
	capacity*=2;
	S.buf[len]='\0';
	strcpy(buf,S.buf);
}
void String_mine::DeleteLast(int index)///удаление с конца
{
	len-=index;
	//capacity++;
	buf[len]='\0';

}
void String_mine::PushBack(char *str) ///вставка в конец
{
	len+=strlen(str);
	capacity*=2;
	String_mine S;
	S.capacity = capacity*2;
	S.buf = new char[S.capacity];
	strcpy(S.buf,buf);
	buf = new char[S.capacity];
	strcat(S.buf,str);
	strcpy(buf,S.buf);
}
char& String_mine::operator [](int x)
{
	if (x>=0)
		return (buf[x]);
	else
		{
            cout << "Error\n";
		    return (buf[len+x]);
		}

}
bool operator ==(const String_mine& str1,const String_mine& str2)
{
	return strcmp(str1.buf,str2.buf)==0;
}
bool operator !=(const String_mine& str1,const String_mine& str2)
{
	return !(str1==str2);
}
bool operator >(const String_mine& str1,const String_mine& str2)
{
	if (strcmp(str2.buf,str1.buf) > 0)
        return true;
    else
        return false;
}
bool operator <(const String_mine& str1,const String_mine& str2)
{
	if (strcmp(str1.buf,str2.buf) < 0)
        return true;
    else
        return false;
}
bool operator <=(const String_mine& str1,const String_mine& str2)
{
	return !(str2>str1);
}
bool operator >=(const String_mine& str1,const String_mine& str2)
{
	return !(str1<str2);
}
String_mine String_mine::operator+(const String_mine& s)
{
	String_mine str1(*this);
	String_mine str2 = s;
	String_mine str = String_mine();
	if (str1.capacity>=str2.capacity)
		str.capacity = 2*str1.capacity;
	else
		str.capacity = 2*str2.capacity;
	str.len = str1.len + str2.len;
	str.buf = new char[str.capacity];
	strcpy(str.buf,str1.buf);
	strcat(str.buf,str2.buf);
	return str;
}
ostream& operator<<(ostream& ostr,const String_mine& str1)
{
	ostr<< str1.buf;
	return (ostr);
}
istream& operator>>(istream& istr,String_mine& str)
{
	istr>> str.buf;
	str.len = strlen(str.buf);
	return (istr);
}
int main()
{
	cout << "Write the string\n";
	String_mine str = String_mine();
	cin >> str;

	cout << "What letter do you want to delete?\n";
	int i;
	cin >> i;
	str.DeleteInside(i-1);
	cout << str <<endl;

	cout << "How man letters do you want to delete in the end?\n";
	int k;
	cin >> k;
	if (k != 0)
        {
            str.DeleteLast(k);
            cout << str << endl;
        }

    cout << "In what position do you want to add a letter?\n";
    cout << "Position:\n";
    int j;
    cin >> j;
    str.PushInside("str", j-1);
    cout << str << endl;

    cout << "Do you want to add string in the end?\n";
    //char* c;
    //cin >> c;
    str.PushBack("STR");
    cout << str << endl;

    cout << "Write the second string\n";
	String_mine str1 = String_mine();
	String_mine str2 = String_mine();
	cin >> str2;
	str1 = str + str2;
	cout << str1 << endl;

	return 0;
}
