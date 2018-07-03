#include <iostream>
#include "xbinder.hpp"
#include <string>
void show(int a ,double b,long* ptr)
{
    *ptr = 888888;
    std::cout<<a<<","<<b<<std::endl;
}
struct Test
{
    void show(int a)
    {
         std::cout<<"show==="<<a<<"str===="<<m_str<<std::endl;
    }
    std::string m_str="default";
};
int main()
{
    long* ptr = new long{};
    auto lam = [](int a,float b){std::cout<<b<<std::endl;};
    auto f = xmh::make_xbinder(lam,10,xmh::place_hold_1);
    f(2.03);
    auto f1 = xmh::make_xbinder(&show,xmh::place_hold_1,88.88,xmh::place_hold_2);
    f1(1024,ptr);
    std::cout<<*ptr<<std::endl;
    auto f2 = xmh::make_xbinder(&Test::show,Test{},10);
    f2();
    return 0;
}