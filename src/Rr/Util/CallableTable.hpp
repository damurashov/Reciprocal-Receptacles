//
// CallableTable.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLBACK_HPP)
#define RR_UTIL_CALLBACK_HPP

#include <Rr/Util/SyncedCallable.hpp>
#include <Rr/Util/LockWrap.hpp>
#include <Rr/Trait/LockType.hpp>

namespace Rr {
namespace Util {

template <class Tsignature, template <class ...> class Tcontainer>
using CallableTable = Tcontainer<Rr::Util::Callable<Tsignature>>;

template <class Tsignature, template<class...> class Tcontainer, class Tsync>
using SyncedCallableTable = Tcontainer<typename Rr::Util::SyncedCallableType<Tsignature, Tsync>::Type>;

template <class Tsignature, class Tsync>
class SyncedCallableWrapper {
	bool enabled;
	typename SyncedCallableType<Tsignature, Tsync>::Type &callable;

public:
	bool setEnabled(bool);

	///
	/// @brief Safely locks the wrapped instance using whatever lock type is
	/// defined by its policy, and returns an instance of LockWrap. For lock
	/// policy inference, \see Trait/LockType.hpp
	///
	typename Rr::Util::LockWrap<typename Rr::Trait::LockType<Tsignature, Tsync>::Type,
		typename SyncedCallableType<Tsignature, Tsync>::Type> asLockWrap()
	{
		return {callable.getSyncPrimitive, callable};
	}

	SyncedCallableWrapper(bool aEnabled, decltype(callable) aCallable): enabled(aEnabled), callable{aCallable}
	{
	}

	SyncedCallableWrapper(decltype(callable) aCallable): SyncedCallableWrapper(true, aCallable)
	{
	}
};

template <class Tsignature, class Ttopic, template <class ...> class Tcontainer, class Tsync>
class SyncedCallableWrapperStaticTable {

	///
	/// @brief Growing container of callables
	///
	static Tcontainer<SyncedCallableWrapper<Tsignature, Tsync>> table;

	static typename Tsync::Type syncPrimitive;
public:
	///
	/// @brief Lock wrap for iterating over table items()
	///
	static typename Rr::Util::LockWrap<typename Rr::Trait::SharedLockType<Tsync>::Type,
		decltype(table)> asSharedLockWrap()
	{
		return {syncPrimitive, table};
	}

	///
	/// @brief Lock wrap for amending the table
	///
	static typename Rr::Util::LockWrap<typename Rr::Trait::SharedLockType<Tsync>::Type,
		decltype(table)> asUniqueLockWrap()
	{
		// return static_cast<decltype(asUniqueLockWrap()) &&>(decltype(asUniqueLockWrap()){syncPrimitive, table});
		return {syncPrimitive, table};
	}
};

}  // namespace Util
}  // namespace Rr

#include "CallableTable.impl"

#endif // RR_UTIL_CALLBACK_HPP
