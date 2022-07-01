// { dg-do run { target c++11 } }

// Copyright (C) 2012-2022 Free Software Foundation, Inc.
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

#include "uxs/map.h"
#include "uxs/vector.h"

#include <tuple>
#include <utility>

namespace {

class PathPoint {
 public:
    PathPoint(char t, const uxs::vector<double>& c) : type(t), coords(c) {}
    PathPoint(char t, uxs::vector<double>&& c) : type(t), coords(std::move(c)) {}
    char getType() const { return type; }
    const uxs::vector<double>& getCoords() const { return coords; }

 private:
    char type;
    uxs::vector<double> coords;
};

int test01() {
    typedef uxs::map<char, uxs::vector<double>> Map;
    Map m;

    uxs::vector<double> coord1 = {0.0, 1.0, 2.0};

    auto ret = m.emplace('a', coord1);
    VERIFY(ret.second);
    VERIFY(m.size() == 1);
    VERIFY(ret.first->first == 'a');

    coord1[0] = 3.0;
    ret = m.emplace('a', coord1);
    VERIFY(!ret.second);
    VERIFY(m.size() == 1);
    VERIFY(ret.first->first == 'a');
    VERIFY(ret.first->second[0] == 0.0);

    auto it = m.emplace_hint(m.begin(), 'b', coord1);
    VERIFY(it != m.end());
    VERIFY(it->first == 'b');
    VERIFY(it->second[0] == 3.0);

    double* px = &coord1[0];
    ret = m.emplace('c', std::move(coord1));
    VERIFY(ret.second);
    VERIFY(ret.first->first == 'c');
    VERIFY(&(ret.first->second[0]) == px);

    return 0;
}

int test02() {
    typedef uxs::map<char, PathPoint> Map;
    Map m;

    uxs::vector<double> coord1 = {0.0, 1.0, 2.0};

    auto ret = m.emplace(std::piecewise_construct, std::make_tuple('a'), std::make_tuple('a', coord1));
    VERIFY(ret.second);
    VERIFY(m.size() == 1);
    VERIFY(ret.first->first == 'a');

    coord1[0] = 3.0;
    ret = m.emplace(std::piecewise_construct, std::make_tuple('a'), std::make_tuple('b', coord1));
    VERIFY(!ret.second);
    VERIFY(m.size() == 1);
    VERIFY(ret.first->first == 'a');
    VERIFY(ret.first->second.getCoords()[0] == 0.0);

    auto it = m.emplace_hint(m.begin(), std::piecewise_construct, std::make_tuple('b'), std::make_tuple('c', coord1));
    VERIFY(it != m.end());
    VERIFY(it->first == 'b');
    VERIFY(it->second.getCoords()[0] == 3.0);

    double* px = &coord1[0];
    ret = m.emplace(std::piecewise_construct, std::make_tuple('c'), std::make_tuple('d', std::move(coord1)));
    VERIFY(ret.second);
    VERIFY(ret.first->first == 'c');
    VERIFY(&(ret.first->second.getCoords()[0]) == px);

    return 0;
}

}  // namespace

ADD_TEST_CASE("", "map", test01);
ADD_TEST_CASE("", "map", test02);
