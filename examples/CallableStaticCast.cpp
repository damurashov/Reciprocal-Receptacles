//
// CallableStaticCast.cpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//


// Use static_cast for method pointers conversion
// Requires inheritance from Rr::RrObject
#define RRO_STATIC_CAST_FN_CONVERSION 1

#include <iostream>
#include <Rr/Util/Callable.hpp>

using namespace std;


struct Base : Rr::RrObject {
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

struct A : Rr::RrObject {
	void callMe(int a)
	{
		std::cout << "A::callMe: " << a << endl;
	}
	void callMe(int a) const
	{
		std::cout << "A::callMe (const): " << a << endl;
	}
};

void callMe(int a)
{
	std::cout << "callMe: " << a << endl;
}

int main(void)
{
	{
		using ConstCallable = Rr::Util::Callable<void(int)const>;

		ConstCallable ca{callMe};

		const A a;
		ConstCallable ca2{&A::callMe, &a};

		ca(42);
		ca2(42);
	}

	{
		using Callable = Rr::Util::Callable<void(int)>;

		Callable ca{callMe};
		A a;
		Callable ca2{&A::callMe, &a};
		Base base;
		Callable ca3{&Base::call, &base};
		Derived derived;
		Callable ca4{&Base::call, &derived};

		ca(42);
		ca2(42);
		ca3(42);
		ca4(42);
	}
}
