#ifndef PRESHELL_CONFIG_HPP
#define PRESHELL_CONFIG_HPP

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
#include <vector>

namespace preshell
{
  class config
  {
  public:
    std::vector<std::string> include_path;
    std::vector<std::string> sysinclude_path;
    std::string builtin_macro_definitions;
    bool log_macro_definitions;
    bool enable_warnings;

    static const config empty;
    static const config default_config;
  private:
    config();

    template <class InputIt>
    config(
      InputIt include_path_begin_,
      InputIt include_path_end_,
      InputIt sysinclude_path_begin_,
      InputIt sysinclude_path_end_,
      const char* builtin_macro_definitions_
    ) :
      include_path(include_path_begin_, include_path_end_),
      sysinclude_path(sysinclude_path_begin_, sysinclude_path_end_),
      builtin_macro_definitions(builtin_macro_definitions_),
      log_macro_definitions(false),
      enable_warnings(true)
    {}
  };
}

#endif

