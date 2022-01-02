//
// Introspection.cpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Sync/Introspection.hpp>
#include <iostream>

using namespace std;

struct DefinesMutex {
	using Mutex = void;
};

struct DefinesLock {
	using Lock = void;
};

int main(void)
{
	cout << Rr::Sync::definesMutex<DefinesMutex>() << endl;
	cout << Rr::Sync::definesMutex<DefinesLock>() << endl;
	cout << Rr::Sync::definesLock<DefinesMutex>() << endl;
	cout << Rr::Sync::definesLock<DefinesLock>() << endl;
}