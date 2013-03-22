#ifndef PRESHELL_MACRO_MAP_HPP
#define PRESHELL_MACRO_MAP_HPP

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
#include <preshell/string.hpp>

#include <map>
#include <string>
#include <iosfwd>

namespace preshell
{
  typedef std::map<std::string, macro_definition> macro_map;

  std::ostream& operator<<(std::ostream& o_, const macro_map& m_);
}

#endif

