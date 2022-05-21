//
// IsPointer.hpp
//
// Created on:  May 21, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_TRAIT_ISPOINTER_HPP_)
#define RR_TRAIT_ISPOINTER_HPP_

#include <Rr/Trait/IntegralConstant.hpp>

namespace Rr {
namespace Trait {
namespace IsPointerImpl {

template <class T> struct IsPointer : FalseType { };
template <> struct IsPointer<T *> : TrueType {};
template <> struct IsPointer<T *const> : TrueType {};
template <> struct IsPointer<T *const volatile> : TrueType {};
template <> struct IsPointer<T *volatile> : TrueType {};

}  // IsPointerImpl

using IsPointerImpl::IsPointer;

}  // Trait
}  // Rr

#endif  // RR_TRAIT_ISPOINTER_HPP_
