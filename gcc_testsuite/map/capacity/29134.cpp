// Copyright (C) 2006-2022 Free Software Foundation, Inc.
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

// 23.3.1 map capacity

#include "gcc_testsuite/util/testsuite_allocator.h"
#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/map.h"

namespace {

// libstdc++/29134
int test01() {
    uxs::map<int, int> m;

    std::allocator<uxs::detail::map_node_type<int, int>> a;
    VERIFY(m.max_size() == __gnu_test::max_size(a));
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test01);
