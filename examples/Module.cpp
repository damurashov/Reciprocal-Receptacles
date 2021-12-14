//
// Module.cpp
//
// Created: 2021-12-14
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <iostream>
#include <list>
#include <Rr/Module.hpp>

template <class Tsignature, class Ttopic>
using Module = Rr::Module<Tsignature, Ttopic, std::list, Rr::Trait::GroupMutSyncTypes<0>>;

namespace ModuleType {
struct Socket;
}  // namespace ModuleType

using ClientCounter = Module<int(), ModuleType::Socket>;

int countClients()
{
	return 1;
}

struct Backend {
	int countClients()
	{
		return 1;
	}
};

int main(void)
{
	Backend backend;
	ClientCounter scm1{countClients};
	ClientCounter scm2{&Backend::countClients, &backend};

	auto iterable = ClientCounter::getIterators();
	for (auto &callable : Rr::Trait::move(iterable)) {
	}
}
