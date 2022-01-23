//
// Wrap.cpp
//
// Created on: 2022-01-22
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Cont/Wrap.hpp>
#include <array>

template <class T>
using Vec3d = std::array<T, 3>;

struct Pod {
	int a;
	char b;
};

TEST_CASE("Cont::Wrap")
{
	SUBCASE("wrap std::array")
	{
		Rr::Cont::Wrap<Pod, Vec3d> wrap;

		auto it = wrap.begin();
		wrap.push({42, 'c'});
		CHECK(42 == it->a);
		CHECK('c' == it->b);

		++it;
		wrap.emplace(12, 'd');
		CHECK(12 == it->a);
		CHECK('d' == it->b);
		CHECK(wrap.size() == 2);

		++it;
		wrap.push({66, 'e'});
		CHECK(66 == it->a);
		CHECK('e' == it->b);

		++it;
		CHECK(it == wrap.end());

		CHECK(wrap.size() == 3);

		for (auto &pod : wrap) {
			(void) pod;
		}
	}
}
