//
// Variant.cpp
//
// Created on:  May 21, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#include <doctest/doctest.h>
#include <Rr/Cont/Variant.hpp>
#include <string>

TEST_CASE("Cont::Variant, construction")
{
	using Variant = Rr::Cont::Variant<int, std::string, char>;

	SUBCASE("move construction") {
		Variant variant1{std::string("")};
		Variant variant2{0};
		Variant variant3{'c'};

		CHECK(Rr::Cont::Vrnt::holdsAlternative<std::string>(variant1));
		CHECK(Rr::Cont::Vrnt::holdsAlternative<char>(variant3));
		CHECK(Rr::Cont::Vrnt::holdsAlternative<int>(variant2));
	}

	SUBCASE("copy construction") {
		std::string valString;

		Variant variant1{valString};
		CHECK(Rr::Cont::Vrnt::holdsAlternative<std::string>(variant1));
	}
}
