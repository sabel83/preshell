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

#include <preshell/context.hpp>
#include <preshell/version.hpp>
#include <preshell/wave_context.hpp>
#include <preshell/file_position.hpp>
#include <preshell/token.hpp>

#include <boost/foreach.hpp>

#include <iostream>
#include <sstream>

using namespace preshell;

std::ostream& preshell::operator<<(std::ostream& o_, const macro_map& m_)
{
  o_ << "macro_map{" << std::endl;

  for (macro_map::const_iterator i = m_.begin(), e = m_.end(); i != e; ++i)
  {
    o_ << " {" << i->first << ", " << i->second << "}" << std::endl;
  }

  return o_ << "}";
}

bool context::tokens_skipped() const
{
  using std::list;

  for (
    list<if_state>::const_iterator i = if_states.begin(), e = if_states.end();
    i != e;
    ++i
  )
  {
    if (*i == if0_then || *i == if1_else)
    {
      return true;
    }
  }
  return false;
}

namespace
{
  file_position unknown_position()
  {
    return file_position("<Unknown>", 1, 1);
  }

  std::string to_string(int n_)
  {
    std::ostringstream s;
    s << n_;
    return s.str();
  }

  template <boost::wave::token_id Id>
  void add_macro(
    const std::string& name_,
    const std::string& value_,
    macro_map& m_
  )
  {
    using std::list;

    const file_position pos("<Unknown>", 1, 1);

    m_[name_] =
      macro_definition(
        unknown_position(),
        list<token>(1, token(Id, value_.c_str(), pos))
      );
  }

  template <int Value>
  void add_macro(const std::string& name_, macro_map& m_)
  {
    add_macro<boost::wave::T_INT>(name_, to_string(Value), m_);
  }

  void add_macro_with_empty_body(const std::string& name_, macro_map& m_)
  {
    m_[name_] = macro_definition(unknown_position(), std::list<token>());
  }
}

context context::initial(
  const config& config_,
  const std::vector<std::string>& macros_,
  indenter& indenter_
)
{
  std::list<if_state> if_states;

  wave_context_ptr wctx = create_context("", if_states, config_, indenter_);
  BOOST_FOREACH(const std::string& i, macros_)
  {
    wctx->add_macro_definition(i);
  }
  macro_map m = get_macros(*wctx);
  add_macro_with_empty_body("__PRESHELL", m);
  add_macro<PRESHELL_MAJOR>("__PRESHELL_MAJOR", m);
  add_macro<PRESHELL_MINOR>("__PRESHELL_MINOR", m);
  add_macro<PRESHELL_PATCH>("__PRESHELL_PATCH", m);
  add_macro<boost::wave::T_STRINGLIT>(
    "help",
    "\"Getting help: #pragma wave preshell_help\"",
    m
  );

  return context(m, "<stdin>", 1);
}

