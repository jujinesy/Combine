/**----------------------------------------------------------------------------
 * _test_boost_bind.cpp
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh,Yonghwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 2014:1:25 14:45 created
**---------------------------------------------------------------------------*/
#include "stdafx.h"

/**
* @brief	boost::bind() - part 1
*/
int f(int a, int b)
{
    return a+b;
}

int g(int a, int b, int c)
{
    return a+b+c;
}

bool boost_bind()
{
    std::cout << boost::bind(f, 1, 2)() << std::endl;           // f(1,2) �� ������ �Լ���ü
    std::cout << boost::bind(g, 1,2,3)() << std::endl;          // g(1,2,3) �� ������ �Լ���ü

    // boost::function �� ����ϸ� bind() �� �Լ���ü�� ������ �� �ִ�!!
    //
	boost::function<int (int, int)> ff = boost::bind(f, 1, 2);
    std::cout	<< "ff(1,2) = "					<< ff(1,2) 
				<< "boost::bind(f, 1, 2)() = "	<< boost::bind(f, 1, 2)() 
				<< std::endl;

	// <¯ �߿�!!>
	// f �Լ��� int(int, int) Ÿ��������
	// boost::bind(f, 1, _1) �� �Ķ���͸� �����ؼ� f(val) ���·� ȣ���� �����ϰ�, 
	// �׻� (1+ val) �� �����ϰ� �ȴ�. �̷� Ư¡�� �̿��ؼ� callback �Լ��� �ΰ����� 
	// �Ķ���͸� �����ϰų�, �Ķ���� ������ �����ϴ� ���� ���� �� �� �ִ�.
	boost::function<int (int, int)> xf = boost::bind(f, 1, _1);
    std::cout	<< "xf(1, 2) = "					<< ff(1, 2) 
				<< "boost::bind(f, 1, _1)(2) = "	<< boost::bind(f, 1, _1)(2) 
				<< std::endl;


    int x = 3;
    int y = 4;
    int z = 5;

    assert( f(y, x) == bind(f, _2, _1)(x,y) );
    assert( g(x, 9, x) == bind(g, _1, 9, _1)(x) );
    assert( g(z,z,z) == bind(g, _3, _3, _3)(x,y,z) );
    assert( g(x,x,x) == bind(g, _1, _1, _1)(x,y,z) );

	return true;
}










/**
* @brief	boost::bind() - part 2 �Լ���ü ���ε�
*/
struct F
{
    int operator()(int a, int b) {return a-b;}
    bool operator()(long a, long b) {return a=b ? true : false;}
};

bool boost_bind2()
{
    F f;

    int x = 104;
    assert( 0 == boost::bind<int>(f, _1, _1)(x) );            // f(x, x)    

    long y = 104;
    assert( true == boost::bind<bool>(f, _1, _1)(y) );            // f(x, x)
    assert( true == boost::bind( boost::type<bool>(), f, _1, _1)(y) );

	return true;
}


/**
* @brief    boost::bind() - part 3 �Լ���ü ���ε�, �Լ� ��ü ���� ó��
            - ���� ������ �ִ� ��� result_type �� ������ ���ϸ� ��������?
            - ���¸� �����ϰ� ���� ��� boost::ref() �� ���� ���۷����� �Ѱܾ� �� 
            - ����Ʈ�� �Լ���ü�� bind() �� �����ؼ� ������ ����
*/
struct FF
{
    int s;
    typedef void result_type;
    void operator() (int x) { s += x; }
};

bool boost_bind3()
{
    FF f={0};
    int a[] = {1,2,3};

    std::for_each(a, a+3, boost::bind( boost::ref(f), _1) );
    assert( f.s == 6 );

	return true;
}



/**
* @brief	boost::bind() - part 4 Ŭ���� �޼ҵ� ���ε�
*/
typedef class A
{
private:    
    int m;
public:
    A(int val) : m(val) {}

    int add_value(int val) 
    { 
        m += val;
        std::cout << __FUNCTION__ << ", m=" << m << std::endl;
        return m;
    }
    
    void start(int v) 
    { 
        std::cout << __FUNCTION__ << ", m=" << m << ", v=" << v << std::endl;
    };
    
}*PA;


bool boost_bind4()
{
    std::vector<int> vint;
    for(int i = 0; i < 12; ++i)
    {
        vint.push_back(i);
    }

    /* 
    - class method ���ε�
    - �Ʒ� �ڵ�� ����
        std::vector<int>::iterator it = vint.begin();
        for(; vint.end(); ++it)
        {
            a.add_value(*it);
        }
    */
    A a(100);
    boost::function <int (int)> functor_add_value = boost::bind( &A::add_value, a, _1);
    std::for_each(vint.begin(), vint.end(), functor_add_value);
    //std::for_each(vint.begin(), vint.end(), boost::bind( &A::add_value, a, _1));
    
    
    /*    
    - class method ���ε��Ҷ��� ù��° �Ķ���Ͱ� class instance �̾�� ��
    - std::vector<PA> �϶��� std::vector<A> �϶��� functor �� �� ������

    - �Ʒ� �ڵ�� ������
        std::vector<PA>::iterator it = vA.begin();
        for(; it != vA.end(); ++it)
        {
            it->start();
        }

    - std::for_each( ) �� [ vA.begin(), vA.end() ) �� boost::bind(&A::start, _1)(it) �� ȣ���̹Ƿ�
      it::start() �� ������
    */
    std::vector<PA> vA;
    for (int i = 0; i < 10; ++i)
    {
        PA pa = new A(i);
        vA.push_back(pa);
    }
    
    int value = 0;
    std::for_each(vA.begin(), vA.end(), boost::bind(&A::start, _1, ++value) );    

	return true;
}

//=============================================================================
// boost bind �� �̿��� �ݹ��Լ� ó�� ����
// �Ϲ� �Լ� �����͸� �����ϸ� boost::bind() �� ó���� �ȵ�
// boost::function ���� �������־�� ��
// 

class A5
{
public:
    void print(const std::string &s) 
	{
        std::cout << s << std::endl;
    }
};


typedef boost::function<void()> callback;

class B5
{
public:
    void set_callback(callback cb) 
	{
        m_cb = cb;
    }

    void do_callback() 
	{
        m_cb();
    }

private:
    callback m_cb;
};

void regular_function() 
{
    std::cout << "regular!" << std::endl;
}

bool boost_bind5()
{
    A5 a;
    B5 b;
    std::string s("message");

    // you forget the "&" here before A::print!
    b.set_callback(boost::bind(&A5::print, &a, s));
    b.do_callback();

    // this will work for regular function pointers too, yay!
    b.set_callback(regular_function);
    b.do_callback();

	return true;
}