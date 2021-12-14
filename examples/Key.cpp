//
// Key.cpp
//
// Created: 2021-12-09
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <iostream>
#include <list>
#include <Rr/Key.hpp>
#include <Rr/Trait/SyncType.hpp>

template <class Tsignature, class Ttopic>
using Key = Rr::Key<Tsignature, Ttopic, std::list, Rr::Trait::GroupMutSyncTypes<0>>;

namespace Topic {
struct CameraConnected;
}  // namespace Topic

using KeyCameraConnected = Key<void(int), Topic::CameraConnected>;

void call(int)
{
	std::cout << "call(int)" << std::endl;
}

struct St {
	void call(int)
	{
		std::cout << "St::call(int)" << std::endl;
	}
	void call(int) const
	{
		std::cout << "St::call(int)const" << std::endl;
	}
	void callMe(int)
	{
		std::cout << "St::callMe(int)" << std::endl;
	}
};

int main(void)
{
	KeyCameraConnected key{call};
	KeyCameraConnected key2{true, call};

	St st;
	KeyCameraConnected key3{(void(St::*)(int))&St::call, &st};
	KeyCameraConnected key4{false, (void(St::*)(int))&St::call, &st};
	key4.setEnabled(true);
	KeyCameraConnected key5{true, &St::callMe, &st};

	KeyCameraConnected::notify(42);
}
