#ifndef PRESHELL_UTIL_HPP
#define PRESHELL_UTIL_HPP

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

#include <string>
#include <sstream>
#include <iostream>

namespace preshell
{
  template <int N>
  unsigned int always()
  {
    return N;
  }

  void throw_away(std::string);

  template <class Iterator>
  static std::string token_list_to_string(Iterator begin_, Iterator end_)
  {
    std::ostringstream s;
    for (Iterator i = begin_; i != end_; ++i)
    {
      s << i->get_value();
    }
    return s.str();
  }

  bool is_pragma_usage(const std::string& s_);
}

#endif

