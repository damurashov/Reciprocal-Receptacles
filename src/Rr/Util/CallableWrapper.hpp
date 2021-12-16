//
// CallableWrapper.hpp
//
// Created: 2021-12-14
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLABLEWRAPPER_HPP)
#define RR_UTIL_CALLABLEWRAPPER_HPP

#include <Rr/Util/LockWrap.hpp>
#include <Rr/Trait/LockType.hpp>
#include <Rr/Trait/SyncType.hpp>
#include <Rr/Util/Callable.hpp>
#include <Rr/Trait/IntegralIn.hpp>
#include <Rr/Util/GenericMock.hpp>

namespace Rr {
namespace Util {

namespace CallableWrapperImpl {

template <class Tsignature, class Tsync>
class LockPolicy {
private:
	static constexpr auto kSyncTraitId = Rr::Trait::ToSyncTraitId<Tsync>::value;
	static constexpr auto kNonSfinaeSyncTraitId = Rr::Trait::ToSyncTraitId<Tsync>::kNonSfinaeValue;
	static constexpr auto kIsConstFn = Rr::Trait::Fn<Tsignature>::kIsConst;
	static constexpr auto kIsGroup = (kNonSfinaeSyncTraitId == Rr::Trait::SyncTraitId::GroupUnique);
	static constexpr auto kIsShared = Rr::Trait::IntegralIn<decltype(kNonSfinaeSyncTraitId),
		Rr::Trait::SyncTraitId::IndividualShared>::value;
	static constexpr auto kIsMutexBased = Rr::Trait::IntegralIn<decltype(kNonSfinaeSyncTraitId),
		kNonSfinaeSyncTraitId, Rr::Trait::SyncTraitId::IndividualShared, Rr::Trait::SyncTraitId::IndividualUnique,
		Rr::Trait::SyncTraitId::GroupUnique>::value;

	static_assert(kIsMutexBased, "Non mutex-based sync strategies are not supported yet");

	using MutTrait = typename Rr::Trait::AsMutTrait<Tsync>::Type;  // Unified trait format storing user types

public:
	using SyncPrimitiveHolderType = typename Rr::Trait::Conditional<kIsGroup,
		Rr::Util::StaticSyncPrimitiveHolder<Tsync>, Rr::Util::HeapSyncPrimitiveHolder<typename MutTrait::Mut>>::Type;

	using GetNotifyLockType = typename Rr::Trait::Conditional<kIsShared && kIsConstFn,
		typename MutTrait::SharedLock, typename MutTrait::UniqueLock>::Type;

	using SetEnabledLockType = typename MutTrait::UniqueLock;
};

}  // namespace CallableWrapperImpl

template <class Tsignature>
class ToggleableCallableWrapper {
	bool &enabled;
	typename Rr::Util::Callable<Tsignature> &callable;

protected:
	void setEnabled(bool aEnabled)
	{
		enabled = aEnabled;
	}

public:
	ToggleableCallableWrapper(bool &aEnabled, decltype(callable) &aCallable): enabled(aEnabled), callable(aCallable)
	{
	}

	bool isEnabled()
	{
		return enabled;
	}

	template <class ...Ta>
	typename Rr::Trait::Fn<Tsignature>::ReturnType operator()(Ta &&...aArgs)
	{
		rr_assert(enabled);
		return callable(static_cast<Ta &&>(aArgs)...);
	}
};

template <class Tsignature, class Tsync>
class SyncedCallableWrapper :
	public Rr::Trait::SyncType<Tsync>::Type,
	protected ToggleableCallableWrapper<Tsignature>
{

protected:
	using Rr::Trait::SyncType<Tsync>::Type::getSyncPrimitive;
	using ToggleableCallableWrapper<Tsignature>::setEnabled;

	SyncedCallableWrapper(bool aEnabled, Rr::Util::Callable<Tsignature> &aCallable):
		Rr::Trait::SyncType<Tsync>::Type{},
		ToggleableCallableWrapper<Tsignature>{*(new bool(aEnabled)), aCallable}  // Won't leak, because it is statically stored in a growing-only container, but WARNING: TODO: error-prone solution
	{
	}

public:
	///
	/// @brief Safely locks the wrapped instance using whatever lock type is
	/// defined by its policy, and returns an instance of LockWrap. For lock
	/// policy inference, \see "Trait/LockType.hpp"
	///
	typename Rr::Util::LockWrap<typename Rr::Trait::LockPolicy<Tsignature, Tsync>::Type,
		ToggleableCallableWrapper<Tsignature>> asLockWrap()
	{
		return {getSyncPrimitive(), *this};
	}
};

}  // namespace Util
}  // namespace Rr

#endif // RR_UTIL_CALLABLEWRAPPER_HPP
