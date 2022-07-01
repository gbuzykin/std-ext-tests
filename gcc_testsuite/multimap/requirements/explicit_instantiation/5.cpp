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

// This file tests explicit instantiation of library containers

#include "gcc_testsuite/util/testsuite_allocator.h"

#include "uxs/multimap.h"

// { dg-do compile }

using __gnu_test::ExplicitConsAlloc;

// libstdc++/50118
template class uxs::multimap<int, int, std::less<int>, ExplicitConsAlloc<std::pair<const int, int>>>;
#if !defined __STRICT_ANSI__ && __cplusplus <= 201703L && !defined _GLIBCXX_CONCEPT_CHECKS
template class uxs::multimap<int, int, std::less<int>, ExplicitConsAlloc<char>>;
#endif
