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
#include <Rr/Trait/StoreType.hpp>

namespace Rr {
namespace Trait {
namespace AddCvprefImpl {

template <class T>
auto sfinaeAddRvalueReference(int) -> typename Rr::Trait::StoreType<T &&>::Type;

template <class T>
auto sfinaeAddRvalueReference(...) -> typename Rr::Trait::StoreType<T>::Type;

template <class T>
struct AddRvalueReference : decltype(sfinaeAddRvalueReference<T>(0)) {
};

template <class T>
auto sfinaeAddLvalueReference(int) -> typename Rr::Trait::StoreType<T&>::Type;

template <class T>
auto sfinaeAddLvalueReference(...) -> typename Rr::Trait::StoreType<T>::Type;

template <class T>
struct AddLvalueReference : decltype(sfinaeAddLvalueReference<T>(0)) {
};

template <class T>
auto sfinaeAddPointer(int) -> typename Rr::Trait::StoreType<typename Rr::Trait::RemoveReference<T>::Type *>::Type;

template <class T>
auto sfinaeAddPointer(...) -> typename Rr::Trait::StoreType<typename Rr::Trait::RemoveReference<T>::Type *>::Type;

template <class T>
struct AddPointer : decltype(sfinaeAddPointer<T>(0)) {
};

}  // AddCvprefImpl

using AddCvprefImpl::AddLvalueReference;
using AddCvprefImpl::AddRvalueReference;
using AddCvprefImpl::AddPointer;

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
