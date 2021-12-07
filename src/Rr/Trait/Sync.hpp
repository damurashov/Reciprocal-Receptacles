//
// Sync.hpp
//
// Created: 2021-12-07
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_SYNC_HPP)
#define RR_TRAIT_SYNC_HPP

#include <Rr/Util/DefaultConfig.hpp>

#if RRO_STL_USED
# if __cplusplus > 201402L
#  include <shared_mutex>
# else
#  include <mutex>
# endif
#else
# include <Rr/Util/Sync.hpp>
#endif

namespace Rr {
namespace Trait {

// Grouped mutex-based read/write locks

#if RRO_STL_USED
# if __cplusplus > 201402L
template <unsigned Igroup>
struct TsyncMut {
	using LockType = std::shared_timed_mutex;
	using ReadLockType = std::shared_lock<std::shared_timed_mutex>;
	using WriteLockType = std::unique_lock<std::shared_timed_mutex>;
	static constexpr auto kGroup = Igroup;
}
#else
template <unsigned Igroup>
struct TsyncMut {
	using LockType = std::mutex;
	using ReadLockType = std::lock_guard;
	using WriteLockType = std::lock_guard;
	static constexpr auto kGroup = Igroup;
};
# else
template <unsigned Igroup>
struct TsyncMut {
	using LockType = Rr::Util::Sync::MockBasicLockable;
	using ReadLockType = Rr::Util::Sync::LockGuard;
	using WriteLockType = Rr::Util::Sync::LockGuard;
	static constexpr auto kGroup = Igroup;
};
#endif

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_SYNC_HPP
