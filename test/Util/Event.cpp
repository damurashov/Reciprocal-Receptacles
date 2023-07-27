//
// Key.cpp
//
// Created on:  Feb 12, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#include <cassert>
#include <doctest/doctest.h>

#include <Rr/Util/Event.hpp>
#include <list>
#include <mutex>
#include <thread>
#include <math.h>

namespace TestUtilEvent {

struct SyncTrait {
	using MutexType = std::mutex;

	template <class ...Ts>
	using CallableContainerType = std::list<Ts...>;
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

static constexpr int kEventTaskCycles = 10;
static constexpr int kEventProducerTaskCycles = 10;
static constexpr std::chrono::milliseconds kEventTaskDelay{1};
static constexpr std::chrono::milliseconds kEventProducerTaskDelay{1};

void eventTask()
{
	Event functionCallbackEvent{TestUtilEvent::simpleCallable};

	for (int i = 0; i < kEventTaskCycles; ++i) {
		std::this_thread::sleep_for(kEventTaskDelay);
		functionCallbackEvent.setEnabled(false);
		std::this_thread::sleep_for(kEventTaskDelay);
		functionCallbackEvent.setEnabled(true);
	}
}

void eventProducerTask(int &aValue)
{
	for (int i = 0; i < kEventProducerTaskCycles; ++i) {
		std::this_thread::sleep_for(kEventTaskDelay);
		Event::notify(aValue);
		std::this_thread::sleep_for(kEventTaskDelay);
		Event::notify(aValue);
	}
}

}  // namespace TestUtilEvent

TEST_CASE("Util::Event Notification")
{
	TestUtilEvent::Event functionCallbackEvent{TestUtilEvent::simpleCallable};
	TestUtilEvent::MethodCallable methodCallable{};
	int a = 0;
	TestUtilEvent::Event::notify(a);
	CHECK(a == 2);
}

/// Checks the correctness of a statistical hypothesis that half of the time
/// the value will not get changed
TEST_CASE("Util::Event Multithreading switch `setEnabled()`")
{
	constexpr int kRuns = 20;
	constexpr float kDeviation = 1.0f;
	constexpr float kExpected = static_cast<float>(TestUtilEvent::kEventProducerTaskCycles);
	float mean = 0.0f;

	for (int i = 0; i < kRuns; ++i) {
		int value = 0;
		std::thread eventTask{TestUtilEvent::eventTask};
		TestUtilEvent::eventProducerTask(value);
		eventTask.join();
		mean += static_cast<float>(value);
	}

	mean /= static_cast<float>(kRuns);
	float diff = fabs(mean - kExpected);
	CHECK(diff < kDeviation);
}
