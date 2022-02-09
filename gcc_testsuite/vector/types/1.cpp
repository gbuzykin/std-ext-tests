// 2005-12-01  Paolo Carlini  <pcarlini@suse.de>

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

// { dg-do compile }
// { dg-options "-Wno-unused-result" }

#include "gcc_testsuite/util/testsuite_greedy_ops.h"
#include "util/vector.h"

namespace {

int test01() {
    util::vector<greedy_ops::X> v(5);
    const util::vector<greedy_ops::X> w(1);

    v[0];
    w[0];
    v.size();
    v.capacity();
    v.resize(1);
    v.insert(v.begin(), greedy_ops::X());
    v.insert(v.begin(), 1, greedy_ops::X());
    v.insert(v.begin(), w.begin(), w.end());
    v = w;

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
