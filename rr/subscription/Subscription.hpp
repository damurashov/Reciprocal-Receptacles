#ifndef SUBSCRIPTION_HPP
#define SUBSCRIPTION_HPP

#include <list>

// ------------ Thread Safety Macro ------------ //

#if 1
# define SUBSCRIPTION_HPP_ENABLE_THREAD_SAFETY
#endif

#if defined(SUBSCRIPTION_HPP_ENABLE_THREAD_SAFETY)
# include <mutex>
# include <thread>
#endif  /* defined(SUBSCRIPTION_HPP_ENABLE_THREAD_SAFETY) */


// ------------ Topics ------------ //
// Feel free to extend

namespace Topics {
	struct Default {};
}  // namespace Keys


// ------------ Utility ------------ //

namespace Util {

// https://en.cppreference.com/w/cpp/named_req/BasicLockable
struct MockBasicLockable final {
	inline void lock()
	{
	}
	inline void unlock()
	{
	}
};

// Mocks STL's lock_guard:
// https://en.cppreference.com/w/cpp/thread/lock_guard
template <typename Mutex>
struct LockGuard final {
public:
	LockGuard(Mutex &aMutex) : mutex(aMutex)
	{
		mutex.lock();
	}
	~LockGuard()
	{
		mutex.unlock();
	}
private:
	Mutex &mutex;
};


// ------------ SyncTraits ------------ //
//
// Synchronization facilities used by Key<...> Interface requirements are
// defined by RAII facilities:
// - LockObserverEnable
// - LockObserverStorage
//

#if defined(SUBSCRIPTION_HPP_ENABLE_THREAD_SAFETY)
struct SyncTraitsStrict {
	using MutObserversStorage = std::mutex;                           // Type of mutex for static observers storage
	using MutObserver         = std::mutex;                           // Type of mutex for particular observers
	using LockObserverStorage = std::lock_guard<MutObserversStorage>; // RAII sync. for static observers storage
	using LockObserverEnable  = std::lock_guard<MutObserver>;         // RAII sync. for particular observers: lifecycle management
	using LockObserverNotify  = std::lock_guard<MutObserver>;         // RAII sync. for particular observers: on 'notify' call
};
#endif /* #if defined(SUBSCRIPTION_HPP_ENABLE_THREAD_SAFETY) */
struct SyncTraitsMock {
	using MutObserversStorage = MockBasicLockable;
	using MutObserver         = MockBasicLockable;
	using LockObserverStorage = LockGuard<MutObserversStorage>;
	using LockObserverEnable  = LockGuard<MutObserver>;
	using LockObserverNotify  = LockGuard<MutObserver>;
};

// Mock class serving as pseudo-type for subscribers
struct Observer {
};

}  // namespace Util


// ------------ Key ------------ //

#if defined(SUBSCRIPTION_HPP_ENABLE_THREAD_SAFETY)
using DefaultSyncTraits = Util::SyncTraitsStrict;
#else
using DefaultSyncTraits = Util::SyncTraitsMock;
#endif

// ------------ KeyBase (multiargument) ------------ //


template <typename Topic, typename SyncTraits, typename ...Type>
class KeyBase {
protected:
	using MemberCallable = void (Util::Observer:: *)(Type...);
	struct ObserverInfo {
		Util::Observer *observer;
		MemberCallable callable;
		typename SyncTraits::MutObserver mutex;
		bool enabled = true;
		ObserverInfo(Util::Observer *aObserver, MemberCallable aCallable) : observer(aObserver), callable(aCallable)
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

	static ObserverInfo &pushObserver(MemberCallable aObsCall, Util::Observer *aObs)
	{
		typename SyncTraits::LockObserverStorage lock(observers.mutex);
		observers.list.emplace_back(aObs, aObsCall);
		return observers.list.back();
	}

	ObserverInfo *observerInfo = nullptr;

public:
	void enableSubscribe(bool aEnabled)
	{
		if (observerInfo) {
			typename SyncTraits::LockObserverEnable lock(observerInfo->mutex);  // Thread-safe, 'coz the entry will not be destructed no matter what
			observerInfo->enabled = aEnabled;
		}
	}

	template <typename InstanceType>
	KeyBase(void (InstanceType:: *callable)(Type...), InstanceType *instance)
	{
		observerInfo = &pushObserver((MemberCallable) callable, (Util::Observer *) instance);
	}

	void notify(Type... args)
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
			auto obsCallable = it->callable;
			(obs->*obsCallable)(args...);
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

template <typename Topic, typename SyncTraits, typename ...Type>
typename KeyBase<Topic, SyncTraits, Type...>::Observers KeyBase<Topic, SyncTraits, Type...>::observers {};

// ------------ Key (single argument) ------------ //

template <typename SingleType, typename Topic = Topics::Default, typename SyncTraits = DefaultSyncTraits>
class Key : public KeyBase<Topic, SyncTraits, SingleType> {
public:
	using KeyBase<Topic, SyncTraits, SingleType>::KeyBase;
};

template <typename Topic, typename SyncTraits>
class Key<void, Topic, SyncTraits> : public KeyBase<Topic, SyncTraits> {
public:
	using KeyBase<Topic, SyncTraits>::KeyBase;
};

#endif // SUBSCRIPTION_HPP
