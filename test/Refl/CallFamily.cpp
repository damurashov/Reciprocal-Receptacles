//
// CallFamily.cpp
//
// Created on: 2022-01-16
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Refl/CallFamily.hpp>
#include <list>

struct S {
	int pushBack(int a)
	{
		return a;
	}
};

struct S2 {
	static int pushBack(int a)
	{
		return a;
	}
};

struct PushBack1 {
	template <class T, class ...Ta>
	static auto call(T &t, int a) -> decltype(t.push_back(a))
	{
		return t.push_back(a);
	}
};

struct PushBack2 {
	template <class T, class ...Ta>
	static auto call(T &t, char b) -> decltype(t.pushBack(b))
	{
		return t.pushBack(b);
	}
};

struct PushBack3 {
	template <class T, class ...Ta>
	static auto call(T &t, int a) -> decltype(t.pushBack(a))
	{
		return t.pushBack(a);
	}
};

struct StaticPushBack1 {
	template <class T>
	static auto call(int a) -> decltype(T::pushBack(a))
	{
		return T::pushBack(a);
	}
};

struct StaticPushBack2 {
	template <class T>
	static auto call(int a) -> decltype(T::push_back(a))
	{
		return T::push_back(a);
	}
};

struct Back1 {
	template <class T>
	static int &call(T &t)
	{
		return t.back();
	}
};

TEST_CASE("CallFamily") {
	S s;
	int a = 422;
	Rr::Refl::CallFamily<PushBack1, PushBack3, PushBack2>::call(s, a);

	SUBCASE("Overload ordering") {
		CHECK(422 == Rr::Refl::CallFamily<PushBack1, PushBack3, PushBack2>::call(s, a));
		CHECK(422 != Rr::Refl::CallFamily<PushBack1, PushBack2, PushBack3>::call(s, a));
	}

	SUBCASE("Check can call") {
		CHECK(false == Rr::Refl::CanCallFamily<PushBack1>::check(s, a));
		CHECK(true == Rr::Refl::CanCallFamily<PushBack1, PushBack2>::check(s, a));
	}

	SUBCASE("STL list") {
		std::list<int> list = {1,2,3};
		CHECK(true == Rr::Refl::CanCallFamily<PushBack1, PushBack2, PushBack3>::check(list, 42));
		CHECK(true == Rr::Refl::CanCallFamily<Back1, PushBack1, PushBack2, PushBack3>::check(list));
		CHECK(3 == Rr::Refl::CallFamily<Back1, PushBack1, PushBack2, PushBack3>::call(list));
	}

	SUBCASE("Call family, static") {
		CHECK(true == Rr::Refl::CanCallFamily<StaticPushBack1, StaticPushBack2>::check<S2>(42));
		CHECK(false == Rr::Refl::CanCallFamily<StaticPushBack1, StaticPushBack2>::check<S>(42));
		CHECK(42 == Rr::Refl::CallFamily<StaticPushBack2, StaticPushBack1>::call<S2>(42));
	}
}
