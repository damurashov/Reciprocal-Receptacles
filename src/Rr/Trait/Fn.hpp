//
// Fn.hpp
//
// Created: 2021-12-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_TRAIT_FN_HPP)
#define RR_TRAIT_FN_HPP

namespace Rr {
namespace Trait {

template <class ...T>
struct Fn;

template<class T, class Tret, class ...Targs>
struct Fn<T, Tret(Targs...)> {
	using CallbackType = Tret(T::*)(Targs...);
	using ReturnType = Tret;
	using InstanceType = T;

	struct Type {
		using Callback = Tret(T::*)(Targs...);
		using Return = Tret;
		using Instance = T;
	};
};

template<class Tret, class ...Targs>
struct Fn<Tret(Targs...)> {
	using CallbackType = Tret(*)(Targs...);
	using ReturnType = Tret;
	using InstanceType = void;

	struct Type {
		using Callback = Tret(*)(Targs...);
		using Return = Tret;
		using Instance = void;
	};
};

}  // namespace Trait
}  // namespace Rr

#endif // RR_TRAIT_FN_HPP
