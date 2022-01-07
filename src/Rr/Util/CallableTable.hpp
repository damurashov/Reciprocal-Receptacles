//
// CallableTable.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLBACK_HPP)
#define RR_UTIL_CALLBACK_HPP

#include <Rr/Util/CallableWrapper.hpp>

namespace Rr {
namespace Util {

template <class Tsignature, template <class ...> class Tcontainer>
using CallableTable = Tcontainer<Rr::Cb::Callable<Tsignature>>;

namespace CallableTableImpl {

template <class Tsignature, class Tsync>
class LockPolicy {
private:
	static constexpr auto kSyncTraitId = Rr::Sync::ToSyncTraitId<Tsync>::value;
	static constexpr auto kNonSfinaeSyncTraitId = Rr::Sync::ToSyncTraitId<Tsync>::kNonSfinaeValue;
	static constexpr auto kIsMutexBased = Rr::Trait::IntegralIn<Rr::Sync::SyncTraitId, /* operand */kSyncTraitId,
		/* list */Rr::Sync::SyncTraitId::IndividualShared, Rr::Sync::SyncTraitId::IndividualUnique,
		Rr::Sync::SyncTraitId::GroupUnique>::value;
	static constexpr auto kIsMock = (kNonSfinaeSyncTraitId == Rr::Sync::SyncTraitId::NoSync);

	static_assert(kIsMutexBased || kIsMock, "Non mutex-based sync types are not supported yet");

	using MutexTrait = typename Rr::Sync::AsMutTrait<Tsync>::Type;

public:
	using SyncPrimitiveType = typename MutexTrait::Mut;
	using UniqueLockType = typename MutexTrait::UniqueLock;
	using SharedLockType = typename Rr::Trait::Conditional</* if */kNonSfinaeSyncTraitId ==
		Rr::Sync::SyncTraitId::IndividualShared, typename MutexTrait::SharedLock, /* else */ typename
		MutexTrait::UniqueLock>::Type;
};

}  // namespace CallableTableImpl

template <class Tsignature, class Ttopic, template <class ...> class Tcontainer, class Tsync>
class SyncedCallableWrapperStaticTable {

	///
	/// @brief Growing container of callables
	///
	static Tcontainer<SyncedCallableWrapper<Tsignature, Tsync>> table;

	static typename CallableTableImpl::LockPolicy<Tsignature, Tsync>::SyncPrimitiveType syncPrimitive;
public:
	///
	/// @brief Lock wrap for iterating over table items()
	///
	static typename Rr::Sync::LockWrap<typename CallableTableImpl::LockPolicy<Tsignature, Tsync>::SharedLockType,
		decltype(table)>
	asSharedLockWrap()
	{
		return {syncPrimitive, table};
	}

	///
	/// @brief Lock wrap for amending the table
	///
	static typename Rr::Sync::LockWrap< typename CallableTableImpl::LockPolicy<Tsignature, Tsync>::UniqueLockType,
		decltype(table)>
	asUniqueLockWrap()
	{
		return {syncPrimitive, table};
	}
};

}  // namespace Util
}  // namespace Rr

#include "CallableTable.impl"

#endif // RR_UTIL_CALLBACK_HPP
