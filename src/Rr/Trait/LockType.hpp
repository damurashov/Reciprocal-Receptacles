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

namespace Rr {
namespace Trait {

// Group lock traits

///
/// @brief Applies a strategy for inferring an appropriate lock type based on
/// callable signature and \arg `Tsync` trait.
///
/// Strategy: isGroupedLock ? WriteLock : isConst ? ReadLock : WriteLock
///
/// @tparam Tsignature Signature of a callable ( \see Callable.hpp )
/// @tparam Tsync Synchronization traits ( \see Trait/Sync.hpp )
///
template <class Tsignature, class Tsync>
struct GroupLockType {

	static constexpr bool kIsGroup = (Tsync::kGroup != 0);  // 0 stands for individual lock
	static constexpr bool kIsConst = Rr::Trait::Fn<Tsignature>::kIsConst;

	using W = typename Tsync::WriteLockType;
	using R = typename Tsync::ReadLockType;

	using Type = typename Conditional<kIsGroup, W,  // Group lock should always be unique
		typename Conditional<kIsConst, R, W>::Type>::Type;
};

///
/// @brief Mock, extension point for other types of locks
///
template <class Tsignature, class Tsync>
struct IsGroupLock
{
	static constexpr bool value = true;
};

template <class Tsignature, class Tsync>
struct LockType
{
	static constexpr bool kIsGroup = IsGroupLock<Tsignature, Tsync>::value;
	using Type = typename Conditional<kIsGroup, typename GroupLockType<Tsignature, Tsync>::Type, void>::Type;

	static_assert(!IsSame<Type, void>::value, "Unknown lock type");
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_LOCKTYPE_HPP
