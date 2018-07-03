#include <iostream>
#include "xbinder.hpp"
void show(int& a, double& b)
{
	std::cout << "a===" << a <<","<< "b===" << b << std::endl;
	b = 1023.023;
}

struct Test
{
	void show(int& a)
	{
		a = 88888;
		m_abc = 233333;
	}
	int m_abc = 0;
};
void same_test(int a, int b, int c)
{
	std::cout << a << "," << b << "," << c << std::endl;
}
int main()
{

	auto f0 = xmh::make_xbinder([](int& a) { std::cout << a << std::endl; a = 1024; }, xmh::place_hold_1);
	int value0 = 10;
	f0(value0);
	std::cout << "value0===" << value0 << std::endl;
	auto f1 = xmh::make_xbinder(&show, 10, xmh::place_hold_1);
	double value1 = 20.1;
	f1(value1);
	std::cout << value1 << std::endl;
	Test t;
	auto f2 = xmh::make_xbinder(&Test::show, &t, xmh::place_hold_1);
	int value2 = 1024;
	f2(value2);
	std::cout << value2 << std::endl;
	std::cout << t.m_abc << std::endl;

	auto f3 = xmh::make_xbinder(&same_test, 0, 1,2);
	f3
	();
	//std::cout << typeid(xmh::make_index<1, 3>::type).name() << std::endl;
	std::cin.get();
	return 0;
}
