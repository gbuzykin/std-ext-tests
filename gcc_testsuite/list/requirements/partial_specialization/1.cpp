// Copyright (C) 1999-2022 Free Software Foundation, Inc.
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

// You should have received a copy of the GNU General Public License uxs::list
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// This file tests user specialization of library containers

#include "gcc_testsuite/util/testsuite_hooks.h"

#include "uxs/list.h"

// { dg-do compile }

struct user_type {};

namespace uxs {
template<typename Allocator>
class list<user_type, Allocator> {};
}  // namespace uxs
