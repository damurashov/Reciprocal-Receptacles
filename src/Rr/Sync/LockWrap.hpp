//
// LockWrap.hpp
//
// Created: 2021-12-13
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_UTIL_LOCKBORROW_HPP)
#define RR_TRAIT_UTIL_LOCKBORROW_HPP

namespace Rr {
namespace Sync {

///
/// @brief RAII lock. Similar to std::lock_guard and similar types, but tightly
/// associated with an instance
///
/// @tparam Tlock     Lock "guard" type
/// @tparam Tinstance Instance type
///
template <class Tlock, class Tinstance>
class LockWrap final {
	Tlock *lock;
	Tinstance *instance;

public:
	template <class TsyncPrimitive>
	LockWrap(TsyncPrimitive &aSyncPrimitive, Tinstance &aInstance): lock{new Tlock{aSyncPrimitive}}, instance{&aInstance}
	{
	}

	LockWrap(const LockWrap &) = delete;
	LockWrap &operator=(const LockWrap &) = delete;
	LockWrap(LockWrap &&aRhs);
	LockWrap &operator=(LockWrap &&aRhs);

	~LockWrap();

	Tinstance &getInstance()
	{
		return *instance;
	}

	operator Tinstance&()
	{
		return *instance;
	}
};

}  // namespace Sync
}  // namespace Rr

#include "LockWrap.impl"

#endif // RR_TRAIT_UTIL_LOCKBORROW_HPP
