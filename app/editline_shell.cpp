// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editline_shell.hpp"
#include "interrupt_handler_override.hpp"
#include "override_guard.hpp"
#include "console.hpp"

#include <editline/readline.h>

#include <boost/bind.hpp>

#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <vector>

#include <cassert>

namespace
{
  bool starts_with(const std::string& prefix_, const std::string& s_)
  {
    if (prefix_.length() <= s_.length())
    {
      for (int i = 0, e = prefix_.length(); i != e; ++i)
      {
        if (prefix_[i] != s_[i])
        {
          return false;
        }
      }
      return true;
    }
    else
    {
      return false;
    }
  }

  template <console::color Color>
  void display(const std::string& s_)
  {
    if (s_ != "")
    {
      console::text_color(Color);
      std::cout << s_;
      console::reset();
      std::cout << std::endl;
    }
  }
}

editline_shell* editline_shell::_instance = 0;

editline_shell::~editline_shell()
{
  assert(_instance);
  _instance = 0;
}

editline_shell::editline_shell(
  const preshell::config& config_,
  const std::vector<std::string>& macros_
) :
  shell(config_, macros_)
{
  assert(!_instance);
  _instance = this;
}

void editline_shell::run()
{
  using boost::bind;

  override_guard<char** (*)(const char*, int, int)>
    ovr1(rl_attempted_completion_function, tab_completion);
  interrupt_handler_override ovr2(bind(&editline_shell::cancel_operation,this));

  while (char* l = readline(prompt().c_str()))
  {
    add_history(l);
    line_available(l);
    free(l);
  }
}

char* editline_shell::tab_generator(const char* text_, int state_)
{
  using std::copy;

  assert(_instance);

  static std::list<std::string> values;
  static std::list<std::string>::const_iterator pos;

  if (!state_) // init
  {
    const std::string text(text_);
    values.clear();
    for (
      preshell::macro_map::const_iterator
        i = _instance->context().macros.begin(),
        e = _instance->context().macros.end();
      i != e;
      ++i
    )
    {
      const std::string s =
        i->second.has_parameters() ? i->first + "(" : i->first;
      if (starts_with(text_, s))
      {
        values.push_back(s);
      }
    }
    for (
      std::vector<std::string>::const_iterator
        i = directives.begin(),
        e = directives.end();
      i != e;
      ++i
    )
    {
      if (starts_with(text_, *i))
      {
        values.push_back(*i);
      }
    }
    pos = values.begin();
  }

  if (pos == values.end())
  {
    return 0;
  }
  else
  {
    char* s = new char[pos->length() + 1];
    copy(pos->begin(), pos->end(), s);
    s[pos->length()] = 0;
    ++pos;
    return s;
  }
}

char** editline_shell::tab_completion(const char* text_, int start_, int end_)
{
  return rl_completion_matches(const_cast<char*>(text_), &tab_generator);
}

void editline_shell::display_normal(const std::string& s_) const
{
  if (s_ != "")
  {
    std::cout << s_ << std::endl;
  }
}

void editline_shell::display_info(const std::string& s_) const
{
  display<console::bright_cyan>(s_);
}

void editline_shell::display_error(const std::string& s_) const
{
  display<console::bright_red>(s_);
}


