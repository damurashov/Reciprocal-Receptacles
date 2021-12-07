//
// Conditional.hpp
//
// Created: 2021-12-07
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

namespace Rr {
namespace Trait {

template <bool, class A, class B>
struct Conditional {
	using type = A;
	using Type = A;
};

template <class A, class B>
struct Conditional<false, A, B> {
	using type = A;
	using Type = A;
};

}  // namespace Trait
}  // namespace Rr
