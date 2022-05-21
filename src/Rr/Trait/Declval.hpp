//
// Declval.hpp
//
// Created on: 2022-01-16
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_DECLVAL_HPP)
#define RR_TRAIT_DECLVAL_HPP

namespace Rr {
namespace Trait {
namespace DeclvalImpl {

static constexpr int p1 = 0;
static constexpr const void *p2 = &p1;

}  // namespace DeclvalImpl

template <class T>
constexpr T &eDeclval()
{
	return *const_cast<T *>((const T *) DeclvalImpl::p2);
}

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_DECLVAL_HPP
