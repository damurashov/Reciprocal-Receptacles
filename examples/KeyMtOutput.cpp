//
// Introspection.cpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <cassert>
#include <Rr/Util/Key.hpp>
#include <mutex>
#include <list>
#include <thread>
#include <vector>

using namespace std;

struct SyncTrait {
	static constexpr auto kPolicy = Rr::Sync::Policy::Type::Mutex;
	using Mutex = std::mutex;
};

struct SyncTraitNoSync {
	static constexpr auto kPolicy = Rr::Sync::Policy::Type::None;
};

using KeyType = Rr::Util::Key<void(void), SyncTraitNoSync, std::list>;

void doOutput()
{
	static int i = 0;
	std::cout << (const char *)".begin.middle.end." << i++ << std::endl;
}

void task()
{
	for (int i = 0; i < 100; ++i) {
		KeyType::notify();
		std::this_thread::sleep_for(std::chrono::milliseconds(i));
	}
}
int main() {
	static constexpr auto knThreads = 4;
	KeyType key{doOutput};
	(void)key;
	std::vector<std::thread> threads{knThreads};

	for (int i = 0; i < knThreads; ++i) {
		threads.emplace_back(task);
	}

	std::this_thread::sleep_for(std::chrono::seconds(10));
}
