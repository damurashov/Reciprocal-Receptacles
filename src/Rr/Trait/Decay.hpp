//
// Decay.hpp
//
// Created on:  May 22, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_TRAIT_DECAY_HPP_)
#define RR_TRAIT_DECAY_HPP_

#include <Rr/Trait/RemoveReference.hpp>
#include <Rr/Trait/IsFunction.hpp>
#include <Rr/Trait/IsPointer.hpp>
#include <Rr/Trait/IsArray.hpp>

namespace Rr {
namespace Trait {
namespace DecayImpl {

using namespace Rr::Trait;

template <class Tnoref>
struct DecayNoref :
	StoreType<
		ConditionalTp<
			/* if */IsArray<Tnoref>::value,
			/* then */RemoveExtentTp<Tnoref> *,
			ConditionalTp<
				/* elseif */IsFunction<Tnoref>::value,
				/* then */typename AddPointer<Tnoref>::Type,
				/* else */typename RemoveCv<Tnoref>::Type
			>
		>
	>
{
};

template <class T>
struct Decay : DecayNoref<typename RemoveReference<T>::Type> {
};

}  // DecayImpl

using DecayImpl::Decay;

}  // Trait
}  // Rr

#endif  // RR_TRAIT_DECAY_HPP_
