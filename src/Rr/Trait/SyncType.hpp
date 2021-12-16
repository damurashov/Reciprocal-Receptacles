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
#include <Rr/Util/GenericMock.hpp>

#if RRO_STL_USED
# if __cplusplus > 201402L
#  include <shared_mutex>
# else
#  include <mutex>
# endif
#endif

namespace Rr {
namespace Trait {

///
/// @brief A kind of a synchronization primitive that is being used
///
enum class SyncTraitId : unsigned {
	NoSync = 0,

	// Mutex-based locks, implicitly specified by user
	IndividualUnique,
	IndividualShared,
	GroupUnique,

	// Mutex-based locks, inferred through use of SFINAE
	SfinaeIndividualUnique,
	SfinaeIndividualShared,
	SfinaeGroupUnique,

	Unknown,
};

///
/// @brief Mock, extension point for other types of sync types
///
template <class Tsync>
struct IsGroupSync
{
	static constexpr bool value = true;
};

template <class Tsync>
struct ToSyncTraitId;

///
/// @brief Obsolete
///
template <class Tsync>
class SyncType {
public:
	static constexpr auto kIsGroup = IsGroupSync<Tsync>::value;
	using Type = typename Rr::Trait::Conditional<kIsGroup, typename Rr::Util::GroupSync<Tsync, Tsync::kGroup>,
		void>::Type;
	static_assert(!Rr::Trait::IsSame<Type, void>::value, "");
};

// AsMutTrait

///
/// @brief Hidden implementation details of `AsMutTrait<...>` converter
///
namespace AsMutTraitImpl {

template <class Tm, class Tlu, class Tls>
struct AsMutTrait {
	using Mut = Tm;
	using LockUnique = Tlu;
	using LockShared = Tls;
};

template <class Tsync>
struct Mock : AsMutTrait<Rr::Util::GenericMock, Rr::Util::GenericMock, Rr::Util::GenericMock> {
};

template <class Tsync>
struct InUnique : AsMutTrait<typename Tsync::MutexType, typename Tsync::LockType, void> {
};

template <class Tsync>
struct InShared : AsMutTrait<typename Tsync::MutexType, typename Tsync::LockType, typename Tsync::SharedLockType> {
};

template <class Tsync>
struct GrUnique : AsMutTrait<decltype(Tsync::mutexInstance), typename Tsync::LockType, void> {
};

template <class Tsync>
struct SfinaeInUnique : AsMutTrait<typename Tsync::UniqueMutexType, typename Tsync::LockType, void> {
};

template <class Tsync>
struct SfinaeInShared : AsMutTrait<typename Tsync::SharedMutexType, typename Tsync::LockType, typename Tsync::SharedLockType> {
};

template <class Tsync>
struct SfinaeGrUnique : AsMutTrait<decltype(Tsync::mutexInstance), typename Tsync::GroupUniqueLockType, void> {
};

}  // namespace AsMutTraitImpl

///
/// @brief Converts a trait to a mutex-based trait
///
/// @tparam Tsync Synchronization trait
/// @tparam Ival  Target type it should be converted to
///
template <class Tsync, SyncTraitId Ival = ToSyncTraitId<Tsync>::value>
using AsMutTrait = typename Rr::Trait::IntegralToType<SyncTraitId, Ival,
	typename AsMutTraitImpl::Mock<Tsync>,
	typename AsMutTraitImpl::InUnique<Tsync>,
	typename AsMutTraitImpl::InShared<Tsync>,
	typename AsMutTraitImpl::GrUnique<Tsync>,
	typename AsMutTraitImpl::SfinaeInUnique<Tsync>,
	typename AsMutTraitImpl::SfinaeInShared<Tsync>,
	typename AsMutTraitImpl::SfinaeGrUnique<Tsync>>::Type;

///
/// @brief Infers the type of a requested synchronization strategy by the
/// structure of a type it is provided with.
///
/// @tparam Tsync A type that has certain fields among its public members, i.e.
/// adhering to certain naming conventions. see getType(1)
///
template <class Tsync>
struct ToSyncTraitId {
private:
	template <SyncTraitId Ival, SyncTraitId InonSfinaeVal>
	struct Value {
		static constexpr auto value = Ival;
		static constexpr auto kNonSfinaeValue = InonSfinaeVal;
	};

	template <class T>
	struct UserValue {
		static constexpr auto value = T::kSyncTraitId;
		static constexpr auto kNonSfinaeValue = value;
	};

	template <unsigned ...>
	struct Sfinae;

	// The following is used to infer the synchronization strategy by looking at
	// what members are present. If no synchronization is used, the user must
	// specify that explicitly by adding `constexpr static auto kSyncTraitId =
	// Rr::SyncTraitId::NoSync`

	template <class T>
	static Value<SyncTraitId::Unknown, SyncTraitId::Unknown> getType(...);  ///< SFINAE-fallback

	template <class T>
	static UserValue<T> getType(Sfinae<sizeof(T::kSyncTraitId)> *);  ///< The strategy has been explicitly specified by user

	template <class T>
	static Value<SyncTraitId::SfinaeIndividualUnique, SyncTraitId::IndividualUnique> getType(Sfinae<sizeof(typename T::UniqueMutexType)> *);  ///< Individual lock through unique mutex

	template <class T>
	static Value<SyncTraitId::SfinaeIndividualShared, SyncTraitId::IndividualShared> getType(Sfinae<sizeof(typename T::SharedMutexType)> *);  ///< Individual lock through shared mutex

	template <class T>
	static Value<SyncTraitId::SfinaeGroupUnique, SyncTraitId::GroupUnique> getType(Sfinae<sizeof(typename T::GroupUniqueLockType)> *);  ///< Group lock through unique mutex

public:
	static constexpr SyncTraitId value = decltype(getType<Tsync>(nullptr))::value;
	static constexpr SyncTraitId kNonSfinaeValue = decltype(getType<Tsync>(nullptr))::kNonSfinaeValue;
	static_assert(value != SyncTraitId::Unknown, "Unknown trait");
};

// Grouped mutex-based read/write locks

template <unsigned Igroup>
struct GroupMutSyncTypesMock {
	// Legacy
	using Type = Rr::Util::GenericMock;
	using ReadLockType = Rr::Util::GenericMock;
	using WriteLockType = Rr::Util::GenericMock;
	static constexpr auto kGroup = Igroup;

	static constexpr auto kSyncTraitId = SyncTraitId::NoSync;
};

#if RRO_STL_USED
# if __cplusplus > 201402L
template <unsigned Igroup>
struct GroupMutSyncTypes {
	// Legacy
	using Type = std::shared_timed_mutex;
	using ReadLockType = std::shared_lock<std::shared_timed_mutex>;
	using WriteLockType = std::unique_lock<std::shared_timed_mutex>;
	static constexpr auto kGroup = Igroup;

	using MutexType = Type;
	using LockType = WriteLockType;
	using SharedLockType = ReadLockType;
	static typename Rr::Trait::Conditional<Igroup == 0, typename Rr::Util::GenericMock, Type>::Type mutexInstance;
	static constexpr auto kSyncTraitId = Rr::Trait::Conditional<Igroup == 0,
		Rr::Trait::IntegralConstant<SyncTraitId, SyncTraitId::IndividualShared>,
		Rr::Trait::IntegralConstant<SyncTraitId, SyncTraitId::GroupUnique>>::Type::value;
}
# else
template <unsigned Igroup>
struct GroupMutSyncTypes {
	// Legacy
	using Type = std::mutex;
	using ReadLockType = std::lock_guard<std::mutex>;
	using WriteLockType = std::lock_guard<std::mutex>;
	static constexpr auto kGroup = Igroup;

	using MutexType = Type;
	using LockType = WriteLockType;
	using SharedLockType = ReadLockType;
	static typename Rr::Trait::Conditional<Igroup == 0, typename Rr::Util::GenericMock, Type>::Type mutexInstance;
	static constexpr auto kSyncTraitId = Rr::Trait::Conditional<Igroup == 0,
		Rr::Trait::IntegralConstant<SyncTraitId, SyncTraitId::IndividualUnique>,
		Rr::Trait::IntegralConstant<SyncTraitId, SyncTraitId::GroupUnique>>::Type::value;
};
# endif
#else
template <unsigned Igroup>
using GroupMutSyncTypes = GroupMutSyncTypesMock<Igroup>;
#endif

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_SYNC_HPP
