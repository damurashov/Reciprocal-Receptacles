//
// Introspection.hpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

// Constexpr sync-related introspection helpers

#if !defined(RR_SYNC_INTROSPECTION_HPP)
#define RR_SYNC_INTROSPECTION_HPP

namespace Rr {
namespace Sync {
namespace IntrospectionImpl {

struct IsMember {
	char a[1];
};

struct NoMember {
	char a[2];
};

static_assert(sizeof(IsMember) != sizeof(NoMember));

struct SfinaeFallback {
	template <class T>
	static NoMember type(...);
};

struct Mutex : SfinaeFallback {
	template <class T>
	static IsMember type(typename T::Mutex *);

	using SfinaeFallback::type;
};

struct Lock : SfinaeFallback {
	using SfinaeFallback::type;

	template <class T>
	static IsMember type(typename T::Lock *);
};

struct CallPolicy : SfinaeFallback {
	using SfinaeFallback::type;

	template <class T>
	static IsMember type(decltype(T::kCallPolicy) *);
};

struct Policy : SfinaeFallback {
	using SfinaeFallback::type;

	template <class T>
	static IsMember type(decltype(T::kPolicy) *);
};

template <class Treference, class T>
constexpr bool defines()
{
	return sizeof(decltype(Treference::template type<T>(nullptr))) == sizeof(IsMember);
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

}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_INTROSPECTION_HPP

