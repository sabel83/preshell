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

#include <preshell/macro_definition.hpp>

#include <iostream>

using namespace preshell;

namespace
{
  template <class It>
  void show_tokens(It begin_, It end_, std::ostream& o_)
  {
    bool first = true;
    for (; begin_ != end_; ++begin_)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        o_ << ", ";
      }
      o_ << begin_->get_value();
    }
  }
}

std::ostream& preshell::operator<<(std::ostream& o_, const macro_definition& m_)
{
  if (m_.has_parameters())
  {
    o_ << "(";
    show_tokens(m_.parameters().begin(), m_.parameters().end(), o_);
    o_ << ") ";
  }

  show_tokens(m_.definition().begin(), m_.definition().end(), o_);
  
  return o_ << " @ " << m_.position();
}


