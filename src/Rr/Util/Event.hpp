//
// Key.hpp
//
// Created on:  Feb 12, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_UTIL_EVENT_HPP)
#define RR_UTIL_EVENT_HPP

#include <Rr/Cb/Callable.hpp>
#include <Rr/Trait/MemberDecay.hpp>
#include <Rr/Trait/Forward.hpp>
#include <Rr/Trait/IntegralToType.hpp>
#include <Rr/Cont/PointerStorage.hpp>
#include <Rr/Sync/SharedMutex.hpp>

namespace Rr {
namespace Util {
namespace EventImpl {

template <class Tsignature, class TsyncTrait, class TargList, class Ttopic>
class Event;

/// Implements event key-like behavior: stores function and method callbacks in
/// a notification queue, and provides an API for invoking those.
template <class Tsignature, class TsyncTrait, class ...Targs, template <class...> class TargsList, class Ttopic>
class Event<Tsignature, TsyncTrait, TargsList<Targs...>, Ttopic> : public Rr::Cb::Callable<Tsignature> {
private:
	/// Shorthand alias
	using SelfType = Event<Tsignature, TsyncTrait, TargsList<Targs...>, Ttopic>;

public:
	/// Get argument type by id.
	template <int Iarg>
	using Arg = typename Rr::Trait::IntToType<Iarg, Targs...>::Type;

	/// Get function return type
	using Ret = typename Rr::Trait::MemberDecay<Tsignature>::ReturnType;

public:
	/// Function callback constructor
	Event(typename Rr::Trait::MemberDecay<Tsignature>::StaticCallbackType aCallback, bool aEnabled = true) :
		Rr::Cb::Callable<Tsignature>{Rr::Trait::forward<
		typename Rr::Trait::MemberDecay<Tsignature>::StaticCallbackType>(aCallback)}
	{
		setEnabled(aEnabled);
	}

	/// Method callback constructor
	template <class Tinstance>
	Event(typename Rr::Trait::MemberDecay<Tsignature, Rr::Trait::StripTp<Tinstance>>::CallbackType aCallback,
			Tinstance aInstance, bool aEnabled = true,
			typename Rr::Trait::EnableIf<!Rr::Trait::IsSame<Tinstance, Rr::Trait::StripTp<Tinstance>>::value>::Type * = nullptr) :
		Rr::Cb::Callable<Tsignature> {
			Rr::Trait::forward<typename Rr::Trait::MemberDecay<Tsignature, Rr::Trait::StripTp<Tinstance>>::CallbackType>(
				aCallback),
			Rr::Trait::forward<Tinstance>(aInstance)
		}
	{
		setEnabled(aEnabled);
	}

	/// Add the event key into the notification queue
	void setEnabled(bool aEnabled)
	{
		mutex.lock();

		if (aEnabled) {
			pointerStorage.tryAdd(static_cast<void *>(this));
		} else {
			pointerStorage.tryRemove(static_cast<void *>(this));
		}

		mutex.unlock();
	}

	/// Notify subscribers
	static void notify(Targs ...aArgs)
	{
		mutex.lockShared();

		for (void *callable : pointerStorage) {
			(*static_cast<SelfType *>(callable))(Rr::Trait::forward<Targs>(aArgs)...);
		}

		mutex.unlockShared();
	}

	~Event()
	{
		setEnabled(false);
	}

private:
	static typename Rr::Sync::template SharedMutex<typename TsyncTrait::MutexType> mutex;

	/// Notification queue
	static typename Rr::Cont::template PointerStorage<typename TsyncTrait::template CallableContainerType<void *>> pointerStorage;
};

template <class Tsignature, class TsyncTrait, class ...Targs, template <class...> class TargsList, class Ttopic>
Rr::Sync::SharedMutex<typename TsyncTrait::MutexType> Event<Tsignature, TsyncTrait, TargsList<Targs...>, Ttopic>::mutex;

template <class Tsignature, class TsyncTrait, class ...Targs, template <class...> class TargsList, class Ttopic>
Rr::Cont::PointerStorage<typename  TsyncTrait::template CallableContainerType<void *>> Event<Tsignature, TsyncTrait, TargsList<Targs...>, Ttopic>::pointerStorage;

}  // EventImpl

/// \tparam Ttopic - placeholder type, a marker, helps to discern between static storages
template <class Tsignature, class TsyncTrait, class Ttopic=void>
struct Event : EventImpl::Event<Tsignature, TsyncTrait, typename Rr::Trait::MemberDecay<Tsignature>::ArgsList, Ttopic> {
private:
	using Base = typename EventImpl::Event<Tsignature, TsyncTrait,
		typename Rr::Trait::MemberDecay<Tsignature>::ArgsList, Ttopic>;

public:
	using Base::Base;
	using Base::setEnabled;
	using Base::notify;
	using Base::Arg;
	using Base::Ret;
};

}  // Util
}  // Rr

#endif  // RR_UTIL_EVENT_HPP
