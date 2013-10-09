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

#include <preshell/config.hpp>

using namespace preshell;

namespace
{
  const char* default_includes[] =
    {
      ""
      #include "default_include.hpp"
    };

  const char* default_sysincludes[] =
    {
      ""
      #include "default_sysinclude.hpp"
    };
}

config::config() :
  log_macro_definitions(false),
  enable_warnings(true)
{}

const config config::empty;

const config
  config::default_config(
    default_includes + 1,
    default_includes + sizeof(default_includes) / sizeof(const char*),
    default_sysincludes + 1,
    default_sysincludes + sizeof(default_sysincludes) / sizeof(const char*),
    #include "default_defines.hpp"
  );

