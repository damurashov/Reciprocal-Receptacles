//
// PrimitiveHolder.hpp
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
class PrimitiveHolder {
	TholderType syncPrimitive;

protected:
	Tprimitive &getSyncPrimitive()
	{
		return *syncPrimitive;
	}

public:
	PrimitiveHolder(Tprimitive &aPrimitive): syncPrimitive{&aPrimitive}
	{
	}
};

template <class TprimitiveType>
struct HeapPrimitiveHolder : PrimitiveHolder<TprimitiveType>{
	HeapPrimitiveHolder(): PrimitiveHolder<TprimitiveType>{*(new TprimitiveType())}
	{
	}
protected:
	using PrimitiveHolder<TprimitiveType>::getSyncPrimitive;
};

template <class Tsync>
struct StaticPrimitiveHolder : PrimitiveHolder<typename Rr::Trait::RemoveReference<decltype(Tsync::syncPrimitive)>::Type> {
	StaticPrimitiveHolder(): PrimitiveHolder<typename Rr::Trait::RemoveReference<decltype(Tsync::syncPrimitive)>::Type>{Tsync::syncPrimitive}
	{
	}

protected:
	using PrimitiveHolder<typename Rr::Trait::RemoveReference<decltype(Tsync::syncPrimitive)>::Type>::getSyncPrimitive;
};

}  // namespace Sync
}  // namespace Rr

#endif // RR_UTIL_SYNC_HPP_

