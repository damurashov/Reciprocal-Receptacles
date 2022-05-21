//
// Key.cpp
//
// Created on:  Feb 12, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Cb/Callable.hpp>
#include <mutex>
#include <list>

namespace TestCbCb {

struct Instance {
	static int nCallsCtor;
	static int nCallsDtor;
	static int nCallsCopyCtor;
	static int nCallsMoveCtor;

	static void resetCalls()
	{
		nCallsCtor = 0;
		nCallsDtor = 0;
		nCallsMoveCtor = 0;
		nCallsCopyCtor = 0;
	}
	Instance()
	{
		++nCallsCtor;
	}

	Instance(const Instance &&)
	{
		++nCallsCopyCtor;
	}

	Instance(Instance &&)
	{
		++nCallsMoveCtor;
	}

	~Instance()
	{
		++nCallsDtor;
	}
};

int Instance::nCallsCtor = 0;
int Instance::nCallsDtor = 0;
int Instance::nCallsCopyCtor = 0;
int Instance::nCallsMoveCtor = 0;

Instance getInstance()
{
	return Instance{};
}

}  // TestCbCb

TEST_CASE("Cb::Cb") {
	MESSAGE(sizeof(Rr::Cb::Callable<void(int)>));
}

TEST_CASE("Cb::Cb: return values") {
	CHECK_EQ(TestCbCb::Instance::nCallsCtor, 0);
	CHECK_EQ(TestCbCb::Instance::nCallsDtor, 0);
	CHECK_EQ(TestCbCb::Instance::nCallsCopyCtor, 0);
	CHECK_EQ(TestCbCb::Instance::nCallsMoveCtor, 0);

	SUBCASE("RVO, no copy on return") {
		Rr::Cb::Callable<TestCbCb::Instance()> key{TestCbCb::getInstance};
		(void)key();
		CHECK_EQ(TestCbCb::Instance::nCallsCtor, 1);
		CHECK_EQ(TestCbCb::Instance::nCallsDtor, 1);
		CHECK_EQ(TestCbCb::Instance::nCallsCopyCtor, 0);
		CHECK_EQ(TestCbCb::Instance::nCallsMoveCtor, 0);
	}

	TestCbCb::Instance::resetCalls();
}
