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

#include <preshell/parse_config.hpp>

#include <boost/foreach.hpp>

int main(int argc_, const char* argv_[])
{
  preshell::config config = preshell::config::default_config;
  std::vector<std::string> macros;
  std::vector<std::string> preprocess;

  const preshell::parse_config_result r =
    preshell::parse_config(
      config,
      macros,
      preprocess,

      argc_, argv_,
      &std::cout, &std::cerr
    );

  if (r == preshell::run_shell)
  {
    readline_shell shell(config, macros);

    shell.display_splash();

    BOOST_FOREACH(const std::string& s, preprocess)
    {
      shell.line_available(s);
    }

    shell.run();
  }

  return r == preshell::exit_with_error ? 1 : 0;
}

