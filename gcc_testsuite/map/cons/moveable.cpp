// { dg-do run { target c++11 } }

// Copyright (C) 2005-2022 Free Software Foundation, Inc.
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

// NOTE: This makes use of the fact that we know how moveable
// is implemented on map (via swap). If the implementation changed
// this test may begin to fail.

#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/map.h"

#include <utility>

namespace {

int test01() {
    uxs::map<int, int> a, b;
    a[2] = 0;
    b[1] = 0;
    b = std::move(a);
    VERIFY(b.find(2) != b.end() && a.find(1) == a.end());

    uxs::map<int, int> c(std::move(b));
    VERIFY(c.find(2) != c.end());
    VERIFY(b.find(2) == b.end());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test01);
