// 2005-09-12  Paolo Carlini  <pcarlini@suse.de>
//
// Copyright (C) 2005-2022 Free Software Foundation, Inc.
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

// { dg-do compile }

#include "uxs/vector.h"

#include "test_suite.h"

namespace {

struct T {
    typedef uxs::vector<int> Vector;
    typedef Vector::iterator iterator;
    typedef Vector::const_iterator const_iterator;

    int t(iterator f) { return *f; }
    int t(const_iterator f) const { return *f; }
};

// libstdc++/23767
int test01() {
    uxs::vector<int> v(1);
    T t;
    T::const_iterator i = v.begin();

    t.t(i);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "vector", test01);
