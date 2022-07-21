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

/// \brief Shared access is a thread-safe wrapper over an instance of `T`. Shared access is stored in a static storage,
/// and thus it is valid to refer to it even when the stored type itself has ceased to exist.
///
/// \details Both during the stored instance's lifetime control (constructors and destructors), and during access to
/// the instance, the same mutex (`primitive`) is used. Therefore, the instance can be safely destructed.
///
/// WORKFLOW. A client gets ownership of a `primitive`, checks whether `stored` is nullptr, and if that's not the case,
/// references the stored instance. The instance during its own creation or destruction, in turn, acquires ownership
/// over `primitive` and modifies `stored`.
///
template <class T, class TsyncTrait>
struct SharedAccess {
	using Type = T;
	using SyncTrait = TsyncTrait;
	using PrimitiveType = typename Rr::Sync::Policy::SharedAccess<TsyncTrait>::PrimitiveType;

	Type *stored = nullptr;
	PrimitiveType primitive;

	void lock()
	{
		Rr::Sync::Policy::SharedAccess<TsyncTrait>::PrimitiveOps::lock(primitive);
	}

	void unlock()
	{
		Rr::Sync::Policy::SharedAccess<TsyncTrait>::PrimitiveOps::unlock(primitive);
	}

	SharedAccess(Type *a): stored{a}, primitive{}
	{
	}

	SharedAccess(): stored{nullptr}, primitive{}
	{
	}
};

/// \brief An iterator can be turned into `LockWrap`, so the instance may be safely accessed at some point later.
///
/// \details This whole "delayed access" scheme is useful, when the instance is unavailable at the moment, but there is
/// a reason to believe that this will be changed.
///
template <class T, class TsyncTrait>
struct LockWrap final {
public:
	struct Wrapped final {
		T *tryGet() {
			return sharedAccess.stored;
		}
		Wrapped(SharedAccess<T, TsyncTrait> &aSharedAccess) : sharedAccess{aSharedAccess}
		{
			sharedAccess.lock();
		}
		~Wrapped()
		{
			sharedAccess.unlock();
		}
	private:
		SharedAccess<T, TsyncTrait> &sharedAccess;
	};

	LockWrap(SharedAccess<T, TsyncTrait> &aSharedAccess) : sharedAccess{aSharedAccess}
	{
	}
	Wrapped wrap()
	{
		return Wrapped{sharedAccess};
	}
private:
	SharedAccess<T, TsyncTrait> &sharedAccess;
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
	using S = typename Rr::Trait::StripTp<decltype(*Rr::Trait::eDeclval<Titerator>())>;
	using Policy = Rr::Sync::Policy::SharedAccess<typename S::SyncTrait>;

	Titerator it;
	Titerator itEnd;
	bool locked;

	void lock();
	void unlock();
	void skip();  // Advance to a next non-nullptr element, until itEnd is met
public:
	using WrappedIt = Titerator;

	SharedAccessIt(Titerator aItBegin, Titerator aItEnd);
	SharedAccessIt(const SharedAccessIt &);
	SharedAccessIt(SharedAccessIt &&);
	SharedAccessIt &operator=(const SharedAccessIt &);
	SharedAccessIt &operator=(SharedAccessIt &&);
	~SharedAccessIt();

	bool operator==(const SharedAccessIt &);
	bool operator!=(const SharedAccessIt &);
	bool operator==(const Titerator &);
	bool operator!=(const Titerator &);
	typename S::Type &operator*();
	typename S::Type *operator->();
	SharedAccessIt &operator++();
};

}  // namespace Sync
}  // namespace Rr

#include "SharedAccess.impl"

#endif // RR_SYNC_SHAREDACCESS_HPP
