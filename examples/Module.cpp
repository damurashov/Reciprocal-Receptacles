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
using Module1 = Rr::Module<Tsignature, Ttopic, std::list, Rr::Trait::GroupMutSyncTypes<1>>;

namespace ModuleType {
struct Socket;
struct HighSpeedSocket;
}  // namespace ModuleType

using ClientCounter = Module1<int(), ModuleType::Socket>;
using SpecialClientCounter = Module1<int(), ModuleType::HighSpeedSocket>;

int countClients()
{
	return 1;
}

int countSpecialClients()
{
	return 1;
}

struct Backend {
	int countClients()
	{
		auto nSpecialClients = 0;

		for (auto &callable : SpecialClientCounter::getIterators()) {
			if (callable.isEnabled()) {  // Not necessary since 0.4.1
				nSpecialClients += callable();
			}
		}

		return 1 + nSpecialClients;
	}
};

void poll()
{
	auto nClients = 0;
	for (auto &callable : ClientCounter::getIterators()) {
		nClients += callable();  // Please note that no `isEnabled()` checks were made
	}

	std::cout << "nClients: " << nClients << std::endl;
}

int main(void)
{
	Backend backend;
	ClientCounter scm1{countClients};
	ClientCounter scm2{&Backend::countClients, &backend};
	SpecialClientCounter scm3{countSpecialClients};

	poll();  // 3
	scm2.setEnabled(false);
	poll();  // 1
	scm1.setEnabled(false);
	poll(); // ...
}
