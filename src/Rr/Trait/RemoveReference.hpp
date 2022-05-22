//
// RemoveReference.hpp
//
// Created: 2021-12-14
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_REMOVEREFERENCE_HPP)
#define RR_TRAIT_REMOVEREFERENCE_HPP

#include <Rr/Trait/Apply.hpp>

namespace Rr {
namespace Trait {

template <class T>
struct RemoveReference : StoreType<T> {
};

template <class T>
struct RemoveReference<T&> : StoreType<T> {
};

template <class T>
struct RemoveReference<T&&> : StoreType<T> {
};

template <class T>
struct RemovePointer : StoreType<T> {
};

template <class T>
struct RemovePointer<T*> : StoreType<T> {
};

template <class T>
struct RemovePointer<T* const> : StoreType<T> {
};

template <class T>
struct RemovePointer<T* volatile> : StoreType<T> {
};

template <class T>
struct RemovePointer<T* const volatile> : StoreType<T> {
};

template <class T>
struct RemoveConst : StoreType<T> {
};

template <class T>
struct RemoveConst<const T> : StoreType<T> {
};

template <class T>
struct RemoveVolatile : StoreType<T> {
};

template <class T>
struct RemoveVolatile<volatile T> : StoreType<T> {
};

template <class T>
struct RemoveCv : StoreType<typename RemoveConst<typename RemoveVolatile<T>::Type>::Type> {
};

template <class T>
struct RemoveExtent : StoreType<T> {
};

template <class T>
struct RemoveExtent<T[]> : StoreType<T> {
};

template <class T, decltype(sizeof(int *)) N>
struct RemoveExtent<T[N]> : StoreType<T> {
};

template <class T>
using Strip = Apply<T, RemoveCv, RemoveReference, RemovePointer>;

template <class T>
using Stript = typename Strip<T>::Type;

template <class T>
using StripTp = typename Strip<T>::Type;

template <class T, bool Fc, bool Fv, bool Fp, bool Fref>
struct RemoveCvpref :
	Apply<T,
		ApplyConditional<Fc, RemoveConst>::template Ttype,
		ApplyConditional<Fv, RemoveVolatile>::template Ttype,
		ApplyConditional<Fp, RemovePointer>::template Ttype,
		ApplyConditional<Fref, RemoveReference>::template Ttype>
{
};

template <class T, bool ...Fs>
using RemoveCvpreft = typename RemoveCvpref<T, Fs...>::Type;

template <class T, bool ...Fs>
using RemoveCvprefTp = typename RemoveCvpref<T, Fs...>::Type;

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_REMOVEREFERENCE_HPP
