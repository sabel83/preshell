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

#include <just/test.hpp>

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

  indenter ind(always<80>, should_not_be_called);
}

JUST_TEST_CASE(test_empty_input)
{
  std::list<std::string> history;
  result_ptr r = precompile("", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r->output);
}

JUST_TEST_CASE(test_one_character_input)
{
  std::list<std::string> history;
  result_ptr r = precompile("a", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("a", r->output);
}

JUST_TEST_CASE(test_macro_call)
{
  std::list<std::string> history;
  const file_position position("<stdin>", 1, 1);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  context ctx;
  ctx.macros["M"] = macro_definition(position, def);
  result_ptr r = precompile("M", ctx, config::empty, ind, history);
  JUST_ASSERT_EQUAL("foo", r->output);
}

JUST_TEST_CASE(test_macro_definition)
{
  std::list<std::string> history;
  const file_position position("<stdin>", 1, 9);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  macro_map m;
  m["M"] = macro_definition(position, def);

  result_ptr r =
    precompile("#define M foo", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r->output);
  JUST_ASSERT_EQUAL(m["M"], r->pp_context.macros["M"]);
}

JUST_TEST_CASE(test_macro_deletion)
{
  std::list<std::string> history;
  const file_position position("<stdin>", 1, 9);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  context ctx;
  ctx.macros["M"] = macro_definition(position, def);

  result_ptr r = precompile("#undef M", ctx, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r->output);
  JUST_ASSERT(r->pp_context.macros.find("M") == r->pp_context.macros.end());
}

JUST_TEST_CASE(test_overriding_predefined_macro)
{
  std::list<std::string> history;
  const file_position position("<stdin>", 1, 1);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  context ctx;
  ctx.macros["__DATE__"] = macro_definition(position, def);
  result_ptr r = precompile("hello", ctx, config::empty, ind, history);
  JUST_ASSERT_EQUAL("hello", r->output);
}

JUST_TEST_CASE(test_missing_endif)
{
  std::list<std::string> history;
  const file_position position("<stdin>", 2, 9);
  std::list<token> def;
  def.push_back(token(boost::wave::T_IDENTIFIER, "foo", position));

  macro_map m;
  m["M"] = macro_definition(position, def);

  result_ptr r =
    precompile("#if 1\n#define M foo", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r->output);
  JUST_ASSERT_EQUAL(m["M"], r->pp_context.macros["M"]);
}

JUST_TEST_CASE(test_in_if_0)
{
  std::list<std::string> history;
  result_ptr r1 = precompile("#if 0", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r1->output);
  result_ptr r2 =
    precompile("hello", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r2->output);
  result_ptr r3 =
    precompile("#else", r2->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r3->output);
  result_ptr r4 =
    precompile("foo", r3->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("foo", r4->output);
  result_ptr r5 =
    precompile("#endif", r4->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r5->output);
  result_ptr r6 =
    precompile("world", r5->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("world", r6->output);
}

JUST_TEST_CASE(test_in_if_1)
{
  std::list<std::string> history;
  result_ptr r1 = precompile("#if 1", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r1->output);
  result_ptr r2 =
    precompile("hello", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("hello", r2->output);
  result_ptr r3 =
    precompile("#else", r2->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r3->output);
  result_ptr r4 =
    precompile("foo", r3->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r4->output);
  result_ptr r5 =
    precompile("#endif", r4->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r5->output);
  result_ptr r6 =
    precompile("world", r5->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("world", r6->output);
}

JUST_TEST_CASE(test_if1_elif1)
{
  std::list<std::string> history;
  result_ptr r1 = precompile("#if 1", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r1->output);
  result_ptr r2 =
   precompile("hello", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("hello", r2->output);
  result_ptr r3 =
    precompile("#elif 1", r2->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r3->output);
  result_ptr r4 =
    precompile("foo", r3->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r4->output);
  result_ptr r5 =
    precompile("#endif", r4->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r5->output);
}

JUST_TEST_CASE(test_if1_elif0)
{
  std::list<std::string> history;
  result_ptr r1 = precompile("#if 1", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r1->output);
  result_ptr r2 =
    precompile("hello", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("hello", r2->output);
  result_ptr r3 =
    precompile("#elif 0", r2->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r3->output);
  result_ptr r4 =
    precompile("foo", r3->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r4->output);
  result_ptr r5 =
    precompile("#endif", r4->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r5->output);
}

JUST_TEST_CASE(test_if0_elif1)
{
  std::list<std::string> history;
  result_ptr r1 = precompile("#if 0", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r1->output);
  result_ptr r2 =
    precompile("hello", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r2->output);
  result_ptr r3 =
    precompile("#elif 1", r2->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r3->output);
  result_ptr r4 =
    precompile("foo", r3->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("foo", r4->output);
  result_ptr r5 =
    precompile("#endif", r4->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r5->output);
}

JUST_TEST_CASE(test_if0_elif0)
{
  std::list<std::string> history;
  result_ptr r1 = precompile("#if 0", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r1->output);
  result_ptr r2 =
    precompile("hello", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r2->output);
  result_ptr r3 =
    precompile("#elif 0", r2->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r3->output);
  result_ptr r4 =
    precompile("foo", r3->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r4->output);
  result_ptr r5 =
    precompile("#endif", r4->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r5->output);
}

JUST_TEST_CASE(test_if0_elif0_else)
{
  std::list<std::string> history;
  result_ptr r1 = precompile("#if 0", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r1->output);
  result_ptr r2 =
    precompile("hello", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r2->output);
  result_ptr r3 =
    precompile("#elif 0", r2->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r3->output);
  result_ptr r4 =
    precompile("foo", r3->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r4->output);
  result_ptr r5 =
    precompile("#else", r4->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r5->output);
  result_ptr r6 =
    precompile("bar", r5->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("bar", r6->output);
  result_ptr r7 =
    precompile("#endif", r6->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r7->output);
}

JUST_TEST_CASE(test_error)
{
  std::list<std::string> history;
  result_ptr r =
   precompile("#error foo", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r->output);
  JUST_ASSERT("" != r->error);
}

JUST_TEST_CASE(test_line)
{
  std::list<std::string> history;
  result_ptr r1 =
    precompile("__LINE__", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("1", r1->output);
  JUST_ASSERT_EQUAL(2, r1->pp_context.line);
  JUST_ASSERT_EQUAL(1, r1->pp_context.macros["__LINE__"].definition().size());
  JUST_ASSERT_EQUAL(
    "2",
    r1->pp_context.macros["__LINE__"].definition().front().get_value()
  );

  result_ptr r2 =
    precompile("__LINE__", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("2", r2->output);
  JUST_ASSERT_EQUAL(3, r2->pp_context.line);
  JUST_ASSERT_EQUAL(1, r2->pp_context.macros["__LINE__"].definition().size());
  JUST_ASSERT_EQUAL(
    "3",
    r2->pp_context.macros["__LINE__"].definition().front().get_value()
  );

  result_ptr r3 =
    precompile(
      "#pragma wave macro_names",
      r2->pp_context,
      config::empty,
      ind,
      history
    );
  JUST_ASSERT(contains("__LINE__", split(r3->info)));

  result_ptr r4 =
    precompile(
      "#pragma wave macros",
      r3->pp_context,
      config::empty,
      ind,
      history
    );
  JUST_ASSERT(contains("__LINE__ 4", split(r4->info)));
}

JUST_TEST_CASE(test_file)
{
  std::list<std::string> history;
  result_ptr r1 =
    precompile("__FILE__", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("\"<stdin>\"", r1->output);
  JUST_ASSERT_EQUAL("<stdin>", r1->pp_context.filename);
  JUST_ASSERT_EQUAL(1, r1->pp_context.macros["__FILE__"].definition().size());
  JUST_ASSERT_EQUAL(
    "\"<stdin>\"",
    r1->pp_context.macros["__FILE__"].definition().front().get_value()
  );

  result_ptr r2 =
    precompile("__FILE__", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("\"<stdin>\"", r2->output);
  JUST_ASSERT_EQUAL("<stdin>", r2->pp_context.filename);
  JUST_ASSERT_EQUAL(1, r2->pp_context.macros["__FILE__"].definition().size());
  JUST_ASSERT_EQUAL(
    "\"<stdin>\"",
    r2->pp_context.macros["__FILE__"].definition().front().get_value()
  );

  result_ptr r3 =
    precompile(
      "#pragma wave macro_names",
      r2->pp_context,
      config::empty,
      ind,
      history
    );
  JUST_ASSERT(contains("__FILE__", split(r3->info)));

  result_ptr r4 =
    precompile(
      "#pragma wave macros",
      r3->pp_context,
      config::empty,
      ind,
      history
    );
  JUST_ASSERT(contains("__FILE__ \"<stdin>\"", split(r4->info)));
}

JUST_TEST_CASE(test_line_override)
{
  std::list<std::string> history;
  result_ptr r1 =
    precompile("#line 13 \"foo.cpp\"", context(), config::empty, ind, history);
  JUST_ASSERT_EQUAL("", r1->output);
  JUST_ASSERT_EQUAL(13, r1->pp_context.line);
  JUST_ASSERT_EQUAL(1, r1->pp_context.macros["__LINE__"].definition().size());
  JUST_ASSERT_EQUAL(
    "13",
    r1->pp_context.macros["__LINE__"].definition().front().get_value()
  );
  JUST_ASSERT_EQUAL("foo.cpp", r1->pp_context.filename);
  JUST_ASSERT_EQUAL(1, r1->pp_context.macros["__FILE__"].definition().size());
  JUST_ASSERT_EQUAL(
    "\"foo.cpp\"",
    r1->pp_context.macros["__FILE__"].definition().front().get_value()
  );

  result_ptr r2 =
    precompile("__LINE__", r1->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("13", r2->output);
  JUST_ASSERT_EQUAL(14, r2->pp_context.line);
  JUST_ASSERT_EQUAL(1, r2->pp_context.macros["__LINE__"].definition().size());
  JUST_ASSERT_EQUAL(
    "14",
    r2->pp_context.macros["__LINE__"].definition().front().get_value()
  );
  JUST_ASSERT_EQUAL("foo.cpp", r2->pp_context.filename);
  JUST_ASSERT_EQUAL(1, r2->pp_context.macros["__FILE__"].definition().size());
  JUST_ASSERT_EQUAL(
    "\"foo.cpp\"",
    r2->pp_context.macros["__FILE__"].definition().front().get_value()
  );

  result_ptr r3 =
    precompile("__FILE__", r2->pp_context, config::empty, ind, history);
  JUST_ASSERT_EQUAL("\"foo.cpp\"", r3->output);

  result_ptr r4 =
    precompile(
      "#pragma wave macro_names",
      r3->pp_context,
      config::empty,
      ind,
      history
    );
  JUST_ASSERT(contains("__FILE__", split(r4->info)));
  JUST_ASSERT(contains("__LINE__", split(r4->info)));

  result_ptr r5 =
    precompile(
      "#pragma wave macros",
      r4->pp_context,
      config::empty,
      ind,
      history
    );
  JUST_ASSERT(contains("__FILE__ \"foo.cpp\"", split(r5->info)));
  JUST_ASSERT(contains("__LINE__ 16", split(r5->info)));
}

JUST_TEST_CASE(test_input_processed_after_warning)
{
  std::list<std::string> history;
  const config cfg = config::empty;

  result_ptr r1 =
    precompile(
      "#define a 1\n"
      "#define a 2\n"
      "#define b 3\n",
      context(),
      cfg,
      ind,
      history
    );

  result_ptr r2 = precompile("b", r1->pp_context, cfg, ind, history);

  JUST_ASSERT_EQUAL("3", r2->output);
}

JUST_TEST_CASE(test_disable_warnings)
{
  std::list<std::string> history;
  config cfg = config::empty;
  cfg.enable_warnings = false;

  result_ptr r =
    precompile(
      "#define a 1\n"
      "#define a 2\n",
      context(),
      cfg,
      ind,
      history
    );

  JUST_ASSERT_EQUAL("", r->error);
}

