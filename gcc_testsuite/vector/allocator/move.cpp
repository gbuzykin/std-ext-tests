// Copyright (C) 2013-2022 Free Software Foundation, Inc.
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

// { dg-do run { target c++11 } }

#include "gcc_testsuite/util/testsuite_allocators.h"
#include "gcc_testsuite/util/testsuite_hooks.h"
#include "util/vector.h"

struct T {
    int i;
};

using __gnu_test::uneq_allocator;

namespace {

int test01() {
    typedef uneq_allocator<T> alloc_type;
    typedef util::vector<T, alloc_type> test_type;
    test_type v1(alloc_type(1));
    v1 = {T()};
    auto it = v1.begin();
    test_type v2(std::move(v1));
    VERIFY(1 == v1.get_allocator().get_personality());
    VERIFY(1 == v2.get_allocator().get_personality());
    VERIFY(it == v2.begin());
    return 0;
}

int test02() {
    typedef uneq_allocator<T> alloc_type;
    typedef util::vector<T, alloc_type> test_type;
    test_type v1(alloc_type(1));
    v1 = {T()};
    test_type v2(std::move(v1), alloc_type(2));
    VERIFY(1 == v1.get_allocator().get_personality());
    VERIFY(2 == v2.get_allocator().get_personality());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
ADD_TEST_CASE("", "vector", test02);
