//
// AddCvpref.hpp
//
// Created on: 2021-12-26
//     Author: dmtr <DOT> murashov <AT> <GMAIL COM>
//

#if !defined(RR_TRAIT_ADDCVPREF_HPP)
#define RR_TRAIT_ADDCVPREF_HPP

#include <Rr/Trait/RemoveReference.hpp>
#include <Rr/Trait/Apply.hpp>

namespace Rr {
namespace Trait {

template <class T>
struct AddPointer : StoreType<typename RemovePointer<T>::Type *> {
};

template <class T>
struct AddLvalueReference : StoreType<typename RemoveReference<T>::Type &> {
};

template <>
struct AddLvalueReference<void> : StoreType<void> {
};

template <class T>
struct AddRvalueReference : StoreType<typename RemoveReference<T>::Type &&> {
};

template <>
struct AddRvalueReference<void> : StoreType<void> {
};

template <class T>
struct AddVolatile : StoreType <volatile T> {
};

template <class T>
struct AddConst : StoreType <const T> {
};

template <class T, bool Fc, bool Fv, bool Fp, bool Flref, bool Frref>
struct AddCvpref : Apply<T,
		ApplyConditional<Fp, AddPointer>::template Ttype,
		ApplyConditional<Fc, AddConst>::template Ttype,
		ApplyConditional<Fv, AddVolatile>::template Ttype,
		ApplyConditional<Flref, AddLvalueReference>::template Ttype,
		ApplyConditional<Frref, AddRvalueReference>::template Ttype>
{
};

template <class T, bool ...Tfs>
using AddCvpreft = typename AddCvpref<T, Tfs...>::Type;

}  // Trait
}  // Rr

#endif // RR_TRAIT_ADDCVPREF_HPP
