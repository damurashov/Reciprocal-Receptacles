//
// LockType.cpp
//
// Created: 2021-12-13
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Trait/SyncType.hpp>
#include <Rr/Trait/LockType.hpp>
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

void groupLockType() {
	std::cout << Rr::Trait::LockTypeImpl::GroupLockType<void(int), GroupSyncTrait<0>>::kIsConst << std::endl;
	std::cout << Rr::Trait::LockTypeImpl::GroupLockType<void(int)const, GroupSyncTrait<0>>::kIsConst << std::endl;

	static_assert(std::is_same<GroupSyncTrait<1>::WriteLockType, typename Rr::Trait::LockPolicy<void(int),       GroupSyncTrait<1>>::Type>::value, "");
	static_assert(std::is_same<GroupSyncTrait<1>::WriteLockType, typename Rr::Trait::LockPolicy<void(int) const, GroupSyncTrait<1>>::Type>::value, "");
	static_assert(std::is_same<GroupSyncTrait<0>::ReadLockType,  typename Rr::Trait::LockPolicy<void(int) const, GroupSyncTrait<0>>::Type>::value, "");
}

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
	groupLockType();
	asSyncTrait();

	return 0;
}
