//
// CallPolicy.cpp
//
// Created on: 2022-01-02
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <Rr/Sync/Policy/Call.hpp>
#include <Rr/Trait/IsSame.hpp>

struct DefinesPolicy {
    static constexpr auto kPolicy = Rr::Sync::Policy::Type::None;
};

struct DefinesCallPolicy {
    static constexpr auto kCallPolicy = Rr::Sync::Policy::Type::Mutex;
    using Mutex = void;
    using Lock = void;
};

int main(void)
{
    Rr::Sync::Policy::Call<DefinesPolicy> c;
    Rr::Sync::Policy::Call<DefinesCallPolicy> c2;
}