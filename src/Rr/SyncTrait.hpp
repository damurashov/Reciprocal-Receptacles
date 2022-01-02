//
// SyncTrait.hpp
//
// Created: 2021-12-17
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

//
// Implements default lock "policy" which defines what and how will be locked.
// It may also serve as an exhaustive example of what format synchronization
// traits should adhere to. This is important because the lock policy is defined
// at compile time, which implies that its determination relies on varioius C++
// generic programming techniques, including SFINAE.
//

/// TODO: add docs on sync. traits

#if !defined(RR_SYNCTRAIT_HPP)
#define RR_SYNCTRAIT_HPP

#include <Rr/Util/GenericMock.hpp>
#include <Rr/Sync/SyncType.hpp>

#if RRO_STL_USED
# if __cplusplus > 201402L
#  include <shared_mutex>
# else
#  include <mutex>
# endif
#endif

namespace Rr {

using Rr::Sync::SyncTraitId;

namespace SyncTraitImpl {

///
/// @brief As for 2021-12-17, depending on implied lock type, sync traits should provide those types
///
template <class Tm, class Tunq, class Tshd, SyncTraitId Isti>
struct MutexTrait {
	static constexpr auto kSyncTraitId = Isti;
	using LockType = Tunq;  ///< RAII lock that locks a mutex in "write mode"
	using SharedLockType = Tshd;  ///< RAII lock that locks a mutex in "read mode"
	using MutexType = Tm;  ///< Type that is compatible with both `LockType` and `SharedLockType`
};

template <class Tm, class Tunq, class Tshd, SyncTraitId Isti, unsigned Igroup = 0>
struct GroupMutexTrait : MutexTrait<Tm, Tunq, Tshd, Isti> {
	static Tm syncPrimitive;
};

template <class Tm, class Tunq, class Tshd, SyncTraitId Isti, unsigned Igroup>
Tm GroupMutexTrait<Tm, Tunq, Tshd, Isti, Igroup>::syncPrimitive;

}  // namespace SyncTraitImpl

// Mock mutex-based sync traits

using MockMutexTrait = SyncTraitImpl::MutexTrait<Rr::Util::GenericMock, Rr::Util::GenericMock, Rr::Util::GenericMock, SyncTraitId::IndividualUnique>;

template <unsigned Igroup = 0>
using MockGroupMutexTrait = typename SyncTraitImpl::GroupMutexTrait<Rr::Util::GenericMock, Rr::Util::GenericMock, Rr::Util::GenericMock, SyncTraitId::GroupUnique, Igroup>;

// Default mutex-based sync traits

#if RRO_STL_USED
# if __cplusplus > 201402L

using DefaultMutexTrait = SyncTraitImpl::MutexTrait<std::shared_timed_mutex, std::unique_lock<std::shared_timed_mutex>, std::shared_lock<std::shared_timed_mutex>, SyncTraitId::IndividualShared>;

template <unsigned Igroup = 0>
using DefaultGroupMutexTrait = typename SyncTraitImpl::GroupMutexTrait<std::shared_timed_mutex, std::unique_lock<std::shared_timed_mutex>, std::shared_lock<std::shared_timed_mutex>, SyncTraitId::GroupUnique, Igroup>;

# else /* C++ 11 */

using DefaultMutexTrait = SyncTraitImpl::MutexTrait<std::mutex, std::unique_lock<std::mutex>, std::unique_lock<std::mutex>, SyncTraitId::IndividualUnique>;

template <unsigned Igroup = 0>
using DefaultGroupMutexTrait = typename SyncTraitImpl::GroupMutexTrait<std::mutex, std::unique_lock<std::mutex>, std::unique_lock<std::mutex>, SyncTraitId::GroupUnique, Igroup>;

# endif
#else // No STL

using DefaultMutexTrait = MockMutexTrait;

template <unsigned Igroup = 0>
using DefaultGroupMutexTrait = MockGroupMutexTrait<Igroup>;

#endif

template <class TgroupMutTrait>
struct GroupMutexLock: Rr::Sync::AsMutTrait<TgroupMutTrait>::Type::UniqueLock {
	using Rr::Sync::AsMutTrait<TgroupMutTrait>::Type::UniqueLock::UniqueLock;
	using Rr::Sync::AsMutTrait<TgroupMutTrait>::Type::UniqueLock::operator=;

	GroupMutexLock(): Rr::Sync::AsMutTrait<TgroupMutTrait>::Type::UniqueLock{TgroupMutTrait::syncPrimitive}
	{
	}
};

}  // namespace Rr

#endif // RR_SYNCTRAIT_HPP
