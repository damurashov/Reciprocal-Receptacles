//
// IntegraIn.cpp
//
// Created: 2021-12-16
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Trait/IntegralIn.hpp>
#include <iostream>

enum Integral {
	One,
	Two,
};

int main(void)
{
	std::cout << Rr::Trait::IntToPos<8, 1, 2, 9>::value << std::endl;
	std::cout << Rr::Trait::IntToPos<8, 1, 2, 8>::value << std::endl;
	std::cout << Rr::Trait::IntToPos<8, 1, 8, 8>::value << std::endl;
	std::cout << Rr::Trait::IntIn<42, 1, 2, 3>::value << std::endl;
	std::cout << Rr::Trait::IntegralIn<Integral, (Integral)42, One, Two>::value << std::endl;
}
