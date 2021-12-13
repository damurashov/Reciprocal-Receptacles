//
// Sync.hpp
//
// Created: 2021-12-07
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_SYNC_HPP)
#define RR_TRAIT_SYNC_HPP

#include <Rr/Util/DefaultConfig.hpp>
#include <Rr/Util/Sync.hpp>

#if RRO_STL_USED
# if __cplusplus > 201402L
#  include <shared_mutex>
# else
#  include <mutex>
# endif
#endif

namespace Rr {
namespace Trait {

struct Empty {};

// Grouped mutex-based read/write locks

template <unsigned Igroup>
struct GroupSyncMutTypesMock {
	using Type = Rr::Util::Sync::MockBasicLockable;
	using ReadLockType = Rr::Util::Sync::LockGuard<Rr::Util::Sync::MockBasicLockable>;
	using WriteLockType = Rr::Util::Sync::LockGuard<Rr::Util::Sync::MockBasicLockable>;
	static constexpr auto kGroup = Igroup;
};

#if RRO_STL_USED
# if __cplusplus > 201402L
template <unsigned Igroup>
struct GroupSyncMutTypes {
	using Type = std::shared_timed_mutex;
	using ReadLockType = std::shared_lock<std::shared_timed_mutex>;
	using WriteLockType = std::unique_lock<std::shared_timed_mutex>;
	static constexpr auto kGroup = Igroup;
}
# else
template <unsigned Igroup>
struct GroupSyncMutTypes {
	using Type = std::mutex;
	using ReadLockType = std::lock_guard<std::mutex>;
	using WriteLockType = std::lock_guard<std::mutex>;
	static constexpr auto kGroup = Igroup;
};
# endif
#else
template <unsigned Igroup>
using GroupSyncMutTypes = GroupSyncMutTypesMock<Igroup>;
#endif

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_SYNC_HPP
