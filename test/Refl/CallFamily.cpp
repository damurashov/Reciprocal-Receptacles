//
// CallFamily.cpp
//
// Created on: 2022-01-16
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Refl/CallFamily.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/Declval.hpp>
#include <list>

struct S {
	int pushBack(int a)
	{
		return a;
	}

	int pushBack()
	{
		return 42;
	}
};

struct S2 {
	static int pushBack(int a)
	{
		return a;
	}

	static int pushBack()
	{
		return 42;
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

struct PushBack4 {
	template <class T>
	static auto call(T &t) -> decltype(t.pushBack())
	{
		return t.pushBack();
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

struct StaticPushBack3 {
	template <class T>
	static auto call() -> decltype(T::pushBack())
	{
		return T::pushBack();
	}
};

struct Back1 {
	template <class T>
	static int &call(T &t)
	{
		return t.back();
	}
};

struct Styped {
	int typed()
	{
		return 42;
	}
};

struct RetTyped {
	int a;
	char b;
};

struct Typed1 {
	template <class T>
	static auto call(T &a) -> decltype((int)a.typed())
	{
		return a.typed();
	}
};

struct Typed2 {
	template <class T>
	static auto call(T &a) -> decltype((RetTyped)a.typed())
	{
		return a.typed();
	}
};

struct Typed3 {
	template <class T>
	static char *call(T &a)
	{
		return a.typed();
	}
};

struct Snodef {
	using Type = void;
};

struct Nodef {
	template <class T>
	static typename T::Type call();
};

struct CallEmplace {
	template <class Ti, class ...Ts>
	static auto call(Ti &&aT, Ts &&...aArgs) -> decltype(aT.emplace(Rr::Trait::forward<Ts>(aArgs)...))
	{
		return aT.emplace(Rr::Trait::forward<Ts>(aArgs)...);
	}
};

struct CallEmplaceBackUnderscore {
	template <class Ti, class ...Ts>
	static auto call(Ti &&aT, Ts &&...aArgs) -> decltype(aT.emplace_back(Rr::Trait::forward<Ts>(aArgs)...))
	{
		return aT.emplace_back(Rr::Trait::forward<Ts>(aArgs)...);
	}
};

struct CallEmplaceBackCamelcase {
	template <class Ti, class ...Ts>
	static auto call(Ti &&aT, Ts &&...aArgs) -> decltype(aT.emplaceBack(Rr::Trait::forward<Ts>(aArgs)...))
	{
		return aT.emplaceBack(Rr::Trait::forward<Ts>(aArgs)...);
	}
};

struct CallEmplaceMarker {
	template <class Ti>
	static auto call() -> decltype(Rr::Trait::declval<Ti>().emplace({}));
};

struct CallEmplaceBackUsMarker {
	template <class Ti>
	static void call(decltype(&Ti::emplace_back) = nullptr);
};

struct CallEmplaceBackCcMarker {
	template <class Ti>
	static void call(decltype(&Ti::emplaceBack) = nullptr);
};

struct CallCamelUnderscore {
	struct None {
	};

	struct Camel {
		static void callMe();
	};

	struct Underscore {
		static void call_me();
	};

	template <class Ti>
	static auto call() -> decltype(Ti::call_me());

	template <class Ti>
	static auto call() -> decltype(Ti::callMe());
};

TEST_CASE("CallFamily") {
	S s;
	int a = 422;
	Rr::Refl::CallFamily<PushBack1, PushBack3, PushBack2>::call(s, a);

	SUBCASE("Camelcase / underscore in one call hint") {
		using namespace Rr::Refl;
		CHECK(!CanCallFamily<CallCamelUnderscore>::check<CallCamelUnderscore::None>());
		CHECK(CanCallFamily<CallCamelUnderscore>::check<CallCamelUnderscore::Camel>());
		CHECK(CanCallFamily<CallCamelUnderscore>::check<CallCamelUnderscore::Underscore>());
	}

	SUBCASE("Has method, marker") {
		CHECK(Rr::Refl::CanCallFamily<CallEmplaceMarker>::check<std::list<int>>());  // Note that std::list<...>::emplace is a template method which draws the compiler being enable to determine the correct "overload"
	}

	SUBCASE("Overload ordering") {
		CHECK(422 == Rr::Refl::CallFamily<PushBack1, PushBack3, PushBack2>::call(s, a));
		CHECK(422 != Rr::Refl::CallFamily<PushBack1, PushBack2, PushBack3>::call(s, a));
	}

	SUBCASE("Overload ordering 2") {
		CHECK(Rr::Refl::CanCallFamily<PushBack1, PushBack3, PushBack2, PushBack4>::check(s));
		CHECK(42 == Rr::Refl::CallFamily<PushBack1, PushBack2, PushBack3, PushBack4>::call(s));
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

	SUBCASE("STL list emplace") {
		struct Pod {
			int a;
			char b;
		};

		std::list<Pod> list;
		list.emplace_back(Pod{42, 'c'});
		CHECK(true == Rr::Refl::CanCallFamily<CallEmplaceBackCamelcase, CallEmplaceBackUnderscore, CallEmplace>::check(list, Pod{42, 'c'}));
		Rr::Refl::CallFamily<CallEmplaceBackCamelcase, CallEmplaceBackUnderscore, CallEmplace>::call(list, Pod{42, 'c'});

		std::list<int> ilist;
		CHECK(true == Rr::Refl::CanCallFamily<CallEmplaceBackCamelcase, CallEmplaceBackUnderscore, CallEmplace>::check(ilist, 42));
	}

	SUBCASE("Call family, static") {
		// With a parameter
		CHECK(true == Rr::Refl::CanCallFamily<StaticPushBack1, StaticPushBack2>::check<S2>(42));
		CHECK(false == Rr::Refl::CanCallFamily<StaticPushBack1, StaticPushBack2>::check<S>(42));
		CHECK(42 == Rr::Refl::CallFamily<StaticPushBack2, StaticPushBack1>::call<S2>(42));

		// W/o a parameter
		CHECK(Rr::Refl::CanCallFamily<StaticPushBack1, StaticPushBack2, StaticPushBack3>::check<S2>());
		CHECK(Rr::Refl::CanCallFamily<StaticPushBack3, StaticPushBack2, StaticPushBack1>::check<S2>());
		CHECK(!Rr::Refl::CanCallFamily<StaticPushBack2>::check<S2>());

		CHECK(42 == Rr::Refl::CallFamily<StaticPushBack1, StaticPushBack2, StaticPushBack3>::call<S2>());
		CHECK(43 == Rr::Refl::CallFamily<StaticPushBack1, StaticPushBack2, StaticPushBack3>::call<S2>(43));
	}

	SUBCASE("Call family, exact type")
	{
		Styped s;
		using Ret = decltype(s.typed());
		using Ret1 = decltype(Rr::Refl::CallFamily<Typed3, Typed2, Typed1>::call(s));
		using Ret2 = decltype(Rr::Refl::CallFamily<Typed2, Typed1>::call(s));

		constexpr bool f1 = Rr::Trait::IsSame<Ret, Ret1>::value;
		constexpr bool f2 = Rr::Trait::IsSame<Ret, Ret2>::value;

		CHECK(!f1);
		CHECK(f2);
	}

	SUBCASE("Call family, decltype")
	{
		using Ret = decltype(Rr::Refl::CallFamily<Nodef>::call<Snodef>());
		constexpr bool f = Rr::Trait::IsSame<Ret, void>::value;

		CHECK(f);
	}
}

