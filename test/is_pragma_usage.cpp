// Preshell - Interactive C/C++ preprocessor shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <preshell/util.hpp>

#include <just/test.hpp>

using namespace preshell;

JUST_TEST_CASE(test_no_pragma_usage)
{
  JUST_ASSERT(!is_pragma_usage("hello"));
}

JUST_TEST_CASE(test_pragma_usage)
{
  JUST_ASSERT(is_pragma_usage("#pragma hello"));
}

JUST_TEST_CASE(test_pragma_with_space)
{
  JUST_ASSERT(is_pragma_usage("# pragma hello"));
  JUST_ASSERT(is_pragma_usage("  #pragma hello"));
}

JUST_TEST_CASE(test_pragma_in_comment)
{
  JUST_ASSERT(!is_pragma_usage("/* #pragma hello */"));
  JUST_ASSERT(!is_pragma_usage("// #pragma hello"));
}


