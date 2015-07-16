//#include <stdio.h>
//#include <iostream>
//using namespace std;
//
//int sum(int a, int b)
//{
//	return a + b;
//}
//
//int a1()
//{
//	int (*pf)(int,int) = sum;
//
//	cout << "pf(int,int): " << pf(4, 5) << endl;
//	cout << "sum(int,int): " << sum(4, 5) << endl;
//	cout << "pf address = " << pf << endl;
//	cout << "sum address = " << sum << endl;
//	return 0;
//}
#include<stdio.h>
//extern void a1();

class MyClass
{
public:
	MyClass();
	~MyClass();
	int sum(int, int);
private:

};

MyClass::MyClass()
{
	//	int (*pf)(int,int) = &sum;
}

MyClass::~MyClass()
{
}

int MyClass::sum(int a, int b)
{
	return a + b;
}