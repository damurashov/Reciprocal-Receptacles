//
// Function.hpp
//
// Created on:  Jun 28, 2022
//     Author: Dmitry Murashov (dmtr <AT> murashov <DOT> gmail.com)
//

#if !defined(RR_CB_FUNCTION_HPP_)
#define RR_CB_FUNCTION_HPP_

#include <Rr/Util/DefaultConfig.hpp>
#include <Rr/Trait/MemberDecay.hpp>
#include <Rr/Trait/Move.hpp>

namespace Rr {
namespace Cb {

template <class Tsg, class ...Ts, template <class ...> class Tal>
class Function<Tsg, Tal<Ts...>> {
private:
	using Ret = typename Rr::Trait::MemberDecay<Tsg>::ReturnType;

	struct Object {
		void *(*constructor)(void *);
		void (*destructor)(void *);
		Ret (*callable)(void *, Ts...);

		template <class T>
		static void* functorConstruct(void *aOther)
		{
			return new T{*reinterpret_cast<T *>(aOther)};
		}

		template <class T>
		static void functorDestruct(void *aInstance)
		{
			if (nullptr != aInstance) {
				reinterpret_cast<T *>(aInstance)->~T();
				delete reinterpret_cast<T *>(aInstance);
			}
		}

		template <class T>
		static Ret functorInvoke(void *aIntsance, Ts ...aArgs)
		{
			return (*reinterpret_cast<T *>(aInstance))(aArgs...);
		}

		Object() : constructor{nullptr}, destructor{nullptr}, callable{nullptr}
		{
		}

		Object(const Object &a) : constructor{a.consturctor}, destructor{a.destructor}, callable{a.callable}
		{
		}

		Object(Object &&a) : constructor{a.consturctor}, destructor{a.destructor}, callable{a.callable}
		{
			a = Object{};
		}

		Object &operator=(const Object &a)
		{
			constructor = a.constructor;
			destructor = a.destructor;
			callable = a.callable;
		}

		Object &operator=(Object &&a)
		{
			constructor = a.constructor;
			destructor = a.destructor;
			callable = a.callable;
			a.constructor = nullptr;
			a.destructor = nullptr;
			a.callable = nullptr;
		}
	};

public:
	static_assert(RRO_USE_OPERATOR_NEW, "Implementation of function requires dynamic memory allocation");

	Ret operator()(Ts ...aArgs)
	{
		rr_assert(nullptr != functor);
		return object.callable(aArgs...);
	}

	operator bool()
	{
		return nullptr != functor;
	}

	template <class Tf>
	Function(Tf &&aFn) : object{Object<Tf>{}}, functor{object.constructor(&aFn)}
	{
	}

	Function() : object{}, functor{nullptr}
	{
	}

	Function(const Function &a)
	{
		object.destructor(functor);
		object = a.object;
		functor = object.constructor(a.functor);
	}

	Function(Function &&a)
	{
		object.destructor(functor);
		object{Rr::Trait::move(a.object)};
		functor = a.functor;
		a.functor = nullptr;
		a.object = Object{};
	}

	Function &operator=(const Function &a)
	{
		object.destructor(functor);
		object = a.object;
		functor = a.functor;

		return *this;
	}

	Function &operator=(Function &&a)
	{
		object.destructor(functor);
		object{Rr::Trait::move(a.object)};
		functor = a.functor;
		a.functor = nullptr;
		a.object = Object{};

		return *this;
	}

private:
	Object object;
	void *functor;
};

}  // Cb
}  // Rr

#endif  // RR_CB_FUNCTION_HPP_
