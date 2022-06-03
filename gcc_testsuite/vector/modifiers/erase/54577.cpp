// { dg-do compile { target c++11 } }

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

#include "test_suite.h"

#include "uxs/vector.h"

namespace {

int test01() {
    uxs::vector<int> v1;
    v1.push_back(1);
    v1.erase(v1.cbegin());

    uxs::vector<int> v2;
    v2.push_back(2);
    v2.push_back(3);
    v2.erase(v2.cbegin(), v2.cend());

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
