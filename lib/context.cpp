// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/context.hpp>
#include <preshell/version.hpp>
#include <preshell/wave_context.hpp>
#include <preshell/file_position.hpp>
#include <preshell/token.hpp>

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
      macro_definition(pos, list<token>(1, token(Id, value_.c_str(), pos)));
  }

  template <int Value>
  void add_macro(const std::string& name_, macro_map& m_)
  {
    add_macro<boost::wave::T_INT>(name_, to_string(Value), m_);
  }
}

context context::initial(
  const config& config_,
  const std::vector<std::string>& macros_
)
{
  std::list<if_state> if_states;
  std::ostringstream s;

  wave_context_ptr wctx = create_context("", if_states, s, config_);
  for (
    std::vector<std::string>::const_iterator
      i = macros_.begin(),
      e = macros_.end();
    i != e;
    ++i
  )
  {
    wctx->add_macro_definition(*i);
  }
  macro_map m = get_macros(*wctx);
  add_macro<PRESHELL_MAJOR>("__PRESHELL_MAJOR__", m);
  add_macro<PRESHELL_MINOR>("__PRESHELL_MINOR__", m);
  add_macro<boost::wave::T_STRINGLIT>(
    "help",
    "\"Getting help: #pragma wave preshell_help\"",
    m
  );

  return context(m, "<stdin>", 1);
}

