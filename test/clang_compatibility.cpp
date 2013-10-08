// Preshell - Interactive C/C++ preprocessor shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include "test_shell.hpp"

#include <boost/test/unit_test.hpp>

using namespace preshell;

BOOST_AUTO_TEST_CASE(test_has_builtin)
{
  test_shell sh(config::default_config);

  sh.line_available("#if __has_builtin(__builtin_trap)");

  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_has_feature)
{
  test_shell sh(config::default_config);

  sh.line_available("#if __has_feature(cxx_rvalue_references)");

  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_has_extension)
{
  test_shell sh(config::default_config);

  sh.line_available("#if __has_extension(cxx_rvalue_references)");

  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_has_attribute)
{
  test_shell sh(config::default_config);

  sh.line_available("#if __has_attribute(always_inline)");

  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_has_include)
{
  test_shell sh(config::default_config);

  sh.line_available("#if __has_include(\"myinclude.h\")");

  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_has_include_next)
{
  test_shell sh(config::default_config);

  sh.line_available("#if __has_include_next(\"myinclude.h\")");

  BOOST_CHECK_EQUAL("", sh.error());
}

BOOST_AUTO_TEST_CASE(test_has_warning)
{
  test_shell sh(config::default_config);

  sh.line_available("#if __has_warning(-Wformat)");

  BOOST_CHECK_EQUAL("", sh.error());
}


