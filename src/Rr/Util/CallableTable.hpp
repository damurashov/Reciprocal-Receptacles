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
using CallableTable = Tcontainer<Rr::Util::Callable<Tsignature>>;

namespace CallableTableImpl {

template <class Tsignature, class Tsync>
class LockPolicy {
private:
	static constexpr auto kSyncTraitId = Rr::Trait::ToSyncTraitId<Tsync>::value;
	static constexpr auto kNonSfinaeSyncTraitId = Rr::Trait::ToSyncTraitId<Tsync>::kNonSfinaeValue;
	static constexpr auto kIsMutexBased = Rr::Trait::IntegralIn<Rr::Trait::SyncTraitId, /* operand */kSyncTraitId,
		/* list */Rr::Trait::SyncTraitId::IndividualShared, Rr::Trait::SyncTraitId::IndividualUnique,
		Rr::Trait::SyncTraitId::GroupUnique>::value;
	static constexpr auto kIsMock = (kNonSfinaeSyncTraitId == Rr::Trait::SyncTraitId::NoSync);

	static_assert(kIsMutexBased || kIsMock, "Non mutex-based sync types are not supported yet");

	using MutType = typename Rr::Trait::AsMutTrait<Tsync>::Type;

	template <class ...>
	struct Tlist;

	template <Rr::Trait::SyncTraitId ...>
	struct Ilist;

public:
	using UniqueLockType = typename MutType::UniqueLock;
	using SharedLockType = typename Rr::Trait::Conditional</* if */kNonSfinaeSyncTraitId ==
		Rr::Trait::SyncTraitId::IndividualShared, typename MutType::SharedLock, /* else */ typename
		MutType::UniqueLock>::Type;
};

}  // namespace CallableTableImpl

template <class Tsignature, class Ttopic, template <class ...> class Tcontainer, class Tsync>
class SyncedCallableWrapperStaticTable {

	///
	/// @brief Growing container of callables
	///
	static Tcontainer<SyncedCallableWrapper<Tsignature, Tsync>> table;

	static typename Tsync::Type syncPrimitive;
public:
	///
	/// @brief Lock wrap for iterating over table items()
	///
	static typename Rr::Util::LockWrap<typename CallableTableImpl::LockPolicy<Tsignature, Tsync>::SharedLockType,
		decltype(table)>
	asSharedLockWrap()
	{
		return {syncPrimitive, table};
	}

	///
	/// @brief Lock wrap for amending the table
	///
	static typename Rr::Util::LockWrap< typename CallableTableImpl::LockPolicy<Tsignature, Tsync>::UniqueLockType,
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
