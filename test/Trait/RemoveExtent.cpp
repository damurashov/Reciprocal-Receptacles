//
// RemoveExtent.cpp
//
// Created on:  May 22, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/RemoveReference.hpp>
#include <doctest/doctest.h>

using namespace Rr::Trait;

template <class Trm, class Tref>
static constexpr bool checkRmeEq()
{
	return Rr::Trait::IsSame<Tref, typename Rr::Trait::RemoveExtent<Trm>::Type>::value;
}

TEST_CASE("Trait::Decay") {
	CHECK(checkRmeEq<int, int>());
	CHECK(checkRmeEq<int[], int>());
	CHECK(checkRmeEq<int[4], int>());
}
