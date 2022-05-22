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
auto sfinaeAddRvalueReference(int) -> Rr::Trait::StoreType<T &&>;

template <class T>
auto sfinaeAddRvalueReference(...) -> Rr::Trait::StoreType<T>;

template <class T>
struct AddRvalueReference : decltype(sfinaeAddRvalueReference<T>(0)) {
};

template <class T>
auto sfinaeAddLvalueReference(int) -> Rr::Trait::StoreType<T&>;

template <class T>
auto sfinaeAddLvalueReference(...) -> Rr::Trait::StoreType<T>;

template <class T>
struct AddLvalueReference : decltype(sfinaeAddLvalueReference<T>(0)) {
};

template <class T>
auto sfinaeAddPointer(int) -> Rr::Trait::StoreType<typename Rr::Trait::RemoveReference<T>::Type *>;

template <class T>
auto sfinaeAddPointer(...) -> Rr::Trait::StoreType<T>;

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
