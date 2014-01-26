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

#include "test_shell.hpp"

#include <boost/test/unit_test.hpp>

using namespace preshell;

BOOST_AUTO_TEST_CASE(test_empty_history)
{
  test_shell sh(config::empty);

  BOOST_CHECK(sh.history().empty());
}

BOOST_AUTO_TEST_CASE(test_one_element_history)
{
  test_shell sh(config::empty);

  sh.line_available("foo");

  BOOST_CHECK_EQUAL(1, sh.history().size());
  BOOST_CHECK_EQUAL("foo", sh.history().front());
}

BOOST_AUTO_TEST_CASE(test_two_element_history)
{
  test_shell sh(config::empty);

  sh.line_available("foo");
  sh.line_available("bar");

  BOOST_CHECK_EQUAL(2, sh.history().size());
  BOOST_CHECK_EQUAL("foo", sh.history().front());
  BOOST_CHECK_EQUAL("bar", sh.history().back());
}

BOOST_AUTO_TEST_CASE(test_repetition_in_history)
{
  std::vector<std::string> add_history_callback_log;

  test_shell sh(add_history_callback_log);

  sh.line_available("foo");
  sh.line_available("foo");

  BOOST_CHECK_EQUAL(2, sh.history().size());
  BOOST_CHECK_EQUAL(1, add_history_callback_log.size());
}

BOOST_AUTO_TEST_CASE(test_empty_line_in_history)
{
  test_shell sh(config::empty);

  sh.line_available("");

  BOOST_CHECK(sh.history().empty());
}

BOOST_AUTO_TEST_CASE(test_pragma_in_history)
{
  test_shell sh(config::empty);

  sh.line_available("#pragma wave preshell_help");

  BOOST_CHECK(sh.history().empty());
}


