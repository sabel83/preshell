#ifndef PRESHELL_CONTEXT_HPP
#define PRESHELL_CONTEXT_HPP

//  Preshell - Interactive C/C++ preprocessor shell
//  Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <preshell/macro_map.hpp>
#include <preshell/if_state.hpp>
#include <preshell/config.hpp>

#include <list>
#include <vector>
#include <string>

namespace preshell
{
  struct context
  {
    macro_map macros;
    std::list<if_state> if_states;

    std::string filename;
    int line;

    context() :
      macros(),
      if_states(),
      filename("<stdin>"),
      line(1)
    {}
    context(const macro_map& macros_, const std::string& filename_, int line_) :
      macros(macros_),
      if_states(),
      filename(filename_),
      line(line_)
    {}

    bool tokens_skipped() const;

    static context initial(
      const config& config_,
      const std::vector<std::string>& macros_
    );
  };
}

#endif

