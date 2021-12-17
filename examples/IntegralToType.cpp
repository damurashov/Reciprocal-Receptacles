//
// IntegralToType.cpp
//
// Created: 2021-12-15
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Trait/IntegralToType.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <list>

void accept(int)
{
}

struct Type0 {};
struct Type1 {};
struct Type2 {};

template <class ...Ta>
struct Ttype0;

template <unsigned Ival>
using Type = typename Rr::Trait::IntegralToType<unsigned, Ival, Type0, Type1, Type2>::Type;

// template <unsigned Ival, class ...Ta>
// using Ttype = typename Rr::Trait::IntegralToTemplate<unsigned, Ival, Ttype0, std::list>::Type<Ta...>;

template <class T>
struct A {
	using Type = typename T::Atype;
};

template <class T>
struct B {
	using Type = typename T::Btype;
};

struct TypeHolder {
	using Btype = void;
};

int main()
{
	static_assert(Rr::Trait::IsSame<Type<0>, Type0>::value, "");
	static_assert(Rr::Trait::IsSame<Type<1>, Type1>::value, "");
	static_assert(Rr::Trait::IsSame<Type<2>, Type2>::value, "");

	using Ltype = Rr::Trait::IntegralToType<unsigned, 1, A<TypeHolder>, B<TypeHolder>>::Type;
}
