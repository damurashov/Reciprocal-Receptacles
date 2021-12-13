//
// CallableTable.cpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

// #define RRO_STATIC_CAST_FN_CONVERSION 1
#include <iostream>
#include <Rr/Util/CallableTable.hpp>
#include <Rr/Trait/Sync.hpp>
#include <list>

using namespace std;

template <class Tsignature>
struct A {
};

void callMe(int, char)
{
	cout << "callMe()" << endl;
}

struct S {
	void callMe(int, char)
	{
		cout << "A::callMe()" << std::endl;
	}
	void callMe(int, char) const
	{
		cout << "A::callMe() const" << std::endl;
	}
};

void callableTable()
{
	S s;
	Rr::Util::CallableTable<void(int, char), std::list> ct{
		{callMe},
		{&S::callMe, &s}
	};

	ct.push_back(callMe);
	ct.emplace_back(callMe);

	ct.push_back({&S::callMe, (S *)&s});
	ct.emplace_back((void (S::*)(int, char))&S::callMe, (S *)&s);

	for (auto &callable : ct) {
		callable(42, 'l');
	}
}

void syncedCallableTable()
{
	const S s;
	Rr::Util::SyncedCallableTable<void(int, char) const, std::list, Rr::Trait::GroupMutSyncTypes<1>> sct {
		{callMe},
		{&S::callMe, &s}
	};

	sct.push_back(callMe);
	sct.emplace_back(callMe);

	sct.push_back({&S::callMe, &s});
	sct.emplace_back((void (S::*)(int, char)const)&S::callMe, &s);

	for (auto &callable : sct) {
		callable(42, 'l');
	}
}

int main(void)
{
	callableTable();
	syncedCallableTable();
}

