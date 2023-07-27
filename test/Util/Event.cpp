//
// Key.cpp
//
// Created on:  Feb 12, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#include <cassert>
#include <doctest/doctest.h>
#include <Rr/Util/Event.hpp>
#include <mutex>
#include <list>

namespace TestUtilEvent {

struct SyncTrait {
	using MutexType = std::mutex;

	template <class T>
	using CallableContainerType = std::list<T>;
};

using Event = typename Rr::Util::Event<void(int &), SyncTrait>;


void simpleCallable(int &aValue)
{
	aValue += 1;
}

struct MethodCallable {
	void callable(int &aValue)
	{
		aValue += 1;
	}

	Event event;

	MethodCallable():
		event{&MethodCallable::callable, this}
	{
	}
};

}  // namespace TestUtilEvent

TEST_CASE("Util::Event Notification") {
	TestUtilEvent::Event functionCallbackEvent{TestUtilEvent::simpleCallable};
	MethodCallable methodCallable{};
	int a = 0;
	TestUtilEvent::Event::notify(a);
	CHECK(a == 2);
}
