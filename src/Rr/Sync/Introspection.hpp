//
// Introspection.hpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

// Constexpr sync-related introspection helpers

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

}  // namespace Sync
}  // namespace Rr
