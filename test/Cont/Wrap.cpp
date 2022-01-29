//
// Wrap.cpp
//
// Created on: 2022-01-22
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Cont/Wrap.hpp>
#include <Rr/Cont/Array.hpp>
#include <array>
#include <list>

template <class T>
using Vec3d = std::array<T, 3>;

template <class T>
using RrVec3d = typename Rr::Cont::Array<T, 3>;

TEST_CASE("Cont::Wrap")
{
	SUBCASE("wrap std::array")
	{
		struct Pod {
			int a;
			char b;
		};
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
			(void)pod;
		}
	}

	SUBCASE("wrap Rr::Cont::Array")
	{
		struct NonPod {
			int a;
			char b;
			NonPod(char aB): b(aB), a(aB)
			{
			}
			NonPod(): a(42), b(42)
			{
			}
		};

		Rr::Cont::Wrap<NonPod, RrVec3d> wrap;

		auto it = wrap.begin();
		wrap.push({1});
		CHECK(1 == it->a);

		++it;
		wrap.emplace();
		CHECK(42 == it->a);
		CHECK(2 == wrap.size());

		++it;
		wrap.push({3});
		CHECK(3 == it->b);
		CHECK(3 == wrap.size());

		++it;
		CHECK(it == wrap.end());

		for (auto &nonpod : wrap) {
			(void)nonpod;
		}
	}

	SUBCASE("wrap std::list") {
		struct Pod {
			int a;
			char b;
		};

		Rr::Cont::Wrap<Pod, std::list> wrap;

		wrap.push({1, 'a'});
		wrap.size();
		CHECK(1 == wrap.size());

		wrap.emplace(Pod{2, 'b'});
		CHECK(2 == wrap.size());
	}
}
