//
// SyncType.hpp
//
// Created: 2021-12-07
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_SYNC_HPP)
#define RR_TRAIT_SYNC_HPP

#include <Rr/Util/DefaultConfig.hpp>
#include <Rr/Util/Sync.hpp>
#include <Rr/Trait/Conditional.hpp>
#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/IntegralToType.hpp>

#if RRO_STL_USED
# if __cplusplus > 201402L
#  include <shared_mutex>
# else
#  include <mutex>
# endif
#endif

namespace Rr {
namespace Trait {

struct Empty {};

// Grouped mutex-based read/write locks

template <unsigned Igroup>
struct GroupMutSyncTypesMock {
	using Type = Rr::Util::Sync::MockBasicLockable;
	using ReadLockType = Rr::Util::Sync::LockGuard<Rr::Util::Sync::MockBasicLockable>;
	using WriteLockType = Rr::Util::Sync::LockGuard<Rr::Util::Sync::MockBasicLockable>;
	static constexpr auto kGroup = Igroup;
};

#if RRO_STL_USED
# if __cplusplus > 201402L
template <unsigned Igroup>
struct GroupMutSyncTypes {
	using Type = std::shared_timed_mutex;
	using ReadLockType = std::shared_lock<std::shared_timed_mutex>;
	using WriteLockType = std::unique_lock<std::shared_timed_mutex>;
	static constexpr auto kGroup = Igroup;
}
# else
template <unsigned Igroup>
struct GroupMutSyncTypes {
	using Type = std::mutex;
	using ReadLockType = std::lock_guard<std::mutex>;
	using WriteLockType = std::lock_guard<std::mutex>;
	static constexpr auto kGroup = Igroup;
};
# endif
#else
template <unsigned Igroup>
using GroupMutSyncTypes = GroupMutSyncTypesMock<Igroup>;
#endif

///
/// @brief Mock, extension point for other types of sync types
///
template <class Tsync>
struct IsGroupSync
{
	static constexpr bool value = true;
};

template <class Tsync>
struct SyncType {
	static constexpr auto kIsGroup = IsGroupSync<Tsync>::value;
	using Type = typename Rr::Trait::Conditional<kIsGroup, typename Rr::Util::GroupSync<Tsync, Tsync::kGroup>,
		void>::Type;
	static_assert(!Rr::Trait::IsSame<Type, void>::value, "");
};

template <class Tsync>
struct SyncTraitId {
public:
	enum EnumTraitId {
		NoLock = 0,
		// Mutex-based locks
		GroupUnique,
		IndividualUnique,
		IndividualShared,
		Max,
	};

private:
	template <EnumTraitId Ielt>
	using Value = Rr::Trait::IntegralConstant<EnumTraitId, Ielt>;

	template <unsigned ...>
	struct Sfinae;

	// The following is used to infer the trait by looking at what members are
	// present.

	template <class T>
	static Value<NoLock> getType(...);

	template <class T>
	static Value<IndividualUnique> getType(Sfinae<sizeof(typename T::SyncType) + sizeof(typename T::UniqueLockType)> *);

	template <class T>
	static Value<IndividualShared> getType(Sfinae<sizeof(typename T::SyncType) + sizeof(typename T::UniqueLockType) + sizeof(typename T::SharedLockType)> *);

	template <class T>
	static Value<GroupUnique> getType(Sfinae<sizeof(typename T::GroupLockType) + sizeof(T::groupMutex)> *);

public:
	static constexpr EnumTraitId value = decltype(getType<Tsync>(nullptr))::value;

	template <EnumTraitId Ival, class ...Ta>
	struct ToType {
		static_assert(sizeof...(Ta) == EnumTraitId::Max, "Wrong number of types");
		using Type = typename Rr::Trait::IntegralToType<EnumTraitId, Ival, Ta...>::Type;
	};
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_SYNC_HPP
