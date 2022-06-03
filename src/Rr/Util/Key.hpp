//
// Key.hpp
//
// Created on:  Feb 12, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_UTIL_KEY_HPP)
#define RR_UTIL_KEY_HPP

#include <Rr/Sync/Storage.hpp>
#include <Rr/Cb/Callable.hpp>
#include <Rr/Trait/MemberDecay.hpp>
#include <Rr/Trait/Forward.hpp>
#include <Rr/Trait/IntegralToType.hpp>

namespace Rr {
namespace Util {
namespace KeyImpl {

template <class Tsignature, class TsyncTrait, template <class ...>class TcallableContainer, class TargList, class Ttopic>
class Key;

template <class Tsignature, class TsyncTrait, template <class ...> class TcallableContainer, class ...Targs,
	template <class...> class TargsList, class Ttopic>
class Key<Tsignature, TsyncTrait, TcallableContainer, TargsList<Targs...>, Ttopic> : public Rr::Cb::Callable<Tsignature> {
private:
	static Rr::Sync::Storage<Rr::Cb::Callable<Tsignature>, TsyncTrait, TcallableContainer> storage;
	typename Rr::Sync::Storage<Rr::Cb::Callable<Tsignature>, TsyncTrait, TcallableContainer>::Value &sharedAccess;

public:
	template <int Iarg>
	using Arg = typename Rr::Trait::IntToType<Iarg, Targs...>::Type;

	using Ret = typename Rr::Trait::MemberDecay<Tsignature>::ReturnType;

	Key(typename Rr::Trait::MemberDecay<Tsignature>::StaticCallbackType aCallback, bool aEnabled = true) :
		Rr::Cb::Callable<Tsignature>{Rr::Trait::forward<
		typename Rr::Trait::MemberDecay<Tsignature>::StaticCallbackType>(aCallback)},
		sharedAccess{storage.reg(*this, aEnabled)}
	{
	}

	template <class Tinstance>
	Key(typename Rr::Trait::MemberDecay<Tsignature, Rr::Trait::StripTp<Tinstance>>::CallbackType aCallback,
		Tinstance aInstance,
		bool aEnabled = true,
		typename Rr::Trait::EnableIf<!Rr::Trait::IsSame<Tinstance, Rr::Trait::StripTp<Tinstance>>::value>::Type * = nullptr) :
		Rr::Cb::Callable<Tsignature> {
			Rr::Trait::forward<typename Rr::Trait::MemberDecay<Tsignature, Rr::Trait::StripTp<Tinstance>>::CallbackType>(
				aCallback),
			Rr::Trait::forward<Tinstance>(aInstance)
		},
		sharedAccess{storage.reg(*this, aEnabled)}
	{
	}

	~Key()
	{
		setEnabled(false);
	}

	static auto getIterators() -> decltype(storage.getIterators())
	{
		return storage.getIterators();
	}

	void setEnabled(bool aEnabled)
	{
		Rr::Sync::Policy::SharedAccess<TsyncTrait>::PrimitiveOps::lock(sharedAccess.primitive);
		sharedAccess.stored = (aEnabled ? this : nullptr);
		Rr::Sync::Policy::SharedAccess<TsyncTrait>::PrimitiveOps::unlock(sharedAccess.primitive);
	}

	static void notify(Targs ...aArgs)
	{
		for (auto &callable : storage.getIterators()) {
			callable(Rr::Trait::forward<Targs>(aArgs)...);
		}
	}

	/// \brief Notify every subscriber, feedbacking the caller on each response using a provided callback.
	///
	template <class Tcb>
	static void notifyCb(Targs ...aArgs, Tcb &&aCb)
	{
		for (auto &callable : storage.getIterators()) {
			aCb(callable(Rr::Trait::forward<Targs>(aArgs)...));
		}
	}

	/// \brief Notify N first subscribers.
	///
	/// Useful for implementing something like a module framework, where only 1 or N responses are required or expected.
	///
	template <class Tcb>
	static void notifyCbn(Targs ...aArgs, Tcb &&aCb, unsigned aN)
	{
		for (auto &callable : storage.getIterators()) {
			aCb(callable(Rr::Trait::forward<Targs>(aArgs)...));

			if (0 == --aN) {
				break;
			}
		}
	}
};

template <class Tsignature, class TsyncTrait, template <class ...> class TcallableContainer, class ...Targs,
	template <class...> class TargsList, class Ttopic>
Rr::Sync::Storage<Rr::Cb::Callable<Tsignature>, TsyncTrait, TcallableContainer>
Key<Tsignature, TsyncTrait, TcallableContainer, TargsList<Targs...>, Ttopic>::storage;

}  // namespace KeyImpl

template <class Tsignature, class TsyncTrait, template <class ...> class TcallableContainer, class Ttopic=void>
struct Key : KeyImpl::Key<Tsignature, TsyncTrait, TcallableContainer, typename Rr::Trait::MemberDecay<Tsignature>::ArgsList, Ttopic> {
private:
	using Base = typename KeyImpl::Key<Tsignature, TsyncTrait, TcallableContainer,
		typename Rr::Trait::MemberDecay<Tsignature>::ArgsList, Ttopic>;

public:
	using Base::Base;
	using Base::getIterators;
	using Base::setEnabled;
	using Base::notify;
	using Base::Arg;
	using Base::Ret;
};

template <class Tsignature, class TsyncTrait, template <class ...> class TcallableContainer, class Ttopic=void>
struct LegacyKey {
private:
	using KeyType = typename KeyImpl::Key<Tsignature, TsyncTrait, TcallableContainer,
		typename Rr::Trait::MemberDecay<Tsignature>::ArgsList, Ttopic>;
	KeyType *key;
public:

	LegacyKey(typename Rr::Trait::MemberDecay<Tsignature>::StaticCallbackType aCallback, bool aEnabled = true)
	{
		key = new KeyType{aCallback, aEnabled};
	}

	template <class Tinstance>
	LegacyKey(typename Rr::Trait::MemberDecay<Tsignature, Rr::Trait::StripTp<Tinstance>>::CallbackType aCallback,
		Tinstance aInstance, bool aEnabled = true, typename Rr::Trait::EnableIf<!Rr::Trait::IsSame<Tinstance,
		Rr::Trait::StripTp<Tinstance>>::value>::Type * = nullptr)
	{
		key = new KeyType{aCallback, aInstance, aEnabled};
	}

	LegacyKey() : key{nullptr}
	{
	}

	~LegacyKey()
	{
		if (key) {
			delete key;
		}
	}

	inline void enableSubscribe(bool aEnabled)
	{
		rr_assert(key != nullptr);
		key->setEnabled(aEnabled);
	}

	template <class ...Ta>
	static void notify(Ta &&...aArgs)
	{
		KeyType::notify(Rr::Trait::forward<Ta>(aArgs)...);
	}

	using Type = typename KeyType::template Arg<0>;
};

}  // namespace Util
}  // namespace Rr

#endif  // RR_UTIL_KEY_HPP
