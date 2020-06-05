#include<iostream>
#include<type_traits>

class A{};
class B:A{};  
class C{};
int main(){
    std::cout<<std::is_base_of<A,B>::value<<std::endl;
    std::cout<<std::is_base_of<B,A>::value<<std::endl;
    std::cout<<std::is_base_of<C,B>::value<<std::endl;
}

//OUTPUT 
//1
//0
//0