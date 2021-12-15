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

///
/// @brief Policy-defined lock type
///
template <class Tsignature, class Tsync>
struct LockPolicy {
	static constexpr bool kIsGroup = IsGroupSync<Tsync>::value;

	///
	/// @brief The inferred type
	///
	using Type = typename Conditional<kIsGroup, typename LockTypeImpl::GroupLockType<Tsignature, Tsync>::Type, void>::Type;

	static_assert(!IsSame<Type, void>::value, "Unknown lock type");
};

///
/// @brief Useful when it is necessary to provide an exclusive access
///
template <class Tsync>
struct UniqueLockType {
	static constexpr bool kIsGroup = IsGroupSync<Tsync>::value;

	///
	/// @brief The inferred type
	///
	using Type = typename Conditional<kIsGroup, typename Tsync::WriteLockType, void>::Type;
};

template <class Tsync>
struct SharedLockType {
	static constexpr bool kIsGroup = IsGroupSync<Tsync>::value;

	///
	/// @brief The inferred type
	///
	using Type = typename Conditional<kIsGroup, typename Tsync::ReadLockType, void>::Type;
};


}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_LOCKTYPE_HPP

