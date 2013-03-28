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

#include "editline_shell.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <vector>
#include <string>

int main(int argc_, char* argv_[])
{
  using boost::program_options::options_description;
  using boost::program_options::variables_map;
  using boost::program_options::store;
  using boost::program_options::notify;
  using boost::program_options::parse_command_line;
  using boost::program_options::value;

  using std::vector;
  using std::string;

  preshell::config config;
  vector<string> macros;
  vector<string> preprocess;

  options_description desc("Options");
  desc.add_options()
    ("help", "Display help")
    ("include,I", value(&config.include_path), "Additional include directory")
    ("sysinclude,S",
      value(&config.sysinclude_path),
      "Additional system include directory")
    ("define,D", value(&macros), "Define macro (format: name[=[value]])")
    ("preprocess,p", value(&preprocess), "Preprocess code at startup")
    ;

  variables_map vm;
  store(parse_command_line(argc_, argv_, desc), vm);
  notify(vm);

  if (vm.count("help"))
  {
    std::cout << desc << std::endl;
  }
  else
  {
    editline_shell shell(config, macros);

    shell.display_splash();

    for (
      std::vector<string>::const_iterator
        i = preprocess.begin(),
        e = preprocess.end();
      i != e;
      ++i
    )
    {
      shell.line_available(*i);
    }

    shell.run();
  }
}

