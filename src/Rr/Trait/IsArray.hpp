//
// IsArray.hpp
//
// Created on:  May 22, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_TRAIT_ISARRAY_HPP_)
#define RR_TRAIT_ISARRAY_HPP_

#include <Rr/Trait/IntegralConstant.hpp>

namespace Rr {
namespace Trait {
namespace IsArrayImpl {

template <class T> struct IsArray : FalseType {};
template <class T> struct IsArray<T[]> : TrueType {};
template <class T, decltype(sizeof(int *)) N> struct IsArray<T[N]> : TrueType {};

}  // IsArrayImpl

using IsArrayImpl::IsArray;

}  // Trait
}  // Rr

#endif  // RR_TRAIT_ISARRAY_HPP_
