//
// Introspection.cpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Sync/Introspection.hpp>
#include <Rr/Sync/Policy/Type.hpp>
#include <iostream>

using namespace std;

struct DefinesMutex {
	using Mutex = void;
};

struct DefinesLock {
	using Lock = void;
};

struct DefinesCallPolicy {
	static constexpr auto kCallPolicy = Rr::Sync::Policy::Type::None;
};

template <class ...Ta>
void defines()
{
	using IterType = int[];
#define _cout_(METHOD) cout << #METHOD << endl; (void)IterType{((void)(cout << Rr::Sync::METHOD<Ta>() << endl), 0)...}
	_cout_(definesMutex);
	_cout_(definesLock);
	_cout_(definesCallPolicy);
#undef _cout_
}

int main(void)
{
	defines<DefinesMutex, DefinesLock, DefinesCallPolicy>();
}