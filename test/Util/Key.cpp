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

namespace TestKey {

enum Ret {
	Correct = 42,
	Incorrect,
};

using Signature = Ret(int)const;

struct SyncTrait {
	static constexpr auto kPolicy = Rr::Sync::Policy::Type::Mutex;
	using Mutex = std::mutex;
};

using Key = Rr::Util::Key<Signature, SyncTrait, std::list>;

struct Call {
	Ret doCall(int) const
	{
		++ncalled;
		return Ret::Correct;
	}

	Ret doCall(int)
	{
		return Ret::Incorrect;
	}

	Key key;

	Call(): key{&Call::doCall, this}
	{
	}

	static int ncalled;

	static void reset()
	{
		ncalled = 0;
	}
};

int Call::ncalled = 0;

Ret staticCallable(int)
{
	++Call::ncalled;
	return Ret::Correct;
}

}  // TestKey

TEST_CASE("Key") {
	TestKey::Call call;
	TestKey::Call::reset();

	SUBCASE("All enabled") {
		for (auto &callable : TestKey::Key::getIterators()) {
			CHECK(TestKey::Ret::Correct == callable(0xffff));
		}

		CHECK(1 == TestKey::Call::ncalled);
	}

	SUBCASE("Enabled / disabled") {
		call.key.setEnabled(false);
		for (auto &callable : TestKey::Key::getIterators()) {
			CHECK(TestKey::Ret::Correct == callable(0));
		}
		TestKey::Key::notify(0);

		CHECK(0 == TestKey::Call::ncalled);

		call.key.setEnabled(true);
		for (auto &callable : TestKey::Key::getIterators()) {
			CHECK(TestKey::Ret::Correct == callable(0));
		}
		TestKey::Key::notify(0);

		CHECK(2 == TestKey::Call::ncalled);
	}

	TestKey::Key keySc{TestKey::staticCallable};

	SUBCASE("static / member") {
		CHECK(0 == TestKey::Call::ncalled);

		for (auto &callable : TestKey::Key::getIterators()) {
			CHECK(TestKey::Ret::Correct == callable(0));
		}

		CHECK(2 == TestKey::Call::ncalled);
	}
}
