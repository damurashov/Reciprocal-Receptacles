//
// IntegralToType.cpp
//
// Created: 2021-12-15
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Trait/IntegralToType.hpp>
#include <Rr/Trait/IsSame.hpp>

void accept(int)
{
}

struct Type0 {};
struct Type1 {};
struct Type2 {};

template <unsigned Ival>
using Type = typename Rr::Trait::IntegralToType<unsigned, Ival, Type0, Type1, Type2>::Type;

int main()
{
	static_assert(Rr::Trait::IsSame<Type<0>, Type0>::value, "");
	static_assert(Rr::Trait::IsSame<Type<1>, Type1>::value, "");
	static_assert(Rr::Trait::IsSame<Type<2>, Type2>::value, "");
}
