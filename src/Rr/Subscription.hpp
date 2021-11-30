#ifndef RR_SUBSCRIPTION_SUBSCRIPTION_HPP
#define RR_SUBSCRIPTION_SUBSCRIPTION_HPP

#include <Rr/Util/Sync.hpp>
#include <Rr/Util/DefaultConfig.hpp>

// TODO: Offer an alternative to std::list. For example, ETL library https://github.com/ETLCPP/etl
# if !RRO_STL_USED
# error The current version of Rr requires STL.
#endif
#include <list>

#if RRO_STL_USED
# include <mutex>
# include <thread>
#endif

namespace Rr {
namespace Subscription {

namespace Util {

struct Observer {
};

struct DefaultTopic {
};

}  // namespace Util

// Sync traits

template <typename Tmutex, template <typename> typename Tlock>
struct SyncTrait final {
	using MutObserversStorage = Tmutex;         // Type of mutex for static observers storage
	using MutObserver         = Tmutex;         // Type of mutex for particular observers
	using LockObserverStorage = Tlock<Tmutex>;  // RAII sync. for static observers storage
	using LockObserverEnable  = Tlock<Tmutex>;  // RAII sync. for particular observers: lifecycle management
	using LockObserverNotify  = Tlock<Tmutex>;  // RAII sync. for particular observers: on 'notify' call
};

using MockSyncTrait = SyncTrait<Rr::Util::Sync::MockBasicLockable, Rr::Util::Sync::LockGuard>;
using DefaultSyncTrait = MockSyncTrait;

#if RRO_STL_USED
using StlSyncTrait = SyncTrait<std::mutex, std::lock_guard>;
#endif

// Base type

template <typename TopicTrait, typename SyncTraits, typename ...Type>
class KeyBase {
protected:
	using MemberCallable = void (Util::Observer:: *)(Type...);
	using StaticCallable = void (*)(Type...);

	struct ObserverInfo {
		Util::Observer *observer;
		union {
			MemberCallable memberCallable;
			StaticCallable staticCallable;
		};
		typename SyncTraits::MutObserver mutex;
		bool enabled = true;
		ObserverInfo(Util::Observer *aObserver, MemberCallable aCallable, bool aEnabled = true) : observer(aObserver), memberCallable(aCallable), enabled(aEnabled)
		{
		}
		ObserverInfo(StaticCallable aCallable, bool aEnabled = true) : observer(nullptr), staticCallable(aCallable), enabled(aEnabled)
		{
		}
	};

	static struct Observers {
		std::list<ObserverInfo> list;
		typename SyncTraits::MutObserversStorage mutex;
	} observers;

	static std::pair<typename std::list<ObserverInfo>::iterator, typename std::list<ObserverInfo>::iterator> getIterators() {
		typename SyncTraits::LockObserverStorage lock(observers.mutex);
		return {observers.list.begin(), observers.list.end()};
	}

	static ObserverInfo &pushObserver(MemberCallable aObsCall, Util::Observer *aObs, bool aEnabled = true)
	{
		typename SyncTraits::LockObserverStorage lock(observers.mutex);
		observers.list.emplace_back(aObs, aObsCall, aEnabled);
		return observers.list.back();
	}

	static ObserverInfo &pushObserver(StaticCallable aStaticCallable, bool aEnabled = true)
	{
		typename SyncTraits::LockObserverStorage lock(observers.mutex);
		observers.list.emplace_back(aStaticCallable, aEnabled);
		return observers.list.back();
	}

	ObserverInfo *observerInfo = nullptr;

public:
	using Topic = TopicTrait;

	void enableSubscribe(bool aEnabled)
	{
		if (observerInfo) {
			typename SyncTraits::LockObserverEnable lock(observerInfo->mutex);  // Thread-safe, 'coz the entry will not be destructed no matter what
			observerInfo->enabled = aEnabled;
		}
	}

	template <typename InstanceType>
	KeyBase(void (InstanceType:: *callable)(Type...), InstanceType *instance, bool enabled = true)
	{
		observerInfo = &pushObserver((MemberCallable) callable, (Util::Observer *) instance, enabled);
	}

	KeyBase(void (*staticCallable)(Type...), bool enabled = true)
	{
		observerInfo = &pushObserver((StaticCallable) staticCallable, enabled);
	}

	static void notify(Type... args)
	{
		auto iterators = getIterators();  // The returned range won't be changed, therefore there's no reason to syncrhonize access

		for (auto it = iterators.first; it != iterators.second; ++it) {
			if (!it->enabled) {  // Preliminary check to avoid unnecessary lock expenses
				continue;
			}
			typename SyncTraits::LockObserverNotify lock(it->mutex);
			if (!it->enabled) {
				continue;
			}
			auto obs = it->observer;
			if (obs) {  // Member
				auto obsCallable = it->memberCallable;
				(obs->*obsCallable)(args...);
			} else {  // Static
				it->staticCallable(args...);
			}
		}
	}

	KeyBase()
	{
	}

	~KeyBase()
	{
		enableSubscribe(false);
	}
};

template <typename TopicTrait, typename SyncTraits, typename ...Type>
typename KeyBase<TopicTrait, SyncTraits, Type...>::Observers KeyBase<TopicTrait, SyncTraits, Type...>::observers {};

// Key, single argument

template <typename SingleType, typename TopicTrait = Util::DefaultTopic, typename SyncTraits = DefaultSyncTrait>
class Key : public KeyBase<TopicTrait, SyncTraits, SingleType> {
public:
	using Type = SingleType;
	using KeyBase<TopicTrait, SyncTraits, SingleType>::KeyBase;
};

template <typename TopicTrait, typename SyncTraits>
class Key<void, TopicTrait, SyncTraits> : public KeyBase<TopicTrait, SyncTraits> {
public:
	using Type = void;
	using KeyBase<TopicTrait, SyncTraits>::KeyBase;
};

}  // namespace Subscription
}  // namespace Rr

#endif // RR_SUBSCRIPTION_SUBSCRIPTION_HPP
