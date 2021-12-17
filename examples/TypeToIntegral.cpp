//
// TypeToIntegral.cpp
//
// Created: 2021-12-16
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Trait/TypeToIntegral.hpp>
#include <iostream>

using namespace std;

int main(void)
{
	std::cout << Rr::Trait::TypeToInt<bool, char, int, void>::value << endl;
	std::cout << Rr::Trait::TypeIn<bool, char, int, void>::value << endl;
	std::cout << Rr::Trait::TypeToInt<void, char, int, void>::value << endl;

}
