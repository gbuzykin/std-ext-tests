// Copyright (C) 2012-2022 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// { dg-do compile { target c++11 } }

// libstdc++/52591

#include "uxs/vector.h"

#include <memory>
#include <type_traits>

// Move-assignment of uxs::vector<T> is allowed for non-MoveAssignable T when
// the allocator type propagates. As an extension we also allow it if the
// allocator type is known to always compare equal.

namespace {

struct C {
    C& operator=(C&&) = delete;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void test01() {
    uxs::vector<C> a;
    a = uxs::vector<C>();
}
#pragma GCC diagnostic pop

template<typename T>
struct A1 : std::allocator<T> {
    template<typename U>
    struct rebind {
        typedef A1<U> other;
    };

    A1() = default;
    template<typename U>
    A1(const A1<U>&) {}

    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::false_type;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void test02() {
    using test_type = uxs::vector<C, A1<C>>;
    static_assert(std::is_nothrow_move_assignable<test_type>::value,
                  "vector is nothrow move-assignable if allocator propagates");
}
#pragma GCC diagnostic pop

template<typename T>
struct A2 : std::allocator<T> {
    template<typename U>
    struct rebind {
        typedef A2<U> other;
    };

    A2() = default;
    template<typename U>
    A2(const A2<U>&) {}

    using propagate_on_container_move_assignment = std::false_type;
    using is_always_equal = std::true_type;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
void test03() {
    using test_type = uxs::vector<C, A2<C>>;
    static_assert(std::is_nothrow_move_assignable<test_type>::value,
                  "vector is nothrow move-assignable if allocator is always equal");
}
#pragma GCC diagnostic pop

}  // namespace
