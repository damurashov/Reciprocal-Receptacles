//
// SharedAccess.hpp
//
// Created on: 2022-01-08
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_SHAREDACCESS_HPP)
#define RR_SYNC_SHAREDACCESS_HPP

#include <Rr/Sync/Policy/SharedAccess.hpp>
#include <Rr/Trait/Declval.hpp>

namespace Rr {
namespace Sync {

template <class T, class TsyncTrait>
struct SharedAccess {
	using Type = T;
	using SyncTrait = TsyncTrait;
	using PrimitiveType = typename Rr::Sync::Policy::SharedAccess<TsyncTrait>::PrimitiveType;

	Type *stored = nullptr;
	PrimitiveType primitive;

	SharedAccess(Type *a): stored{a}, primitive{}
	{
	}

	SharedAccess(): stored{nullptr}, primitive{}
	{
	}
};

///
/// @brief Iterates over elements, enables one to skip positions which contain nullptr-ed `SharedAccess` instances.
/// Works as a wrapper over an iterator type pertaining to whatever container stores `SharedAccess<...>` instances. The
/// container is expected to maintain its iterators' validity throughout the SharedAccessIt's lifespan, because no locks
/// related to the container itself are used here.
///
/// @tparam Titerator
///
template <class Titerator>
class SharedAccessIt {
	using S = typename Rr::Trait::StripTp<decltype(*Rr::Trait::declval<Titerator>())>;
	using Policy = Rr::Sync::Policy::SharedAccess<typename S::SyncTrait>;

	Titerator it;
	Titerator itEnd;
	bool locked;

	void lock();
	void unlock();
	void skip();  // Advance to a next non-nullptr element, until itEnd is met
public:
	SharedAccessIt(Titerator aItBegin, Titerator aItEnd);
	SharedAccessIt(const SharedAccessIt &);
	SharedAccessIt(SharedAccessIt &&);
	SharedAccessIt &operator=(const SharedAccessIt &);
	SharedAccessIt &operator=(SharedAccessIt &&);
	~SharedAccessIt();

	bool operator==(const SharedAccessIt &);
	bool operator!=(const SharedAccessIt &);

	typename S::Type &operator*();
	typename S::Type *operator->();
	SharedAccessIt &operator++();
};

}  // namespace Sync
}  // namespace Rr

#include "SharedAccess.impl"

#endif // RR_SYNC_SHAREDACCESS_HPP
