//
// Wrap.hpp
//
// Created on: 2022-01-22
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(RR_CONT_WRAPPER_HPP)
#define RR_CONT_WRAPPER_HPP

#include <Rr/Trait/Declval.hpp>
#include <Rr/Refl/CallFamily.hpp>

namespace Rr {
namespace Cont {
namespace WrapImpl {

enum class ContainerType {
	Index,
	Push,
	Incompatible,
};

template <class Tv, template <class...> class Tc>
struct GetContainerType;

template <ContainerType, class Tv, template <class ...> class Tc>
struct Wrap;

template <class Tv, template <class ...> class Tc>
struct Wrap<ContainerType::Incompatible, Tv, Tc> {
};

template <class Tv, template <class ...> class Tc>
class Wrap<ContainerType::Index, Tv, Tc> : Tc<Tv> {
	unsigned pos;
	const unsigned maxSize;

public:
	using Iterator = decltype(&Rr::Trait::declval<Tc<Tv>>().at(0));

	using Tc<Tv>::Tc;
	using Tc<Tv>::at;

	Wrap();
	unsigned size() const;
	Iterator begin();
	Iterator end();
	void push(const Tv &);

	template <class ...Ts>
	void emplace(Ts &&...);
};

template <class Tv, template <class ...> class Tc>
struct Wrap<ContainerType::Push, Tv, Tc> {
};

}  // namespace WrapImpl

template <class Tvalue, template <class ...> class Tcontainer>
using Wrap = typename WrapImpl::Wrap<WrapImpl::GetContainerType<Tvalue, Tcontainer>::value, Tvalue, Tcontainer>;

}  // namespace Cont
}  // namespace Rr

#include "Wrap.impl"

#endif // RR_CONT_WRAPPER_HPP

