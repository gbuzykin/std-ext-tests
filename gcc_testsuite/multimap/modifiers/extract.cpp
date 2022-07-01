// Copyright (C) 2016-2022 Free Software Foundation, Inc.
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

// { dg-do run { target c++17 } }

#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/map.h"
#include "uxs/multimap.h"

#include <algorithm>

namespace {

using test_type = uxs::multimap<int, int>;

int test01() {
    test_type c{{1, 10}, {1, 11}, {2, 20}, {2, 21}, {3, 30}, {3, 31}};
    test_type::node_type node;
    test_type::iterator pos;

    node = c.extract(0);
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 6);

    pos = c.insert(std::move(node));
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 6);
    VERIFY(pos == c.end());

    node = c.extract(1);
    VERIFY((bool)node);
    VERIFY(!node.empty());
    VERIFY(c.size() == 5);
    VERIFY(c.count(1) == 1);
    VERIFY(node.get_allocator() == c.get_allocator());
    VERIFY(node.key() == 1);
    int mapped = node.mapped();
    VERIFY(mapped == 10 || mapped == 11);

    node.key() = 4;
    node.mapped() = 40;
    VERIFY(node.key() == 4);
    VERIFY(node.mapped() == 40);

    pos = c.insert(std::move(node));
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 6);
    VERIFY(pos != c.end());
    VERIFY(pos->first == 4);
    VERIFY(pos->second == 40);
    VERIFY(c.count(1) == 1);
    VERIFY(c.count(4) == 1);
    VERIFY(std::is_sorted(c.begin(), c.end()));

    pos = c.insert(c.begin(), std::move(node));
    VERIFY(!node);
    VERIFY(node.empty());
    VERIFY(c.size() == 6);
    VERIFY(pos == c.end());

    node = c.extract(1);
    mapped = node.mapped();
    pos = c.insert(c.begin(), std::move(node));
    VERIFY(!node);
    VERIFY(c.size() == 6);
    VERIFY(pos != c.end());
    VERIFY(pos->first == 1);
    VERIFY(pos->second == mapped);

    test_type c2 = c;
    node = c2.extract(1);
    mapped = node.mapped();
    pos = c.insert(std::move(node));
    VERIFY(pos != c.end());
    VERIFY(node.empty());
    VERIFY(pos->first == 1);
    VERIFY(pos->second == mapped);

    return 0;
}

int test02() {
    test_type c{{1, 10}, {1, 11}, {2, 20}, {2, 21}, {3, 30}, {3, 31}};
    test_type::node_type node;
    test_type::iterator pos;

    node = c.extract(c.begin());
    VERIFY(!node.empty());
    VERIFY(c.size() == 5);
    VERIFY(node.get_allocator() == c.get_allocator());
    VERIFY(node.key() == 1);
    VERIFY(node.mapped() == 10);

    pos = c.insert(std::next(c.begin()), std::move(node));
    VERIFY(node.empty());
    VERIFY(c.size() == 6);
    VERIFY(pos != c.end());
    VERIFY(pos->first == 1);
    VERIFY(pos->second == 10);
    VERIFY(pos == std::next(c.begin()));

    return 0;
}

int test03() {
    struct less : std::less<int> {};
    using compat_type1 = uxs::multimap<int, int, less>;
    static_assert(std::is_same<test_type::node_type, compat_type1::node_type>::value, "");
    using compat_type2 = uxs::map<int, int>;
    static_assert(std::is_same<test_type::node_type, compat_type2::node_type>::value, "");
    using compat_type3 = uxs::map<int, int, less>;
    static_assert(std::is_same<test_type::node_type, compat_type3::node_type>::value, "");
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "multimap", test01);
ADD_TEST_CASE("", "multimap", test02);
ADD_TEST_CASE("", "multimap", test03);
