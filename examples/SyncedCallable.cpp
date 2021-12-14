//
// SyncedCallable.cpp
//
// Created: 2021-12-14
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <iostream>
#include <Rr/Util/SyncedCallable.hpp>
#include <Rr/Trait/SyncType.hpp>

using namespace Rr::Util;
using namespace Rr::Trait;

using SyncTrait = GroupMutSyncTypesMock<0>;
using CbType = typename SyncedCallableType<void(int), SyncTrait>::Type;

struct St {
	void call(int)
	{
		std::cout << "void St::call(int)" << std::endl;
	}
};

void call(int)
{
	std::cout << "void call(int)" << std::endl;
}

void syncedCallable()
{
}

int main()
{
	St st;
	CbType callableStatic{call};
	CbType callableMember{&St::call, &st};

	callableStatic(42);
	callableMember(42);

	return 0;
}
