//
// SharedAccess.cpp
//
// Created on: 2022-01-29
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Sync/SharedAccess.hpp>
#include <Rr/Cb/Callable.hpp>
#include <mutex>
#include <list>
#include <array>
#include <iostream>

using namespace std;

struct TestMutex : std::mutex {
	using std::mutex::mutex;

	static int nLockCalls;
	static int nUnlockCalls;

	void lock()
	{
		std::mutex::lock();
		++nLockCalls;
	}

	void unlock()
	{
		std::mutex::unlock();
		++nUnlockCalls;
	}
};

int TestMutex::nLockCalls = 0;
int TestMutex::nUnlockCalls = 0;

struct SyncTrait {
	static constexpr auto kSharedAccessPolicy = Rr::Sync::Policy::Type::Mutex;
	using Mutex = TestMutex;
};

struct Key {

	using CallableType = typename Rr::Cb::Callable<void(void)>;
	using SharedAccessType = Rr::Sync::SharedAccess<CallableType, SyncTrait>;
	using StorageType = std::list<SharedAccessType>;
	static StorageType list;

	static int nCalls;

	void callMe()
	{
		++nCalls;
	}

	CallableType callable;
	SharedAccessType *ptr;

	Key(): callable{&Key::callMe, this}
	{
		list.emplace_back(&callable);
		ptr = &list.back();
	}

	~Key()
	{
		ptr->stored = nullptr;
	}

	void setEnabled(bool fEnabled)
	{
		ptr->stored = fEnabled ? &callable : nullptr;
	}

	static auto begin() -> decltype(Rr::Sync::SharedAccessIt<StorageType::iterator>{list.begin(), list.end()})
	{
		return Rr::Sync::SharedAccessIt<StorageType::iterator>{list.begin(), list.end()};
	}

	static auto end() -> decltype(Rr::Sync::SharedAccessIt<StorageType::iterator>{list.end(), list.end()})
	{
		return Rr::Sync::SharedAccessIt<StorageType::iterator>{list.end(), list.end()};
	}
};

Key::StorageType Key::list {};
int Key::nCalls = 0;

static void reset()
{
	TestMutex::nLockCalls = 0;
	TestMutex::nUnlockCalls = 0;
	Key::nCalls = 0;
}

static void iterate()
{
	for (auto it = Key::begin(); it != Key::end(); ++it) {
		(*it)();
	}
}

TEST_CASE("SharedAccess") {

	std::array<Key, 3> keys;
	reset();

	SUBCASE("all enabled") {
		iterate();

		CHECK_EQ(3, Key::list.size());
		CHECK_EQ(3, TestMutex::nLockCalls);
		CHECK_EQ(3, TestMutex::nUnlockCalls);
		CHECK_EQ(3, Key::nCalls);
	}

	SUBCASE("first disabled") {
		keys[0].setEnabled(false);
		iterate();

		CHECK_EQ(3, Key::list.size());
		CHECK_EQ(3, TestMutex::nLockCalls);
		CHECK_EQ(3, TestMutex::nUnlockCalls);
		CHECK_EQ(2, Key::nCalls);
	}

	SUBCASE("all disabled") {
		for (auto &k : keys) {
			k.setEnabled(false);
		}
		iterate();

		CHECK_EQ(3, Key::list.size());
		CHECK_EQ(3, TestMutex::nLockCalls);
		CHECK_EQ(3, TestMutex::nUnlockCalls);
		CHECK_EQ(0, Key::nCalls);
	}

	reset();
	Key::list.clear();
}
