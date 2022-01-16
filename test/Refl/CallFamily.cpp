//
// CallFamily.cpp
//
// Created on: 2022-01-16
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Refl/CallFamily.hpp>

struct S {
    int pushBack(int a)
    {
        return a;
    }
};

struct Overload1 {
    template <class T, class ...Ta>
    static auto call(T &t, int a) -> decltype(t.push_back(a))
    {
        return t.push_back(a);
    }
};

struct Overload2 {
    template <class T, class ...Ta>
    static auto call(T &t, char b) -> decltype(t.pushBack(b))
    {
        return t.pushBack(b);
    }
};

struct Overload3 {
    template <class T, class ...Ta>
    static auto call(T &t, int a) -> decltype(t.pushBack(a))
    {
        return t.pushBack(a);
    }
};

TEST_CASE("CallFamily") {
    S s;
    int a = 422;
    Rr::Refl::CallFamily<Overload1, Overload3, Overload2>::call(s, a);

    SUBCASE("Overload ordering") {
        CHECK(422 == Rr::Refl::CallFamily<Overload1, Overload3, Overload2>::call(s, a));
        CHECK(422 != Rr::Refl::CallFamily<Overload1, Overload2, Overload3>::call(s, a));
    }

    SUBCASE("Check can call") {
        CHECK(false == Rr::Refl::CanCallFamily<Overload1>::check(s, a));
        CHECK(true == Rr::Refl::CanCallFamily<Overload1, Overload2>::check(s, a));
    }
}
