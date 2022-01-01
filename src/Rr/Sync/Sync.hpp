//
// Sync.hpp
//
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_UTIL_SYNC_HPP_)
#define RR_UTIL_SYNC_HPP_

#include <Rr/Sync/SyncMock.hpp>
#include <Rr/Trait/RemoveReference.hpp>

namespace Rr {
namespace Sync {

template <class Tprimitive, class TholderType = Tprimitive *>
class SyncPrimitiveHolder {
	TholderType syncPrimitive;

protected:
	Tprimitive &getSyncPrimitive()
	{
		return *syncPrimitive;
	}

public:
	SyncPrimitiveHolder(Tprimitive &aPrimitive): syncPrimitive{&aPrimitive}
	{
	}
};

template <class TprimitiveType>
struct HeapSyncPrimitiveHolder : SyncPrimitiveHolder<TprimitiveType>{
	HeapSyncPrimitiveHolder(): SyncPrimitiveHolder<TprimitiveType>{*(new TprimitiveType())}
	{
	}
protected:
	using SyncPrimitiveHolder<TprimitiveType>::getSyncPrimitive;
};

template <class Tsync>
struct StaticSyncPrimitiveHolder : SyncPrimitiveHolder<typename Rr::Trait::RemoveReference<decltype(Tsync::syncPrimitive)>::Type> {
	StaticSyncPrimitiveHolder(): SyncPrimitiveHolder<typename Rr::Trait::RemoveReference<decltype(Tsync::syncPrimitive)>::Type>{Tsync::syncPrimitive}
	{
	}

protected:
	using SyncPrimitiveHolder<typename Rr::Trait::RemoveReference<decltype(Tsync::syncPrimitive)>::Type>::getSyncPrimitive;
};

}  // namespace Sync
}  // namespace Rr

#endif // RR_UTIL_SYNC_HPP_

