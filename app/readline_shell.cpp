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

#include "readline_shell.hpp"

#include <console/attributes.hpp>

#include <readline/readline.h>
#include <readline/history.h>

#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <vector>

#include <cassert>

#include <signal.h>

namespace
{
  class readline_tab_completion_override : boost::noncopyable
  {
  private:
    typedef char** (*callback)(const char*, int, int);
  public:
    readline_tab_completion_override(callback cb_) :
      _old(rl_attempted_completion_function)
    {
      rl_attempted_completion_function = cb_;
    }

    ~readline_tab_completion_override()
    {
      rl_attempted_completion_function = _old;
    }
  private:
    char** (*_old)(const char*, int, int);
  };

  template <int SigNum>
  class signal_handler_override : boost::noncopyable
  {
  public:
    signal_handler_override(sighandler_t handler_) :
      _old(signal(SigNum, handler_))
    {}

    ~signal_handler_override()
    {
      signal(SigNum, _old);
    }
  private:
    sighandler_t _old;
  };

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
      std::cout
        << console::foreground<Color> << s_ << console::normal
        << std::endl;
    }
  }
}

readline_shell* readline_shell::_instance = 0;

readline_shell::~readline_shell()
{
  assert(_instance);
  _instance = 0;
}

readline_shell::readline_shell(
  const preshell::config& config_,
  const std::vector<std::string>& macros_
) :
  shell(config_, macros_)
{
  assert(!_instance);
  _instance = this;
}

void readline_shell::sig_int_handler(int)
{
  assert(_instance);
  _instance->cancel_operation();
}

void readline_shell::run()
{
  readline_tab_completion_override ovr1(tab_completion);
  signal_handler_override<SIGINT> ovr2(sig_int_handler);

  for (;;)
  {
    if (char* l = readline(prompt().c_str()))
    {
      add_history(l);
      line_available(l);
      free(l);
    }
    else
    {
      break;
    }
  }
}

char* readline_shell::tab_generator(const char* text_, int state_)
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

char** readline_shell::tab_completion(const char* text_, int start_, int end_)
{
  return rl_completion_matches(const_cast<char*>(text_), &tab_generator);
}

void readline_shell::display_normal(const std::string& s_) const
{
  if (s_ != "")
  {
    std::cout << s_ << std::endl;
  }
}

void readline_shell::display_info(const std::string& s_) const
{
  display<console::cyan>(s_);
}

void readline_shell::display_error(const std::string& s_) const
{
  display<console::red>(s_);
}

