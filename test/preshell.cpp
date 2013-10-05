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

#define BOOST_TEST_MODULE preshell

#include <preshell/preshell.hpp>

#include <boost/test/unit_test.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <list>
#include <string>
#include <algorithm>

using namespace preshell;

namespace
{
  std::list<std::string> split(const std::string& s_)
  {
    std::list<std::string> l;
    return boost::algorithm::split(l, s_, boost::is_any_of("\n"));
  }

  bool contains(const std::string& s_, const std::list<std::string>& l_)
  {
    return std::find(l_.begin(), l_.end(), s_) != l_.end();
  }

  template <unsigned int Result>
  unsigned int always()
  {
    return Result;
  }

  void should_not_be_called(std::string)
  {
    BOOST_CHECK(false);
  }

  indenter ind(always<80>, should_not_be_called);
}

BOOST_AUTO_TEST_CASE(test_empty_input)
{
  result_ptr r = precompile("", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r->output);
}

BOOST_AUTO_TEST_CASE(test_one_character_input)
{
  result_ptr r = precompile("a", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("a", r->output);
}

BOOST_AUTO_TEST_CASE(test_macro_call)
{
  const file_position position("<stdin>", 1, 1);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  context ctx;
  ctx.macros["M"] = macro_definition(position, def);
  result_ptr r = precompile("M", ctx, config::empty, ind);
  BOOST_CHECK_EQUAL("foo", r->output);
}

BOOST_AUTO_TEST_CASE(test_macro_definition)
{
  const file_position position("<stdin>", 1, 9);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  macro_map m;
  m["M"] = macro_definition(position, def);

  result_ptr r = precompile("#define M foo", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r->output);
  BOOST_CHECK_EQUAL(m["M"], r->pp_context.macros["M"]);
}

BOOST_AUTO_TEST_CASE(test_macro_deletion)
{
  const file_position position("<stdin>", 1, 9);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  context ctx;
  ctx.macros["M"] = macro_definition(position, def);

  result_ptr r = precompile("#undef M", ctx, config::empty, ind);
  BOOST_CHECK_EQUAL("", r->output);
  BOOST_CHECK(r->pp_context.macros.find("M") == r->pp_context.macros.end());
}

BOOST_AUTO_TEST_CASE(test_overriding_predefined_macro)
{
  const file_position position("<stdin>", 1, 1);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  context ctx;
  ctx.macros["__DATE__"] = macro_definition(position, def);
  result_ptr r = precompile("hello", ctx, config::empty, ind);
  BOOST_CHECK_EQUAL("hello", r->output);
}

BOOST_AUTO_TEST_CASE(test_missing_endif)
{
  const file_position position("<stdin>", 2, 9);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  macro_map m;
  m["M"] = macro_definition(position, def);

  result_ptr
    r = precompile("#if 1\n#define M foo", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r->output);
  BOOST_CHECK_EQUAL(m["M"], r->pp_context.macros["M"]);
}

BOOST_AUTO_TEST_CASE(test_in_if_0)
{
  result_ptr r1 = precompile("#if 0", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r1->output);
  result_ptr r2 = precompile("hello", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r2->output);
  result_ptr r3 = precompile("#else", r2->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r3->output);
  result_ptr r4 = precompile("foo", r3->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("foo", r4->output);
  result_ptr r5 = precompile("#endif", r4->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r5->output);
  result_ptr r6 = precompile("world", r5->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("world", r6->output);
}

BOOST_AUTO_TEST_CASE(test_in_if_1)
{
  result_ptr r1 = precompile("#if 1", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r1->output);
  result_ptr r2 = precompile("hello", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("hello", r2->output);
  result_ptr r3 = precompile("#else", r2->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r3->output);
  result_ptr r4 = precompile("foo", r3->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r4->output);
  result_ptr r5 = precompile("#endif", r4->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r5->output);
  result_ptr r6 = precompile("world", r5->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("world", r6->output);
}

BOOST_AUTO_TEST_CASE(test_if1_elif1)
{
  result_ptr r1 = precompile("#if 1", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r1->output);
  result_ptr r2 = precompile("hello", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("hello", r2->output);
  result_ptr r3 = precompile("#elif 1", r2->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r3->output);
  result_ptr r4 = precompile("foo", r3->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r4->output);
  result_ptr r5 = precompile("#endif", r4->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r5->output);
}

BOOST_AUTO_TEST_CASE(test_if1_elif0)
{
  result_ptr r1 = precompile("#if 1", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r1->output);
  result_ptr r2 = precompile("hello", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("hello", r2->output);
  result_ptr r3 = precompile("#elif 0", r2->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r3->output);
  result_ptr r4 = precompile("foo", r3->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r4->output);
  result_ptr r5 = precompile("#endif", r4->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r5->output);
}

BOOST_AUTO_TEST_CASE(test_if0_elif1)
{
  result_ptr r1 = precompile("#if 0", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r1->output);
  result_ptr r2 = precompile("hello", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r2->output);
  result_ptr r3 = precompile("#elif 1", r2->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r3->output);
  result_ptr r4 = precompile("foo", r3->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("foo", r4->output);
  result_ptr r5 = precompile("#endif", r4->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r5->output);
}

BOOST_AUTO_TEST_CASE(test_if0_elif0)
{
  result_ptr r1 = precompile("#if 0", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r1->output);
  result_ptr r2 = precompile("hello", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r2->output);
  result_ptr r3 = precompile("#elif 0", r2->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r3->output);
  result_ptr r4 = precompile("foo", r3->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r4->output);
  result_ptr r5 = precompile("#endif", r4->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r5->output);
}

BOOST_AUTO_TEST_CASE(test_if0_elif0_else)
{
  result_ptr r1 = precompile("#if 0", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r1->output);
  result_ptr r2 = precompile("hello", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r2->output);
  result_ptr r3 = precompile("#elif 0", r2->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r3->output);
  result_ptr r4 = precompile("foo", r3->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r4->output);
  result_ptr r5 = precompile("#else", r4->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r5->output);
  result_ptr r6 = precompile("bar", r5->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("bar", r6->output);
  result_ptr r7 = precompile("#endif", r6->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("", r7->output);
}

BOOST_AUTO_TEST_CASE(test_error)
{
  result_ptr r = precompile("#error foo", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r->output);
  BOOST_CHECK("" != r->error);
}

BOOST_AUTO_TEST_CASE(test_line)
{
  result_ptr r1 = precompile("__LINE__", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("1", r1->output);
  BOOST_CHECK_EQUAL(2, r1->pp_context.line);
  BOOST_REQUIRE_EQUAL(1, r1->pp_context.macros["__LINE__"].definition().size());
  BOOST_CHECK_EQUAL(
    "2",
    r1->pp_context.macros["__LINE__"].definition().front().get_value()
  );

  result_ptr r2 = precompile("__LINE__", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("2", r2->output);
  BOOST_CHECK_EQUAL(3, r2->pp_context.line);
  BOOST_REQUIRE_EQUAL(1, r2->pp_context.macros["__LINE__"].definition().size());
  BOOST_CHECK_EQUAL(
    "3",
    r2->pp_context.macros["__LINE__"].definition().front().get_value()
  );

  result_ptr r3 =
    precompile("#pragma wave macro_names", r2->pp_context, config::empty, ind);
  BOOST_CHECK(contains("__LINE__", split(r3->info)));

  result_ptr
    r4 = precompile("#pragma wave macros", r3->pp_context, config::empty, ind);
  BOOST_CHECK(contains("__LINE__ 4", split(r4->info)));
}

BOOST_AUTO_TEST_CASE(test_file)
{
  result_ptr r1 = precompile("__FILE__", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("\"<stdin>\"", r1->output);
  BOOST_CHECK_EQUAL("<stdin>", r1->pp_context.filename);
  BOOST_REQUIRE_EQUAL(1, r1->pp_context.macros["__FILE__"].definition().size());
  BOOST_CHECK_EQUAL(
    "\"<stdin>\"",
    r1->pp_context.macros["__FILE__"].definition().front().get_value()
  );

  result_ptr r2 = precompile("__FILE__", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("\"<stdin>\"", r2->output);
  BOOST_CHECK_EQUAL("<stdin>", r2->pp_context.filename);
  BOOST_REQUIRE_EQUAL(1, r2->pp_context.macros["__FILE__"].definition().size());
  BOOST_CHECK_EQUAL(
    "\"<stdin>\"",
    r2->pp_context.macros["__FILE__"].definition().front().get_value()
  );

  result_ptr r3 =
    precompile("#pragma wave macro_names", r2->pp_context, config::empty, ind);
  BOOST_CHECK(contains("__FILE__", split(r3->info)));

  result_ptr
    r4 = precompile("#pragma wave macros", r3->pp_context, config::empty, ind);
  BOOST_CHECK(contains("__FILE__ \"<stdin>\"", split(r4->info)));
}

BOOST_AUTO_TEST_CASE(test_line_override)
{
  result_ptr
    r1 = precompile("#line 13 \"foo.cpp\"", context(), config::empty, ind);
  BOOST_CHECK_EQUAL("", r1->output);
  BOOST_CHECK_EQUAL(13, r1->pp_context.line);
  BOOST_REQUIRE_EQUAL(1, r1->pp_context.macros["__LINE__"].definition().size());
  BOOST_CHECK_EQUAL(
    "13",
    r1->pp_context.macros["__LINE__"].definition().front().get_value()
  );
  BOOST_CHECK_EQUAL("foo.cpp", r1->pp_context.filename);
  BOOST_REQUIRE_EQUAL(1, r1->pp_context.macros["__FILE__"].definition().size());
  BOOST_CHECK_EQUAL(
    "\"foo.cpp\"",
    r1->pp_context.macros["__FILE__"].definition().front().get_value()
  );

  result_ptr r2 = precompile("__LINE__", r1->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("13", r2->output);
  BOOST_CHECK_EQUAL(14, r2->pp_context.line);
  BOOST_REQUIRE_EQUAL(1, r2->pp_context.macros["__LINE__"].definition().size());
  BOOST_CHECK_EQUAL(
    "14",
    r2->pp_context.macros["__LINE__"].definition().front().get_value()
  );
  BOOST_CHECK_EQUAL("foo.cpp", r2->pp_context.filename);
  BOOST_REQUIRE_EQUAL(1, r2->pp_context.macros["__FILE__"].definition().size());
  BOOST_CHECK_EQUAL(
    "\"foo.cpp\"",
    r2->pp_context.macros["__FILE__"].definition().front().get_value()
  );

  result_ptr r3 = precompile("__FILE__", r2->pp_context, config::empty, ind);
  BOOST_CHECK_EQUAL("\"foo.cpp\"", r3->output);

  result_ptr r4 =
    precompile("#pragma wave macro_names", r3->pp_context, config::empty, ind);
  BOOST_CHECK(contains("__FILE__", split(r4->info)));
  BOOST_CHECK(contains("__LINE__", split(r4->info)));

  result_ptr
    r5 = precompile("#pragma wave macros", r4->pp_context, config::empty, ind);
  BOOST_CHECK(contains("__FILE__ \"foo.cpp\"", split(r5->info)));
  BOOST_CHECK(contains("__LINE__ 16", split(r5->info)));
}


