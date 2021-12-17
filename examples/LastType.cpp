//
// LastType.cpp
//
// Created: 2021-12-16
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <iostream>
#include <Rr/Trait/LastType.hpp>
#include <Rr/Trait/IsSame.hpp>

int main(void)
{
	static_assert(Rr::Trait::IsSame<void, Rr::Trait::LastType<int, char, bool, void>::Type>::value, "");
	static_assert(!Rr::Trait::IsSame<char, Rr::Trait::LastType<int, char, bool, void>::Type>::value, "");
}
