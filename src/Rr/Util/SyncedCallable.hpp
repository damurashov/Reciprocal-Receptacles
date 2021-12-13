//
// SyncedCallable.hpp
//
// Created: 2021-12-13
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_SYNCEDCALLABLE_HPP)
#define RR_UTIL_SYNCEDCALLABLE_HPP

#include <Rr/Util/Callable.hpp>
#include <Rr/Trait/Conditional.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/LockType.hpp>

namespace Rr {
namespace Util {

// GroupSyncedCallable

namespace GroupSyncedCallableImpl {

///
/// @brief Static storage for whatever sync primitive is being used
///
/// @tparam Tsync  See Trait/Sync
/// @tparam Igroup If `Igroup != 0`, common (group) lock instance is used
///
template <class Tsync, unsigned Igroup>
struct GroupSync {
	static typename Tsync::Type value;
};

template <class Tsync, unsigned Igroup>
typename Tsync::Type GroupSync<Tsync, Igroup>::value;

///
/// @brief Group lock variant
///
template <class Tsync, unsigned Igroup>
struct GroupSyncedCallableBase {
	typename Tsync::Type &syncPrimitive;
	GroupSyncedCallableBase(): syncPrimitive(GroupSync<Tsync, Igroup>::value) {}
};

///
/// @brief Individual lock variant
///
template <class Tsync>
struct GroupSyncedCallableBase<Tsync, 0> {
	typename Tsync::Type syncPrimitive;
};

}  // namespace GroupSyncedCallableImpl

template <class Tsignature, class Tsync>
class GroupSyncedCallable :
	protected GroupSyncedCallableImpl::GroupSyncedCallableBase<Tsync, Tsync::kGroup>,
	public Rr::Util::Callable<Tsignature>
{
public:
	using GroupSyncedCallableImpl::GroupSyncedCallableBase<Tsync, Tsync::kGroup>::GroupSyncedCallableBase;
	using Rr::Util::Callable<Tsignature>::Callable;
	using Rr::Util::Callable<Tsignature>::operator();

	template <class ...TtArgs>
	GroupSyncedCallable(TtArgs &&...aArgs):
		GroupSyncedCallableImpl::GroupSyncedCallableBase<Tsync, Tsync::kGroup>{},
		Callable<Tsignature>(static_cast<TtArgs &&>(aArgs)...)
	{
	}
};

// SyncedCallableType

template <class Tsignature, class Tsync>
struct SyncedCallableType
{
	static constexpr auto kIsGroup = Rr::Trait::IsGroupSync<Tsync>::value;
	using Type = typename Rr::Trait::Conditional<kIsGroup, GroupSyncedCallable<Tsignature, Tsync>, void>::Type;
	static_assert(!Rr::Trait::IsSame<Type, void>::value, "");
};

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_SYNCEDCALLABLE_HPP
