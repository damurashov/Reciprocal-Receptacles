#include <doctest/doctest.h>

TEST_CASE("dummy") {
	int a = 42;

	SUBCASE("Should fail") {
		a = -1;
		CHECK(a < 0);
	}

	SUBCASE("Should succeed") {
		CHECK(a > 0);
	}

}
