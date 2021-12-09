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
#include <Rr/Trait/Sync.hpp>
#include <Rr/Util/Sync.hpp>

namespace Rr {
namespace Util {

template <class Tsignature, class Tsync>
class SyncedCallable :
	public Rr::Util::Callable<Tsignature>,
	public Rr::Trait::InheritLockType<Tsync>
{
	bool enabled;
public:
	using Rr::Util::Callable<Tsignature>::Callable;
	using Rr::Util::Callable<Tsignature>::operator();
	using Tsync::Type::Type;

	// Helpers for Rr::Util::Lock;
	using LockType = typename Rr::Trait::Conditional<Rr::Trait::Fn<Tsignature>::kConst, typename Tsync::ReadLockType, typename Tsync::WriteLockType>::Type;
	using SyncType = typename Tsync::Type;
	static constexpr auto kGroup = Tsync::kGroup;

	template <class ...Ta>
	SyncedCallable(Ta &&...aArgs) : Callable<Tsignature>(static_cast<Ta &&>(aArgs)...), enabled{true}
	{
	}

	template <class ...Ta>
	SyncedCallable(bool aEnabled, Ta &&...aArgs) : Callable<Tsignature>(static_cast<Ta &&>(aArgs)...), enabled{aEnabled}
	{
	}

	void setEnabled(bool aEnabled)
	{
		enabled = aEnabled;
	}
};

template <typename Tsignature, template <class ...> class Tcontainer>
using CallableTable = Tcontainer<Rr::Util::Callable<Tsignature>>;

template <class Tsignature, template<class...> class Tcontainer, class Tsync = Rr::Trait::TsyncMut<0>>
using SyncedCallableTable = Tcontainer<Rr::Util::SyncedCallable<Tsignature, Tsync>>;

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_CALLBACK_HPP
