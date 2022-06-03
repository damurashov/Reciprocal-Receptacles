//
// Max.hpp
//
// Created on: 2021-12-04
//     Author: dmtr <DOT> murashov <AT> <GMAIL COM>
//

#if !defined(RR_TRAIT_MAX_HPP)
#define RR_TRAIT_MAX_HPP

#include <Rr/Util/DefaultConfig.hpp>

namespace Rr {
namespace Trait {

template <Rr::usize N1, Rr::usize N2, bool Ngt = (N1 > N2)>
struct Max2;

template <Rr::usize N1, Rr::usize N2>
struct Max2<N1, N2, true> {
	static constexpr auto value = N1;
};

template <Rr::usize N1, Rr::usize N2>
struct Max2<N1, N2, false> {
	static constexpr auto value = N2;
};

template <Rr::usize N, Rr::usize ...Nsizeofs>
struct MaxN {
	static constexpr auto value = Max2<N, MaxN<(Nsizeofs)...>::value>::value;
};

template <Rr::usize N>
struct MaxN<N> {
	static constexpr auto value = N;
};

template <class T, class ...Targs>
struct MaxSizeof {
	static constexpr auto value = MaxN<sizeof(T), sizeof(Targs)...>::value;
};

}  // Trait
}  // Rr

#endif // RR_TRAIT_MAX_HPP
