// Copyright (C) 2014-2022 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.
//

// { dg-do run { target c++11 } }

#include "gcc_testsuite/util/testsuite_allocator.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/map.h"

namespace {

int test01() {
    using namespace __gnu_test;

    typedef tracker_allocator<std::pair<const int, int>> alloc_type;
    typedef uxs::map<int, int, std::less<int>, alloc_type> test_type;

    tracker_allocator_counter::reset();

    test_type v1;
    v1 = {{0, 0}, {1, 1}};

    auto allocs = tracker_allocator_counter::get_allocation_count();
    auto constructs = tracker_allocator_counter::get_construct_count();

    VERIFY(allocs != 0);
    VERIFY(constructs != 0);

    // Check no allocation on list initialization.
    v1 = {{4, 4}, {5, 5}};

    VERIFY(tracker_allocator_counter::get_allocation_count() == allocs);
    VERIFY(tracker_allocator_counter::get_construct_count() == constructs /*+ 2*/);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test01);
