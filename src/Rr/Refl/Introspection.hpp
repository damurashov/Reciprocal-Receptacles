//
// Introspection.hpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

// Constexpr sync-related introspection helpers

#if !defined(RR_REFL_INTROSPECTION_HPP)
#define RR_REFL_INTROSPECTION_HPP

#include <Rr/Trait/IsSame.hpp>
#include <Rr/Trait/StoreType.hpp>
#include <Rr/Trait/RemoveReference.hpp>
#include <Rr/Trait/Declval.hpp>
#include <Rr/Util/GenericMock.hpp>
#include <Rr/Refl/NoMember.hpp>
#include <Rr/Refl/CallSfinae.hpp>

namespace Rr {
namespace Refl {

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

///
/// @brief Extensible wrapper over SFINAE mechanism
///
/// Usage:
///
/// @code
/// struct IdoHaveSuchMemberOrType {bool someMember;};
/// struct SomeMemberSfinae {template <class T> static void type(decltype(T::someMember) *); };
/// auto fDefinesTypeOrMember = Rr::Sync::Policy::Defines<SomeMemberSfinae, IdoHaveSuchMemberOrType>();
/// @endcode
///
/// @tparam Treference SfinaeFallback
/// @tparam T          Class or struct to be checked
///
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
	return !Trait::IsSame<NoMember, decltype(CallSfinae::lock(Trait::eDeclval<T>()))>::value;
}

template <class T>
constexpr bool definesTryLockMethod()
{
	return !Trait::IsSame<NoMember, decltype(CallSfinae::tryLock(Trait::eDeclval<T>()))>::value;
}

}  // namespace Refl
}  // namespace Rr

#endif // RR_REFL_INTROSPECTION_HPP

