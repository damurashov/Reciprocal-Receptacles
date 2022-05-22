//
// Decay.cpp
//
// Created on:  May 22, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#include <Rr/Trait/IsConstructible.hpp>
#include <string>
#include <doctest/doctest.h>

using namespace Rr::Trait;

struct S {
	S(int)
	{
	}

	explicit S(char)
	{
	}

	S(std::string)
	{
	}

	S(const S &)
	{
	}

	S(S &&) = delete;
};

template <class Tobj, class ...Ta>
static constexpr bool checkConstructible()
{
	return Rr::Trait::IsConstructible<Tobj, Ta...>::value;
}

TEST_CASE("Trait::IsConstructible") {
	CHECK(checkConstructible<S, int>());
	CHECK(checkConstructible<S, char>());
	CHECK(checkConstructible<S, const S &>());
	CHECK(!checkConstructible<S, S &&>());
	CHECK(checkConstructible<std::string, const char *, unsigned>());
	CHECK(checkConstructible<S, std::string>());
	CHECK(checkConstructible<S, const char *>());
}
