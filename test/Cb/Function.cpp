//
// Key.cpp
//
// Created on:  Feb 12, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#undef RRO_USE_OPERATOR_NEW
#define RRO_USE_OPERATOR_NEW 1

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Cb/Function.hpp>
#include <mutex>
#include <list>

template <class ...Ts> struct ArgsList;

TEST_CASE("Cb::Function") {
	Rr::Cb::Function<void(int), ArgsList<int>> function;
}
