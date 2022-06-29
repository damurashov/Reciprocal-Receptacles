//
// Callable.cpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//


// Use reinterpret_cast for method pointers conversion
// No inheritance from Rr::RrObject required
#define RRO_STATIC_CAST_FN_CONVERSION 0

#include <iostream>
#include <Rr/Cb/Callable.hpp>

using namespace std;

#define PRINT_SIZEOF(a) std::cout << "sizeof " #a << " = " << sizeof(a) << std::endl

int main(void)
{
	using Wrap = Rr::Cb::CallableImpl::CallWrap<void(int), typename Rr::Trait::MemberDecay<void(int)>::ArgsList>;
	PRINT_SIZEOF(Wrap);

	using Member = Rr::Cb::CallableImpl::CallMember<void(int), typename Rr::Trait::MemberDecay<void(int)>::ArgsList>;
	PRINT_SIZEOF(Member);
	PRINT_SIZEOF(Member::cell);
	PRINT_SIZEOF(Member::caller);
	PRINT_SIZEOF(Member::_vptr);

	using Cb = Rr::Cb::Callable<void(int)>;
	PRINT_SIZEOF(Cb);
}
