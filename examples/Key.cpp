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

void call(int aInt)
{
	std::cout << "call(int) " << aInt << std::endl;
}

struct St {
	void call(int aInt)
	{
		std::cout << "St::call(int) " << aInt << std::endl;
	}
	void call(int aInt) const
	{
		std::cout << "St::call(int)const " << aInt << std::endl;
	}
	void callMe(int aInt)
	{
		std::cout << "St::callMe(int) " << aInt << std::endl;
	}
};

int main(void)
{
	St st;
	KeyCameraConnected key{call};
	KeyCameraConnected key2{true, call};
	KeyCameraConnected key3{(void(St::*)(int))&St::call, &st};
	KeyCameraConnected key4{false, (void(St::*)(int))&St::call, &st};
	KeyCameraConnected key5{true, &St::callMe, &st};

	KeyCameraConnected::notify(42);
	key4.setEnabled(false);
	KeyCameraConnected::notify(777);
}
