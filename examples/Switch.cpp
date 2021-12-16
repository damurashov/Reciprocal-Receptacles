//
// Switch.cpp
//
// Created: 2021-12-16
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Trait/Switch.hpp>
#include <iostream>
#include <string>

using namespace std;

void accept(bool)
{
	cout << "bool" << endl;
}

void accept(char)
{
	cout << "char" << endl;
}

void accept(std::string)
{
	cout << "string" << endl;
}

enum Types {
	Bool,
	Char,
	String
};

int main()
{
	typename Rr::Trait::Switch<Types, String, void(bool, char, std::string), Bool, Char, String>::Type t{};
	typename Rr::Trait::SwitchInt<1, void(bool, char, std::string), 0, 1, 2>::Type t2{};
	accept(t);
	accept(t2);
}
