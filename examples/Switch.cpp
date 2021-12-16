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

void accept(int)
{
	cout << "int" << endl;
}

enum Types {
	Bool = 1,
	Char,
	String,
	Echo,
};

template <int ...Ia>
struct Ilist;

template <Types ...>
struct ItList;

template <class ...Ta>
struct Tlist;

int main()
{

	typename Rr::Trait::SwitchInt<-1, Ilist<1, 2, 3>, Tlist<bool, char, std::string, int>>::Type t1{};
	typename Rr::Trait::SwitchInt<2, Tlist<bool, char, std::string>, Ilist<1, 2, 3>>::Type t2{};
	typename Rr::Trait::Switch<Types, String, ItList<Bool, Char, String>, Tlist<bool, char, std::string>>::Type t3{};
	typename Rr::Trait::Switch<Types, Bool, Tlist<bool, char, std::string>, ItList<Bool, Char, String>>::Type t4{};
	typename Rr::Trait::Switch<Types, Echo, Tlist<bool, char, std::string, int>, ItList<Bool, Char, String>>::Type t5{};

	accept(t1);
	accept(t2);
	accept(t3);
	accept(t4);
	accept(t5);
}
