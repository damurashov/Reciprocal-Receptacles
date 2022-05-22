//
// Declval.hpp
//
// Created on: 2022-01-16
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_DECLVAL_HPP)
#define RR_TRAIT_DECLVAL_HPP

#include <Rr/Trait/AddCvpref.hpp>
#include <Rr/Trait/IntegralConstant.hpp>

namespace Rr {
namespace Trait {
namespace DeclvalImpl {

static constexpr int p1 = 0;
static constexpr const void *p2 = &p1;

template <class T>
struct AlwaysFalse : FalseType {
};

template <class T>
typename AddRvalueReference<T>::Type declval() noexcept
{
	static_assert(AlwaysFalse<T>::value, "Use of declval in an evaluated context");
}

}  // namespace DeclvalImpl

/// \brief Non-idiomatic implementation of STL-like declval that can be used in an evaluated context
///
template <class T>
constexpr T &eDeclval()
{
	return *const_cast<T *>((const T *) DeclvalImpl::p2);
}

using DeclvalImpl::declval;

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_DECLVAL_HPP
