//
// LockType.cpp
//
// Created: 2021-12-13
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Trait/SyncType.hpp>
#include <type_traits>
#include <iostream>

using namespace std;

template <unsigned Igroup>
struct GroupSyncTrait {
	using ReadLockType = int;
	using WriteLockType = float;
	using Type = void;
	static constexpr auto kGroup = Igroup;
};

struct IndUnique {
	using UniqueMutexType = int;
	using LockType = int;
};

struct NoSync {
	static constexpr auto kSyncTraitId = Rr::Trait::SyncTraitId::NoSync;
};

struct IndShared {
	using SharedMutexType = int;
	using LockType = int;
	using SharedLockType = int;
};

struct GroupUnique {
	using GroupUniqueLockType = int;
	int syncPrimitive;
};

void asSyncTrait()
{
	std::cout << (int)Rr::Trait::ToSyncTraitId<NoSync>::value << endl;
	std::cout << (int)Rr::Trait::ToSyncTraitId<IndUnique>::value << endl;
	std::cout << (int)Rr::Trait::ToSyncTraitId<IndShared>::value << endl;
	std::cout << (int)Rr::Trait::ToSyncTraitId<GroupUnique>::value << endl;

	// using MutTrait = Rr::Trait::AsMutTrait<IndShared>;
	// MutTrait mt {};
}

int main(void)
{
	asSyncTrait();

	return 0;
}
