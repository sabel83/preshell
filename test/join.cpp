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
#include <preshell/token.hpp>

#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

using namespace preshell;

namespace
{
  std::string parse_and_output(const std::string& s_)
  {
    const boost::wave::cpplexer::lex_iterator<token>
      b(s_.begin(), s_.end(), token::position_type(), boost::wave::support_cpp),
      e;
    const bool never_cancel = false;
    std::vector<std::string>* const no_warnings = 0;
    return join(b, e, no_warnings, never_cancel, true);
  }
}

BOOST_AUTO_TEST_CASE(test_join_suppressing_empty_line)
{
  BOOST_REQUIRE_EQUAL("", parse_and_output("\n"));
}

BOOST_AUTO_TEST_CASE(test_join_not_suppressing_nonempty_line)
{
  BOOST_REQUIRE_EQUAL("a\n", parse_and_output("a\n"));
}

BOOST_AUTO_TEST_CASE(test_suppressing_with_cpp_comment)
{
  BOOST_REQUIRE_EQUAL("// comment \nx", parse_and_output("// comment \n\nx"));
}

BOOST_AUTO_TEST_CASE(test_suppressing_with_c_comment)
{
  BOOST_REQUIRE_EQUAL("// comment \nx", parse_and_output("// comment \n\nx"));
}


