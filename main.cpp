#include <iostream>
#include "MyString.h"


int main() {

MyString a (4);
MyString b (6);
MyString c (5);

a.Add("01234567890123456789");
b.Add("xxx");
c.Add("");


//b = a.copy(9,11,6);
a.searchReplase(c,b);
// b = b + b;
cout << a << endl;




//a = "mama mila " + a + " mama mila";
//b.Add("mama mila");
//// нельзя совмещать функцию Add и присваивание, тк присваинваие содержит деструктор
//c=b;
//
//cout << a <<endl;
//cout << b <<endl;
//cout << c <<endl;
//
//cout << a <<endl;
////a.searchReplase(b,c);
////a.searchReplase(b,a);
//a.searchReplase(b,b);// ???
//
//
//cout << a;


    return 0;
}
