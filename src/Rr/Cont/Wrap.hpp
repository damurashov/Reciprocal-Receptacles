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
#include <Rr/Trait/EnableIf.hpp>

namespace Rr {
namespace Cont {
namespace WrapImpl {

enum class ContainerType {
	Index,
	Push,
	Incompatible,
};

// Sfinaes

struct CallIndex {
	template <class T2>
	static auto call(T2 &a) -> decltype(a.at(0))
	{
		a.at(0);
	}
};

struct CallMaxSizeUnderscore {
	template <class T2>
	static auto call(T2 &a) -> decltype(a.max_size())
	{
		return a.max_size();
	}
};

struct CallMaxSizeCamelcase {
	template <class T2>
	static auto call(T2 &a) -> decltype(a.maxSize())
	{
		return a.maxSize();
	}
};

struct CallCapacity {
	template <class T2>
	static auto call (T2 &a) -> decltype(a.capacity())
	{
		return a.capacity();
	}
};

struct CallPush {
	template <class Ti, class Tv>
	static auto call(Ti &aI, Tv &aV) -> decltype(aI.push(aV))
	{
		return aI.push(aV);
	}
};

struct CallPushBackUnderscore {
	template <class Ti, class Tv>
	static auto call(Ti &aI, Tv &aV) -> decltype(aI.push_back(aV))
	{
		return aI.push_back(aV);
	}
};

struct CallPushBackCamelcase {
	template <class Ti, class Tv>
	static auto call(Ti &aI, Tv &aV) -> decltype(aI.pushBack(aV))
	{
		aI.pushBack(aV);
	}
};

struct CallEmplace {
	template <class Ti, class ...Ts>
	static auto call(Ti &&aT, Ts &&...aArgs) -> decltype(aT.emplace(Rr::Trait::forward<Ts>(aArgs)...))
	{
		return aT.emplace(Rr::Trait::forward<Ts>(aArgs)...);
	}
};

struct CallEmplaceBackUnderscore {
	template <class Ti, class ...Ts>
	static auto call(Ti &&aT, Ts &&...aArgs) -> decltype(aT.emplace_back(Rr::Trait::forward<Ts>(aArgs)...))
	{
		return aT.emplace_back(Rr::Trait::forward<Ts>(aArgs)...);
	}
};

struct CallEmplaceBackCamelcase {
	template <class Ti, class ...Ts>
	static auto call(Ti &&aT, Ts &&...aArgs) -> decltype(aT.emplaceBack(Rr::Trait::forward<Ts>(aArgs)...))
	{
		return aT.emplaceBack(Rr::Trait::forward<Ts>(aArgs)...);
	}
};

struct CallBegin {
	template <class Ti>
	static auto call(Ti &aI) -> decltype(aI.begin())
	{
		aI.begin();
	}
};

struct CallEnd {
	template <class Ti>
	static auto call(Ti &&aI) -> decltype(aI.end())
	{
		aI.end();
	}
};

// Introspection

template <class Tv, template <class...> class Tc>
struct GetContainerType {
	static constexpr bool fIndexable = Rr::Refl::CanCallFamily<CallIndex>::check(Rr::Trait::eDeclval<Tc<Tv>>());
	static constexpr bool fKnownCapacity = Rr::Refl::CanCallFamily<CallCapacity,
		CallMaxSizeCamelcase, CallMaxSizeUnderscore>::check(Rr::Trait::eDeclval<Tc<Tv>>());

	static constexpr bool fHasMethodPush = Rr::Refl::CanCallFamily<CallPush, CallPushBackCamelcase,
		CallPushBackUnderscore>::check(Rr::Trait::eDeclval<Tc<Tv>>(), Rr::Trait::eDeclval<Tv>());

	static constexpr bool fHasMethodEmplace = Rr::Refl::CanCallFamily<CallEmplaceBackCamelcase,
		CallEmplaceBackUnderscore, CallEmplace>::check(Rr::Trait::eDeclval<Tc<int>>(), 42);

	static constexpr bool fExpandable = fHasMethodPush || fHasMethodEmplace;
	static constexpr bool fHasMethodBegin = Rr::Refl::CanCallFamily<CallBegin>::check(Rr::Trait::eDeclval<Tc<Tv>>());
	static constexpr bool fHasMethodEnd = Rr::Refl::CanCallFamily<CallEnd>::check(Rr::Trait::eDeclval<Tc<Tv>>());

	static constexpr auto value = fExpandable && fHasMethodBegin && fHasMethodEnd ? ContainerType::Push :
		fIndexable && fKnownCapacity ? ContainerType::Index :
		ContainerType::Incompatible;

	static_assert(value != ContainerType::Incompatible, "Incompatible container type");
};

// Implementation for different types derived through introspection

template <ContainerType, class Tv, template <class ...> class Tc>
struct Wrap;

template <class Tv, template <class ...> class Tc>
struct Wrap<ContainerType::Incompatible, Tv, Tc> {
};

///
/// @brief Implementation for std::array-like containers
///
template <class Tv, template <class ...> class Tc>
class Wrap<ContainerType::Index, Tv, Tc> : public Tc<Tv> {
	unsigned pos;
	const unsigned maxSize;
	using Base = Tc<Tv>;

public:
	using Iterator = decltype(&Rr::Trait::eDeclval<Tc<Tv>>().at(0));

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

///
/// @brief Implementation for std::list-like containers
///
template <class Tv, template <class ...> class Tc>
class Wrap<ContainerType::Push, Tv, Tc> : Tc<Tv> {
	unsigned sz = 0;
	using Base = Tc<Tv>;

public:
	using Iterator = decltype(Rr::Trait::eDeclval<Tc<Tv>>().begin());

	using Tc<Tv>::Tc;

	Wrap();
	unsigned size() const;
	Iterator begin();
	Iterator end();

	template <bool F = GetContainerType<Tv, Tc>::fHasMethodPush>
	typename Rr::Trait::EnableIf<F>::Type push(const Tv &aVal)
	{
		++sz;
		Rr::Refl::CallFamily<CallPush, CallPushBackCamelcase, CallPushBackUnderscore>::call(static_cast<Tc<Tv> &>(*this), aVal);
	}

	template <bool F = GetContainerType<Tv, Tc>::fHasMethodPush>
	typename Rr::Trait::EnableIf<!F>::Type push(const Tv &aVal)
	{
		return emplace(aVal);
	}

	template <bool F = GetContainerType<Tv, Tc>::fHasMethodEmplace, class ...Ts>
	typename Rr::Trait::EnableIf<F>::Type emplace(Ts &&...aArgs)
	{
		++sz;
		Rr::Refl::CallFamily<CallEmplaceBackCamelcase, CallEmplaceBackUnderscore, CallEmplace>::call(static_cast<Tc<Tv> &>(*this), Rr::Trait::forward<Ts>(aArgs)...);
	}

	template <bool F = GetContainerType<Tv, Tc>::fHasMethodEmplace, class ...Ts>
	typename Rr::Trait::EnableIf<!F>::Type emplace(Ts &&...aArgs)
	{
		push(Tv{Rr::Trait::forward<Ts>(aArgs)...});
	}
};

}  // namespace WrapImpl

template <class Tvalue, template <class ...> class Tcontainer>
using Wrap = typename WrapImpl::Wrap<WrapImpl::GetContainerType<Tvalue, Tcontainer>::value, Tvalue, Tcontainer>;

}  // namespace Cont
}  // namespace Rr

#include "Wrap.impl"

#endif // RR_CONT_WRAPPER_HPP
