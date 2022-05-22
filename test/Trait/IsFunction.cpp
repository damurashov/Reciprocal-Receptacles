#include <Rr/Trait/IsFunction.hpp>
#include <doctest/doctest.h>

using namespace Rr::Trait;

namespace TestTraitIsFunction {

struct S {
};

}  // TestTraitIsFunction

#define CHECK_EXPR_WRAP(a) {bool b = a; CHECK(b);}  // Some expressions, DOCTEST cannot digest

TEST_CASE("Trait::IsFunction") {
	// Short function signatures and function pointer types are both deemed functions
	CHECK_EXPR_WRAP(Rr::Trait::IsFunction<int(void)>::value);
	CHECK_EXPR_WRAP(IsFunction<int()>::value);
	CHECK_EXPR_WRAP(!IsFunction<int(*)(char)>::value);
	CHECK_EXPR_WRAP(!IsFunction<int(*)(void)>::value);
	CHECK_EXPR_WRAP(IsFunction<int()>::value);
	CHECK_EXPR_WRAP(IsFunction<int(char)>::value);

	// Methods are not considered functions
	CHECK_EXPR_WRAP(!IsFunction<int(TestTraitIsFunction::S::*)()>::value);
	CHECK_EXPR_WRAP(!IsFunction<int(TestTraitIsFunction::S::*)(int)>::value);
}

#undef TEST_EXPR_WRAP
