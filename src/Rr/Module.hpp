//
// Module.hpp
// 
// Author: Dmitry Murashov
//   Mail: dmtrDOTmurashovATgmailDOTcom (gmail.com)
//

#if !defined(RR_SUBSCRIPTION_SUBSCRIPTION_HPP)
#define RR_SUBSCRIPTION_SUBSCRIPTION_HPP

#include <type_traits>
#include <Rr/Util/DefaultConfig.hpp>
#include <Rr/Util/Sync.hpp>

#if RRO_STL_USED
# if __cplusplus < 201402L
#  include <mutex>
# else
#  include <shared_mutex>
# endif
#endif

namespace Rr {
namespace Module {

struct DefaultSyncTrait {
	using MutexType = Rr::Util::Sync::MockSharedMutex;
	using WriteLockType = Rr::Util::Sync::LockGuard<Rr::Util::Sync::MockSharedMutex>;
	using ReadLockType = Rr::Util::Sync::LockGuard<Rr::Util::Sync::MockSharedMutex>;
};

#if RRO_STL_USED
struct Stl11SyncTrait {
	using MutexType = std::mutex;
	using ReadLockType = std::lock_guard<MutexType>;
	using WriteLockType = std::lock_guard<MutexType>;
};

# if __cplusplus > 201402L
struct Stl14SyncTrait {
	using MutexType = std::shared_timed_mutex;
	using ReadLockType = std::shared_lock<MutexType>;
	using WriteLockType = std::shared_lock<MutexType>;
};
# endif

# if __cplusplus > 201402L
using StlSyncTrait = Stl14SyncTrait;
# else
using StlSyncTrait = Stl11SyncTrait;
# endif

#endif

// Useful shortcuts

namespace Util {

template <typename T>
using DecayPreserveConst = std::remove_volatile<typename std::remove_pointer<typename std::remove_reference<T>::type>::type>;

// LockType

template <typename Tinterface, typename TsyncTrait>
struct LockType {
	using Type = typename TsyncTrait::WriteLockType;
};

template <typename Tinterface, typename TsyncTrait>
struct LockType<const Tinterface, TsyncTrait> {
	using Type = typename TsyncTrait::ReadLockType;
};

// The storage for a given type

template <typename Tinterface, typename TsyncTrait>
struct InstanceStorage {
	static Tinterface *instance;
	static typename TsyncTrait::MutexType mutex;
};

template <typename Tinterface, typename TsyncTrait>
Tinterface *InstanceStorage<Tinterface, TsyncTrait>::instance = nullptr;

template <typename Tinterface, typename TsyncTrait>
typename TsyncTrait::MutexType InstanceStorage<Tinterface, TsyncTrait>::mutex;

}  // namespace Util

// Rr::Module::Mod<...>::* implementation

template <typename Tinterface, typename TsyncTrait>
class Mod : private Rr::Module::Util::InstanceStorage<typename std::decay<Tinterface>::type, TsyncTrait> {  // Use `::decay` to prevent from creating multiple instances for different types
public:
	using Rr::Module::Util::InstanceStorage<typename std::decay<Tinterface>::type, TsyncTrait>::InstanceStorage;
	using Rr::Module::Util::InstanceStorage<typename std::decay<Tinterface>::type, TsyncTrait>::mutex;
	using Rr::Module::Util::InstanceStorage<typename std::decay<Tinterface>::type, TsyncTrait>::instance;

	typename Rr::Module::Util::LockType<typename Util::DecayPreserveConst<Tinterface>::type, TsyncTrait>::Type lock;

public:
	Mod();
	bool isValid() const;
	typename Rr::Module::Util::DecayPreserveConst<Tinterface>::type &getInstance();
	static void setInstance(typename std::decay<Tinterface>::type &instance);
};

}  // namespace Module
}  // namespace Rr

#include "Rr/Module.impl"

#endif // RR_SUBSCRIPTION_SUBSCRIPTION_HPP
