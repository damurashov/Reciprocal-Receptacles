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
#include <Rr/Trait/Apply.hpp>

namespace Rr {
namespace Trait {
namespace DecayImpl {

using namespace Rr::Trait;

template <class T>
struct Decay :
	Apply<T,
		RemoveReference,
		ApplyConditional<IsArray<T>::value, MakeApplyChain<RemoveExtent, AddPointer>::template Ttype>::template Ttype,
		ApplyConditional<IsFunction<T>::value, AddPointer, RemoveCv>::template Ttype
	>
{
};

}  // DecayImpl

using DecayImpl::Decay;

}  // Trait
}  // Rr

#endif  // RR_TRAIT_DECAY_HPP_
