//
// Storage.cpp
//
// Created on: 2022-01-30
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Sync/Storage.hpp>
#include <Rr/Cb/Callable.hpp>
#include <list>
#include <mutex>

namespace TestStorage {

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
	static constexpr auto kPolicy = Rr::Sync::Policy::Type::Mutex;
	using Mutex = TestMutex;
};

struct Key {
	using Callable = Rr::Cb::Callable<void(void)>;
	using Container = Rr::Sync::Storage<Callable, SyncTrait, std::list>;

	static int nCalls;
	static Container container;

	Callable callable;
	Container::Value &control;

	void call()
	{
		++nCalls;
	}

	Key(): callable{&Key::call, this}, control{container.reg(callable)}
	{
	}

	void setEnabled(bool f)
	{
		 control.stored = (f ? &callable : nullptr);
	}

	static void iterateAll()
	{
		for (auto &callable : container.getIterators()) {
			callable();
		}
	}
};

int Key::nCalls = 0;
Key::Container Key::container{};

}  // namespace TestStorage

TEST_CASE("Storage") {
	using namespace TestStorage;

	TestStorage::Key::nCalls = 0;
	TestStorage::TestMutex::nLockCalls = 0;
	TestStorage::TestMutex::nUnlockCalls = 0;
	Key keys[2];

	SUBCASE("Callable") {
		Key::iterateAll();

		CHECK_EQ(2, TestStorage::Key::nCalls);
		CHECK_EQ(6, TestStorage::TestMutex::nLockCalls);
		CHECK_EQ(TestStorage::TestMutex::nUnlockCalls, TestStorage::TestMutex::nLockCalls);
	}

//	SUBCASE("Callable, disable one") {
//		keys[0].setEnabled(false);
//		Key::iterateAll();

//		CHECK_EQ(1, Key::nCalls);
//		CHECK_EQ(6, TestMutex::nLockCalls);
//		CHECK_EQ(TestMutex::nUnlockCalls, TestMutex::nLockCalls);
//	}

	reinterpret_cast<std::list<Key::Container::Value> *>(&Key::container)->clear();
}
