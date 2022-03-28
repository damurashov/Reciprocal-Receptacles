//
// Key.cpp
//
// Created on:  Feb 12, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Util/Key.hpp>
#include <mutex>
#include <list>

namespace TestCb {

}  // TestCb

TEST_CASE("Cb") {
	MESSAGE(sizeof(Rr::Cb::Callable<void(int)>));
}
