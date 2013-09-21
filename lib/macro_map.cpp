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

#include <preshell/macro_map.hpp>

#include <iostream>

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


