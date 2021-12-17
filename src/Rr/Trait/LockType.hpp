//
// LockType.hpp
//
// Created: 2021-12-13
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_LOCKTYPE_HPP)
#define RR_TRAIT_LOCKTYPE_HPP

#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/Conditional.hpp>
#include <Rr/Trait/Fn.hpp>
#include <Rr/Trait/SyncType.hpp>

namespace Rr {
namespace Trait {

namespace LockTypeImpl {

// Group lock traits

///
/// @brief Applies a policy for inferring an appropriate lock type based on
/// callable signature and \arg `Tsync` trait.
///
/// @tparam Tsignature Signature of a callable ( \see Callable.hpp )
/// @tparam Tsync Synchronization traits ( \see Trait/SyncType.hpp )
///
template <class Tsignature, class Tsync>
struct GroupLockType {

	static constexpr bool kIsGroup = (Tsync::kGroup != 0);  // 0 stands for individual lock
	static constexpr bool kIsConst = Rr::Trait::Fn<Tsignature>::kIsConst;

	using W = typename Tsync::WriteLockType;
	using R = typename Tsync::ReadLockType;

	// isGroupedLock ? WriteLock : isConst ? ReadLock : WriteLock
	using Type = typename Conditional<kIsGroup, W,  // Group lock should always be unique
		typename Conditional<kIsConst, R, W>::Type>::Type;
};

}  // namespace LockTypeImpl


}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_LOCKTYPE_HPP

