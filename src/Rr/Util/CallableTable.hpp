//
// CallableTable.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLBACK_HPP)
#define RR_UTIL_CALLBACK_HPP

#include <Rr/Util/Callable.hpp>
#include <Rr/Trait/Conditional.hpp>
#include <Rr/Trait/EnableIf.hpp>
#include <Rr/Trait/Sync.hpp>
#include <Rr/Trait/LockType.hpp>
#include <Rr/Util/Sync.hpp>

namespace Rr {
namespace Util {

namespace CallableTableImpl {

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
	typename Tsync::Type &mutex;
	GroupSyncedCallableBase(): mutex(GroupSync<Tsync, Igroup>::value) {}
};

///
/// @brief Individual lock variant
///
template <class Tsync>
struct GroupSyncedCallableBase<Tsync, 0> {
	typename Tsync::Type mutex;
};

}  // namespace CallableTableImpl

template <class Tsignature, class Tsync>
class GroupSyncedCallable :
	protected CallableTableImpl::GroupSyncedCallableBase<Tsync, Tsync::kGroup>,
	public Rr::Util::Callable<Tsignature>
{
public:
	using CallableTableImpl::GroupSyncedCallableBase<Tsync, Tsync::kGroup>::GroupSyncedCallableBase;
	using Rr::Util::Callable<Tsignature>::Callable;
	using Rr::Util::Callable<Tsignature>::operator();

	template <class ...TtArgs>
	GroupSyncedCallable(bool aEnabled, TtArgs &&...aArgs):
		CallableTableImpl::GroupSyncedCallableBase<Tsync, Tsync::kGroup>{},
		Callable<Tsignature>(static_cast<TtArgs &&>(aArgs)...)
	{
	}

	template <typename ...TtArgs>
	GroupSyncedCallable(TtArgs &&...aArgs): GroupSyncedCallable(true, static_cast<TtArgs &&>(aArgs)...)
	{
	}
};

template <typename Tsignature, template <class ...> class Tcontainer>
using CallableTable = Tcontainer<Rr::Util::Callable<Tsignature>>;

template <class Tsignature, template<class...> class Tcontainer, class Tsync>
using GroupSyncedCallableTable = Tcontainer<Rr::Util::GroupSyncedCallable<Tsignature, Tsync>>;

///
/// @brief Extension point, if other sync. mechanisms will be used
///
template <class Tsignature, template<class...> class Tcontainer, class Tsync>
struct CallableTableType {
	static constexpr auto kIsGroup = Rr::Trait::IsGroupLock<Tsignature, Tsync>::value;
	using Type = typename Rr::Trait::Conditional<kIsGroup, GroupSyncedCallableTable<Tsignature, Tcontainer, Tsync>, void>::Type;

	static_assert(!Rr::Trait::IsSame<Type, void>::value, "");
};

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_CALLBACK_HPP
