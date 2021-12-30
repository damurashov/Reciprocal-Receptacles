//
// Apply.hpp
//
// Created on: 2021-12-26
//     Author: dmtr <DOT> murashov <AT> <GMAIL COM>
//

#if !defined(RR_TRAIT_APPLY_HPP)
#define RR_TRAIT_APPLY_HPP

#include <Rr/Trait/StoreType.hpp>
#include <Rr/Trait/Conditional.hpp>

namespace Rr {
namespace Trait {

template <bool F, template <class...> class Tt>
struct ApplyConditional {
    template<class T>
    struct Ttype : Tt<T> {
    };
};

template <template <class...> class Tt>
struct ApplyConditional<false, Tt> {
    template<class T>
    struct Ttype : StoreType<T> {
    };
};

namespace ApplyImpl {

template <class T, template <class...> class Tt, template <class...> class ...Tts>
struct Apply : StoreType<typename Apply<typename Tt<T>::Type, Tts...>::Type> {
};

template <class T>
struct Apply<T, StoreType> : StoreType<T> {
};

}  // ApplyImpl

template <class T, template <class...> class ...Tts>
struct Apply : ApplyImpl::Apply<T, Tts..., StoreType> {
};

}  // Trait
}  // Rr

#endif // RR_TRAIT_APPLY_HPP
