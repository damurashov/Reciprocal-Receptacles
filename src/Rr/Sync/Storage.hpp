//
// Storage.hpp
//
// Created on: 2022-01-08
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_SYNC_STORAGE_HPP)
#define RR_SYNC_STORAGE_HPP

#include <Rr/Sync/SharedAccess.hpp>
#include <Rr/Sync/Policy/Storage.hpp>
#include <Rr/Cont/Wrap.hpp>

namespace Rr {
namespace Sync {
namespace StorageImpl {

template <class T, class TsyncTrait, template<class...> class Tcontainer>
class Storage : public Rr::Cont::Wrap<SharedAccess<T, TsyncTrait>, Tcontainer> {
private:
	using Policy = typename Policy::Storage<TsyncTrait>;
	using Primitive = typename Policy::PrimitiveType;
	using Container = Rr::Cont::Wrap<SharedAccess<T, TsyncTrait>, Tcontainer>;
	using ContainerIterator = typename Container::Iterator;

	Primitive primitive;

	using Container::begin;
	using Container::end;
	using Container::push;
	using Container::emplace;
	using Container::Container;

public:
	using Value = SharedAccess<T, TsyncTrait>;
	using Iterator = SharedAccessIt<ContainerIterator>;

	Value &reg(T &);

	template <class ...Ts>
	Storage(Ts &&...aArgs) : Container{Trait::forward<Ts>(aArgs)...}, primitive{}
	{
	}

	struct Iterators {
		Iterator itBegin;
		Iterator itEnd;

		Iterator begin()
		{
			return itBegin;
		}

		Iterator end()
		{
			return itEnd;
		}
	};

	Iterators getIterators();
};

}  // namespace StorageImpl

template <class T, class TsyncTrait, template<class...> class Tcontainer>
using Storage = StorageImpl::Storage<T, TsyncTrait, Tcontainer>;

}  // namespace Sync
}  // namespace Rr

#include "Storage.impl"

#endif // RR_SYNC_STORAGE_HPP
