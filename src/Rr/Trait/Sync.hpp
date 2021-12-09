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

///
/// @brief Inheritance helper. Returns mock, if Tsync::kGroup==0, i.e. the type
/// is subject to group lock, not individual lock. This is used to spare memory
/// through not
///
/// @tparam Tsync. Trait having ::Type, ::ReadLockType, and ::WriteLockType as
/// nested types
///
template <typename Tsync>
using InheritLockType = typename Rr::Trait::Conditional<Tsync::kGroup == 0, typename Tsync::Type, Empty>::Type;

// Grouped mutex-based read/write locks

template <unsigned Igroup>
struct TsyncMutMock {
	using Type = Rr::Util::Sync::MockBasicLockable;
	using ReadLockType = Rr::Util::Sync::LockGuard<Rr::Util::Sync::MockBasicLockable>;
	using WriteLockType = Rr::Util::Sync::LockGuard<Rr::Util::Sync::MockBasicLockable>;
	static constexpr auto kGroup = Igroup;
};

#if RRO_STL_USED
# if __cplusplus > 201402L
template <unsigned Igroup>
struct TsyncMut {
	using Type = std::shared_timed_mutex;
	using ReadLockType = std::shared_lock<std::shared_timed_mutex>;
	using WriteLockType = std::unique_lock<std::shared_timed_mutex>;
	static constexpr auto kGroup = Igroup;
}
# else
template <unsigned Igroup>
struct TsyncMut {
	using Type = std::mutex;
	using ReadLockType = std::lock_guard<std::mutex>;
	using WriteLockType = std::lock_guard<std::mutex>;
	static constexpr auto kGroup = Igroup;
};
# endif
#else
template <unsigned Igroup>
using TsyncMut = TsyncMutMock<Igroup>;
#endif

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_SYNC_HPP
