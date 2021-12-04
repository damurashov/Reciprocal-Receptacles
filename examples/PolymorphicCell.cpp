//
// PolymorphicCell.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Util/CallbackTable.hpp>
#include <Rr/Util/DefaultConfig.hpp>
#include <Rr/Util/PolymorphicCell.hpp>
#include <iostream>
#include <string>
#include <list>

struct A {
	void callMe(char c)
	{
		std::cout << c << std::endl;
	}

	A()
	{
		std::cout << "creating A" << std::endl;
	}

	virtual void saySomething() = 0;
	virtual ~A() = default;
};

struct B : A {
	virtual void saySomething()
	{
		std::cout << "something B" << std::endl;
	}

	B()
	{
		std::cout << "creating B" << std::endl;
	}

	char a[12];
};

struct C : A {
	C()
	{
		std::cout << "creating C" << std::endl;
	}

	void saySomething() override
	{
		std::cout << "something C" << std::endl;
	}
};

void makeCallable()
{
}

void apply(A& a)
{
	a.saySomething();
}

int main(void)
{
	// std::cout << Rr::Util::MaxSizeof<long, int, char, char>::value << std::endl;
	// std::cout << sizeof(int) << std::endl;

	using Pc = Rr::Util::PolymorphicCell<A, B>;

	std::cout << sizeof(B) << std::endl;
	std::cout << Pc::size() << std::endl;

	std::list<Pc> list;
	list.emplace_back(Pc::ptr<C>());
	list.emplace_back(Pc::ptr<B>());

	for (auto &instance : list) {
		apply(instance);
	}
}
