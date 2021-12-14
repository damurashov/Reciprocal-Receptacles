//
// CallableTable.hpp
//
// Created: 2021-12-03
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_CALLBACK_HPP)
#define RR_UTIL_CALLBACK_HPP

#include <Rr/Util/SyncedCallable.hpp>
#include <Rr/Util/CallableWrapper.hpp>

namespace Rr {
namespace Util {

template <class Tsignature, template <class ...> class Tcontainer>
using CallableTable = Tcontainer<Rr::Util::Callable<Tsignature>>;

template <class Tsignature, template<class...> class Tcontainer, class Tsync>
using SyncedCallableTable = Tcontainer<typename Rr::Util::SyncedCallableType<Tsignature, Tsync>::Type>;

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
