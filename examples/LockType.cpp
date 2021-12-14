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

template <unsigned Igroup>
struct GroupSyncTrait {
	using ReadLockType = int;
	using WriteLockType = float;
	using Type = void;
	static constexpr auto kGroup = Igroup;
};

void groupLockType() {
	std::cout << Rr::Trait::GroupLockType<void(int), GroupSyncTrait<0>>::kIsConst << std::endl;
	std::cout << Rr::Trait::GroupLockType<void(int)const, GroupSyncTrait<0>>::kIsConst << std::endl;

	static_assert(std::is_same<GroupSyncTrait<1>::WriteLockType, typename Rr::Trait::LockType<void(int),       GroupSyncTrait<1>>::Type>::value, "");
	static_assert(std::is_same<GroupSyncTrait<1>::WriteLockType, typename Rr::Trait::LockType<void(int) const, GroupSyncTrait<1>>::Type>::value, "");
	static_assert(std::is_same<GroupSyncTrait<0>::ReadLockType,  typename Rr::Trait::LockType<void(int) const, GroupSyncTrait<0>>::Type>::value, "");
}

int main(void)
{
	groupLockType();

	return 0;
}
