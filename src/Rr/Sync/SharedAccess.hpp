//
// SharedAccess.hpp
//
// Created on: 2022-01-08
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_SHAREDACCESS_HPP)
#define RR_SYNC_SHAREDACCESS_HPP

#include <Rr/Sync/Policy/SharedAccess.hpp>

namespace Rr {
namespace Sync {

template <class T, class TsyncTrait>
struct SharedAccess {
	using Type = T;
	using SyncTrait = TsyncTrait;
	using Primitive = typename Rr::Sync::Policy::SharedAccess<TsyncTrait>::Primitive;

	Type *stored = nullptr;
	Primitive primitive;
};

///
/// @brief Iterates over elements, enables one to skip positions which contain nullptr-ed `SharedAccess` instances. Works as
/// a wrapper over an iterator type pertaining to whatever container stores `SharedAccess<...>` instances. The container is
/// expected to maintain its iterators' validity throughout the SharedAccessIt's lifespan, because no locks pertaining to the
/// container itself are used here.
///
/// @tparam Titerator
///
template <class Titerator>
class SharedAccessIt {
	using S = typename Rr::Trait::RemoveCvpreft<decltype(*Titerator())>;
	using Policy = typename Rr::Sync::Policy::SharedAccess<S::SyncTrait>;

	Titerator it;
	Titerator itEnd;
	bool locked;

	void lock();
	void unlock();
	void skip();  // Advance to a next non-nullptr element, until itEnd is met
public:
	SharedAccessIt(Titerator aItBegin, Titerator aItEnd);
	SharedAccessIt(const SharedAccessIt &) = default;
	SharedAccessIt(SharedAccessIt &&) = default;
	SharedAccessIt &operator=(const SharedAccessIt &) = default;
	SharedAccessIt &operator=(SharedAccessIt &&) = default;
	~SharedAccessIt();

	bool operator==(const SharedAccessIt &);
	bool operator!=(const SharedAccessIt &);

	S::Type &operator*();
	S::Type *operator->();
	SharedAccessIt &operator++();
};

}  // namespace Sync
}  // namespace Rr

#endif // RR_SYNC_SHAREDACCESS_HPP
