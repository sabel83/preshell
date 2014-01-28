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
#include "temp_header.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>

#include <boost/bind.hpp>

using namespace preshell;

namespace
{
  template <class F>
  void test_changing_header_between_replay_impl(F replay_history_)
  {
    temp_header header;

    config cfg = config::empty;
    cfg.sysinclude_path.push_back(header.include_dir());

    test_shell sh(cfg);

    header.create("#define FOO bar");
    sh.line_available(header.include_command());
    header.create("#define FOO foo");
    replay_history_(sh);
    sh.line_available("FOO");

    BOOST_CHECK_EQUAL("foo", sh.normal());
    BOOST_CHECK_EQUAL("", sh.error());
  }
}

BOOST_AUTO_TEST_CASE(test_replay_no_include)
{
  test_shell sh(config::empty);

  sh.line_available("#define FOO bar");
  sh.replay_history();
  sh.line_available("FOO");

  BOOST_CHECK_EQUAL("bar", sh.normal());
}

BOOST_AUTO_TEST_CASE(test_temp_header)
{
  temp_header header;

  config cfg = config::empty;
  cfg.sysinclude_path.push_back(header.include_dir());

  test_shell sh(cfg);

  header.create("#define FOO bar");
  sh.line_available(header.include_command());
  sh.line_available("FOO");

  BOOST_CHECK_EQUAL("bar", sh.normal());
}

BOOST_AUTO_TEST_CASE(test_changing_header_between_replay)
{
  test_changing_header_between_replay_impl(
    boost::bind(&test_shell::replay_history, _1)
  );
}

BOOST_AUTO_TEST_CASE(test_no_output_during_replay_history)
{
  test_shell sh(config::empty);

  sh.line_available("foo bar");
  sh.replay_history();

  BOOST_CHECK_EQUAL("foo bar", sh.normal());
}

BOOST_AUTO_TEST_CASE(test_pragma_for_replay_history)
{
  test_changing_header_between_replay_impl(
    boost::bind(&test_shell::line_available, _1, "#pragma wave replay_history")
  );
}


