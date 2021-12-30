//
// Fn.cpp
//
// Created on: 2021-12-26
//     Author: dmtr <DOT> murashov <AT> <GMAIL COM>
//

#include <Rr/Trait/MemberDecay.hpp>
#include <Rr/Trait/AddCvpref.hpp>
#include <Rr/Trait/RemoveReference.hpp>
#include <Rr/Trait/RemoveReference.hpp>
#include <iostream>

template <class Tcb>
typename Rr::Trait::MemberDecay<Tcb>::ReturnType
invoke(Tcb aCallback, typename Rr::Trait::MemberDecay<Tcb>::InstanceType *aInstance)
{
	return (aInstance->*aCallback)("echo hello");
}

template <class Tsignature, class Tinstance>
void signatureInvoke(typename Rr::Trait::MemberDecay<Tsignature, Rr::Trait::Stript<Tinstance>>::CallbackType aCallback, Tinstance aInstance)
{
	(aInstance->*aCallback)("echo hello");
}

struct Derived {
	int doInvoke(const char *a)
	{
		std::cout << a << std::endl;
		return 42;
	}

	int doInvoke(const char *a) const
	{
		std::cout << a << std::endl;
		return 42;
	}
};

void memberDecay()
{
	const Derived d;
	// invoke(&Derived::doInvoke, &d);  // Cannot choose overloaded version
	signatureInvoke<int(const char *)const>(&Derived::doInvoke, &d);
}

int main(void)
{
	memberDecay();
}
