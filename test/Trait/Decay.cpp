//
// Decay.cpp
//
// Created on:  May 22, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#include <Rr/Trait/Decay.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <doctest/doctest.h>

using namespace Rr::Trait;

template <class Tdecay, class Tref>
static constexpr bool checkDecayEq()
{
	return Rr::Trait::IsSame<Tref, typename Rr::Trait::Decay<Tdecay>::Type>::value;
}

TEST_CASE("Trait::Decay") {
	CHECK(checkDecayEq<int, int>());
	CHECK(checkDecayEq<int[], int *>());
	CHECK(checkDecayEq<int[4], int *>());
	CHECK(checkDecayEq<int &, int>());
	CHECK(checkDecayEq<int &&, int>());
	CHECK(checkDecayEq<const int &, int>());
	CHECK(checkDecayEq<const int *, const int *>());
	CHECK(checkDecayEq<const int * const , const int *>());
	CHECK(checkDecayEq<const int * volatile , const int *>());
	CHECK(checkDecayEq<const int * const volatile , const int *>());
	CHECK(checkDecayEq<int(void), int(*)(void)>());
	CHECK(checkDecayEq<int(*)(void), int(*)(void)>());
}
