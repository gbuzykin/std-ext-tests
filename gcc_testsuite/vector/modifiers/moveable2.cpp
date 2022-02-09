// { dg-do run { target c++11 } }

// 2011-06-07  Paolo Carlini  <paolo.carlini@oracle.com>

// Copyright (C) 2011-2022 Free Software Foundation, Inc.
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

#include "gcc_testsuite/util/testsuite_hooks.h"
#include "gcc_testsuite/util/testsuite_rvalref.h"
#include "util/vector.h"

namespace {

int test01() {
    using namespace __gnu_test;

    util::vector<throwing_move_constructor> v1;

    throwing_move_constructor tmc;

    v1.push_back(tmc);
    VERIFY(v1.size() == 1);

    v1.push_back(tmc);
    VERIFY(v1.size() == 2);

    v1.insert(v1.end(), tmc);
    VERIFY(v1.size() == 3);

    v1.insert(v1.end(), 100, tmc);
    VERIFY(v1.size() == 103);

    v1.insert(v1.end(), 10, tmc);
    VERIFY(v1.size() == 113);

    v1.insert(v1.end(), 1, tmc);
    VERIFY(v1.size() == 114);

    util::vector<throwing_move_constructor> v2;

    throwing_move_constructor tmca[] = {throwing_move_constructor(), throwing_move_constructor(),
                                        throwing_move_constructor(), throwing_move_constructor()};

    v2.insert(v2.end(), tmca, tmca + 1);
    VERIFY(v2.size() == 1);

    v2.insert(v2.end(), tmca, tmca + 4);
    VERIFY(v2.size() == 5);

    v2.insert(v2.end(), tmca, tmca + 2);
    VERIFY(v2.size() == 7);

    v2.insert(v2.end(), tmca, tmca + 1);
    VERIFY(v2.size() == 8);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
