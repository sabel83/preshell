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

#include <preshell/preshell.hpp>
#include <preshell/util.hpp>
#include "test_util.hpp"

#include <boost/test/unit_test.hpp>

#include <string>

using namespace preshell;

namespace
{
  indenter ind(always<80>, should_not_be_called);
}

BOOST_AUTO_TEST_CASE(test_definition_is_logged)
{
  std::list<std::string> history;
  config cfg = config::empty;
  cfg.log_macro_definitions = true;
  result_ptr r = precompile("#define foo bar", context(), cfg, ind, history);
  BOOST_CHECK_EQUAL("<stdin>:1:9: #define foo\n", r->info);
}

BOOST_AUTO_TEST_CASE(test_definition_is_not_logged_when_disabled)
{
  std::list<std::string> history;
  config cfg = config::empty;
  cfg.log_macro_definitions = false;
  result_ptr r = precompile("#define foo bar", context(), cfg, ind, history);
  BOOST_CHECK_EQUAL("", r->info);
}

BOOST_AUTO_TEST_CASE(test_undefinition_is_logged)
{
  std::list<std::string> history;
  config cfg = config::empty;
  cfg.log_macro_definitions = true;
  result_ptr r0 = precompile("#define foo bar", context(), cfg, ind, history);

  result_ptr r = precompile("#undef foo", r0->pp_context, cfg, ind, history);
  BOOST_CHECK_EQUAL("<stdin>:2:1: #undef foo\n", r->info);
}


