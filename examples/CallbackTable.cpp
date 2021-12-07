//
// CallbackTable.cpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <iostream>
#include <Rr/Util/CallbackTable.hpp>
#include <list>

using namespace std;

template <class Tsignature>
struct A {
};

template <typename T1, typename ...T>
using List = std::list<T1, T...>;

void callMe(int, char)
{
	cout << "callMe()" << endl;
}

struct S {
	void callMe(int, char)
	{
		cout << "A::callMe()" << std::endl;
	}
};

int main(void)
{
	S s;
	Rr::Util::CallbackTable<void(int, char), List> ct{{callMe}, {&S::callMe, &s}};

	for (auto &callable : ct) {
		callable(42, 'l');
	}
}
