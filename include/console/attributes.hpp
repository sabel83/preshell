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

#ifndef CONSOLE_ATTRIBUTES_HPP
#define CONSOLE_ATTRIBUTES_HPP

#include <boost/utility.hpp>
#include <boost/scoped_ptr.hpp>

#include <iostream>

namespace console
{
  enum color
  {
    black = 0,
    red = 1,
    green = 2,
    yellow = 3,
    blue = 4,
    magenta = 5,
    cyan = 6,
    gray = 7
  };
  
  #ifdef DEFINE_CONSOLE_MODE
    #error DEFINE_CONSOLE_MODE already defined
  #endif
  #define DEFINE_CONSOLE_MODE(name, code) \
    template <class Traits> \
    std::basic_ostream<char, Traits>& name( \
      std::basic_ostream<char, Traits>& o_ \
    ) \
    { \
      return o_ << "\033[" #code "m"; \
    }
  
  DEFINE_CONSOLE_MODE(normal, 0)
  DEFINE_CONSOLE_MODE(bold, 1)
  DEFINE_CONSOLE_MODE(underscore, 4)
  DEFINE_CONSOLE_MODE(blink, 5)
  DEFINE_CONSOLE_MODE(reverse_video, 7)
  DEFINE_CONSOLE_MODE(concealed, 8)
  
  #undef DEFINE_CONSOLE_MODE
  
  template <color Color, class Traits>
  std::basic_ostream<char, Traits>& foreground(
    std::basic_ostream<char, Traits>& o_
  )
  {
    return o_ << "\033[" << 30 + Color << "m";
  }
  
  template <color Color, class Traits>
  std::basic_ostream<char, Traits>& background(
    std::basic_ostream<char, Traits>& o_
  )
  {
    return o_ << "\033[" << 40 + Color << "m";
  }
  
  class reset_guard : boost::noncopyable
  {
  public:
    template <class Traits>
    explicit reset_guard(std::basic_ostream<char, Traits>& o_) :
      _o(new basic_ostream_impl<Traits>(o_))
    {}
  private:
    struct basic_ostream
    {
      virtual ~basic_ostream() {}
    };
  
    template <class Traits>
    class basic_ostream_impl : public basic_ostream
    {
    public:
      explicit basic_ostream_impl(std::basic_ostream<char, Traits>& o_) :
        _o(o_)
      {}
  
      virtual ~basic_ostream_impl()
      {
        _o << normal;
      }
    private:
      std::basic_ostream<char, Traits>& _o;
    };
    
    boost::scoped_ptr<basic_ostream> _o;
  };
}

#endif


