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
#include <Rr/Trait/Switch.hpp>

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
	static constexpr auto kIsShared = (kNonSfinaeSyncTraitId == Rr::Trait::SyncTraitId::IndividualShared);
	static constexpr auto kIsMutexBased = Rr::Trait::IntegralIn<decltype(kNonSfinaeSyncTraitId),
		kNonSfinaeSyncTraitId, Rr::Trait::SyncTraitId::IndividualShared, Rr::Trait::SyncTraitId::IndividualUnique,
		Rr::Trait::SyncTraitId::GroupUnique>::value;

	static_assert(kIsMutexBased, "Non mutex-based sync strategies are not supported yet");

	using MutTrait = typename Rr::Trait::AsMutTrait<Tsync>::Type;  // Unified trait format storing user types

	template <Rr::Trait::SyncTraitId ...>
	class Ilist;

	template <class ...>
	class Tlist;

public:
	///
	/// @brief (1) Static storage for group lock, (2) Mock storage, when no sync
	/// is used, (3) MutTrait::Mut infferred from user-provided types in other
	/// cases
	///
	using SyncPrimitiveHolderType = typename Rr::Trait::Switch<Rr::Trait::SyncTraitId, kNonSfinaeSyncTraitId,
		Ilist</* case 1 */Rr::Trait::SyncTraitId::GroupUnique, /* case 2 */Rr::Trait::SyncTraitId::NoSync>,
		Tlist</* then 1 */typename Rr::Util::StaticSyncPrimitiveHolder<Tsync>,
		/* then 2 */typename Rr::Util::GenericMock,
		/* then default */typename Rr::Util::HeapSyncPrimitiveHolder<typename MutTrait::Mut>>>::Type;

	///
	/// @brief Lock for notifying callables.
	///
	/// Read-lock (shared lock) is used for locking const non-group function
	/// signature callables. Mock locks are used for group locks. Unique lock
	/// for any other type.
	///
	using GetNotifyLockType = typename Rr::Trait::Conditional</* if */kIsShared && kIsConstFn,
		typename MutTrait::SharedLock, typename Rr::Trait::Conditional</* elif */kIsGroup, Rr::Util::GenericMock,
		/* else */typename MutTrait::UniqueLock>::Type>::Type;

	///
	/// @brief Just use default MutTrait::Unique lock. It will take mocks into
	/// consideration as well
	///
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
	public CallableWrapperImpl::LockPolicy<Tsignature, Tsync>::SyncPrimitiveHolderType,
	protected ToggleableCallableWrapper<Tsignature>
{

protected:
	using CallableWrapperImpl::LockPolicy<Tsignature, Tsync>::SyncPrimitiveHolderType::getSyncPrimitive;
	using ToggleableCallableWrapper<Tsignature>::setEnabled;

	SyncedCallableWrapper(bool aEnabled, Rr::Util::Callable<Tsignature> &aCallable):
		CallableWrapperImpl::LockPolicy<Tsignature, Tsync>::SyncPrimitiveHolderType{},
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
