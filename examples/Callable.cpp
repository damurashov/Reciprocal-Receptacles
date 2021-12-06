//
// Callable.cpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <iostream>
#include <Rr/Util/Callable.hpp>

using namespace std;

struct Base {
	virtual void call(int a)
	{
		std::cout << "Base::call: " << a << endl;
	}

	virtual ~Base() = default;
};

struct Derived : Base {
	void call(int a) override {
		std::cout << "Derived::call: " << a << endl;
	}
};

struct A {
	void callMe(int a)
	{
		std::cout << "A::callMe: " << a << endl;
	}
};

void callMe(int a)
{
	std::cout << "callMe: " << a << endl;
}

int main(void)
{
	using Callable = Rr::Util::Callable<void(int)>;

	Base base;
	Derived derived;
	A a;

	Callable cBase{&Base::call, &base};
	Callable cDerived{&Base::call, (Base *)&derived};
	Callable cA{&A::callMe, &a};
	Callable cStatic{callMe};

	cBase(42);
	cDerived(42);
	cA(42);
	cStatic(42);

	cout << Callable::kConst << endl;
}
