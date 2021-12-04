//
// Max.hpp
//
// Created on: 2021-12-04
//     Author: dmtr <DOT> murashov <AT> <GMAIL COM>
//

#if !defined(RR_TRAIT_MAX_HPP)
#define RR_TRAIT_MAX_HPP

namespace Rr {
namespace Trait {

template <unsigned N1, unsigned N2, bool Ngt = (N1 > N2)>
struct Max2;

template <unsigned N1, unsigned N2>
struct Max2<N1, N2, true> {
	static constexpr auto value = N1;
};

template <unsigned N1, unsigned N2>
struct Max2<N1, N2, false> {
	static constexpr auto value = N2;
};

template <unsigned N, unsigned ...Nsizeofs>
struct MaxN {
	static constexpr auto value = Max2<N, MaxN<(Nsizeofs)...>::value>::value;
};

template <unsigned N>
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
