// { dg-do run { target c++11 } }

// 2010-11-10  Paolo Carlini  <paolo.carlini@oracle.com>
//
// Copyright (C) 2010-2022 Free Software Foundation, Inc.
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

#include "gcc_testsuite/util/testsuite_hooks.h"
#include "gcc_testsuite/util/testsuite_rvalref.h"

#include "uxs/multiset.h"

#include <iterator>

namespace {

int test01() {
    using __gnu_test::rvalstruct;

    typedef uxs::multiset<rvalstruct> Set;
    Set s;
    VERIFY(s.empty());

    Set::iterator i = s.insert(rvalstruct(1));
    VERIFY(s.size() == 1);
    VERIFY(std::distance(s.begin(), s.end()) == 1);
    VERIFY(i == s.begin());
    VERIFY((*i).val == 1);

    return 0;
}

int test02() {
    using __gnu_test::rvalstruct;

    typedef uxs::multiset<rvalstruct> Set;
    Set s;
    VERIFY(s.empty());

    s.insert(rvalstruct(2));
    Set::iterator i = s.insert(rvalstruct(2));
    VERIFY(s.size() == 2);
    VERIFY(std::distance(s.begin(), s.end()) == 2);
    VERIFY((*i).val == 2);

    Set::iterator i2 = s.begin();
    ++i2;
    VERIFY(i == s.begin() || i == i2);
    VERIFY((*(s.begin())).val == 2 && (*i2).val == 2);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multiset", test01);
ADD_TEST_CASE("", "multiset", test02);
