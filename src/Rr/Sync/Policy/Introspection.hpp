//
// Introspection.hpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

// Constexpr sync-related introspection helpers

#if !defined(RR_SYNC_POLICY_INTROSPECTION_HPP)
#define RR_SYNC_POLICY_INTROSPECTION_HPP

#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/StoreType.hpp>
#include <Rr/Trait/RemoveReference.hpp>
#include <Rr/Util/GenericMock.hpp>
#include <Rr/Sync/Policy/NoMember.hpp>
#include <Rr/Sync/Policy/LockSfinae.hpp>

namespace Rr {
namespace Sync {
namespace Policy {

namespace IntrospectionImpl {

struct SfinaeFallback {
	template <class T>
	static NoMember type(...);
};

struct Mutex {
	template <class T>
	static void type(typename T::Mutex *);
};

struct Lock {
	template <class T>
	static void type(typename T::Lock *);
};

struct CallPolicy {
	template <class T>
	static void type(decltype(T::kCallPolicy) *);
};

struct Policy {
	template <class T>
	static void type(decltype(T::kPolicy) *);
};

struct SharedAccessPolicy {
	template <class T>
	static void type(decltype(T::kSharedAccessPolicy) *);
};

struct StoragePolicy {
	template <class T>
	static void type(decltype(T::kStoragePolicy) *);
};

template <class Tsfinae>
struct FormSfinae : SfinaeFallback, Tsfinae {
	using Tsfinae::type;
	using SfinaeFallback::type;
};

}  // namespace IntrospectionImpl

template <class Treference, class T>
constexpr bool defines()
{
	using Fs = typename IntrospectionImpl::FormSfinae<Treference>;
	using RetType = decltype(Fs::template type<T>(nullptr));
	using StrippedRetType = Trait::StripTp<RetType>;
	return !Trait::IsSame<StrippedRetType, NoMember>::value;
}

template <class T>
constexpr bool definesMutex()
{
	return defines<IntrospectionImpl::Mutex, T>();
}

template <class T>
constexpr bool definesLock()
{
	return defines<IntrospectionImpl::Lock, T>();
}

template <class T>
constexpr bool definesCallPolicy()
{
	return defines<IntrospectionImpl::CallPolicy, T>();
}

template <class T>
constexpr bool definesPolicy()
{
	return defines<IntrospectionImpl::Policy, T>();
}

template <class T>
constexpr bool definesSharedAccessPolicy()
{
	return defines<IntrospectionImpl::SharedAccessPolicy, T>();
}

template <class T>
constexpr bool definesStoragePolicy()
{
	return defines<IntrospectionImpl::StoragePolicy, T>();
}

template <class T>
constexpr bool definesLockMethod()
{
	constexpr const char *arg = "";
	return !Trait::IsSame<NoMember, decltype(LockSfinae::lock(*const_cast<T *>(reinterpret_cast<const T *const>(arg))))>::value;
}

template <class T>
constexpr bool definesTryLockMethod()
{
	constexpr const char *arg = "";
	return !Trait::IsSame<NoMember, decltype(LockSfinae::tryLock(*const_cast<T *>(reinterpret_cast<const T *const>(arg))))>::value;
}

}  // namespace Policy
}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_POLICY_INTROSPECTION_HPP

