//
// CallableTable.cpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <iostream>
#include <Rr/Util/CallableTable.hpp>
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

void callableTable()
{
	S s;
	Rr::Util::CallableTable<void(int, char), List> ct{{callMe}, {&S::callMe, &s}};
	ct.push_back(callMe);
	ct.emplace_back(callMe);

	for (auto &callable : ct) {
		callable(42, 'l');
	}
}

void syncedCallableTable()
{
	Rr::Util::SyncedCallableTable<void(int, char) const, std::list, Rr::Trait::TsyncMut<1>> obst;
	obst.emplace_back(callMe);
	cout << obst.back().kGroup << endl;
}

int main(void)
{
	callableTable();
	syncedCallableTable();
}

