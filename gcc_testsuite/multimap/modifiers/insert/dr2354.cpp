// Copyright (C) 2017-2022 Free Software Foundation, Inc.
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

#include "uxs/multimap.h"

namespace {

struct MoveOnly {
    MoveOnly(int) {}
    MoveOnly(MoveOnly&&) = default;
};

void test01() {
    uxs::multimap<int, MoveOnly> m;
    m.insert({1, 2});  // PR libstdc++/82522  - LWG 2354
}

void test02() {
    uxs::multimap<int, MoveOnly> m;
    m.insert(m.begin(), {1, 2});  // PR libstdc++/82522  - LWG 2354
}

}  // namespace
