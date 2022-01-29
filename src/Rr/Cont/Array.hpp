//
// Array.hpp
//
// Created on: 2022-01-15
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_CONT_ARRAY_HPP)
#define RR_CONT_ARRAY_HPP

#include <Rr/Util/DefaultConfig.hpp>

namespace Rr {
namespace Cont {

template <class T, unsigned N>
struct Array {
	T stored[N];

	using Value = T;
	using Reference = T &;
	using ConstReference = const Reference;

	using Iterator = Value *;
	using ConstIterator = const Iterator;
	using ReverseIterator = Iterator;
	using ConstReverseIterator = ConstIterator;

	Reference &at(unsigned aPos);
	ConstReference &at(unsigned aPos) const;
	constexpr unsigned maxSize() const;

	Iterator begin();
	Iterator end();
	ConstIterator cbegin() const;
	ConstIterator cend() const;
};

}  // namespace Cont
}  // namespace Rr

#include "Array.impl"

#endif // RR_CONT_ARRAY_HPP
