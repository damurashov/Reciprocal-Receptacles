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

struct Mutex : SfinaeFallback {
	template <class T>
	static void type(typename T::Mutex *);

	using SfinaeFallback::type;
};

struct Lock : SfinaeFallback {
	using SfinaeFallback::type;

	template <class T>
	static void type(typename T::Lock *);
};

struct CallPolicy : SfinaeFallback {
	using SfinaeFallback::type;

	template <class T>
	static void type(decltype(T::kCallPolicy) *);
};

struct Policy : SfinaeFallback {
	using SfinaeFallback::type;

	template <class T>
	static void type(decltype(T::kPolicy) *);
};

struct SharedAccessPolicy : SfinaeFallback {
	using SfinaeFallback::type;

	template <class T>
	static void type(decltype(T::kSharedAccessPolicy) *);
};

template <class Treference, class T>
constexpr bool defines()
{
	return !Trait::IsSame<decltype(Treference::template type<T>(nullptr)), NoMember>::value;
}

}  // namespace IntrospectionImpl

template <class T>
constexpr bool definesMutex()
{
	return IntrospectionImpl::defines<IntrospectionImpl::Mutex, T>();
}

template <class T>
constexpr bool definesLock()
{
	return IntrospectionImpl::defines<IntrospectionImpl::Lock, T>();
}

template <class T>
constexpr bool definesCallPolicy()
{
	return IntrospectionImpl::defines<IntrospectionImpl::CallPolicy, T>();
}

template <class T>
constexpr bool definesPolicy()
{
	return IntrospectionImpl::defines<IntrospectionImpl::Policy, T>();
}

template <class T>
constexpr bool definesSharedAccessPolicy()
{
	return IntrospectionImpl::defines<IntrospectionImpl::SharedAccessPolicy, T>();
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

