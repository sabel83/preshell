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

#include <process/process.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <boost/foreach.hpp>

#include <vector>
#include <string>
#include <stdexcept>

namespace
{
  void append(
    std::vector<std::string>& v1_,
    const std::vector<std::string>& v2_
  )
  {
    v1_.insert(v1_.end(), v2_.begin(), v2_.end());
  }

  std::string run(const std::vector<std::string>& cmd_)
  {
    const process::output o = process::run(cmd_, "");
    return o.standard_output() + o.standard_error();
  }

  std::string run(const std::string& cmd_, const std::string& arg1_)
  {
    std::vector<std::string> cmd;
    cmd.push_back(cmd_);
    cmd.push_back(arg1_);
    return run(cmd);
  }

  std::vector<std::string> get_gcc_default_sysinclude(
    const std::string& gcc_path_
  )
  {
    using boost::algorithm::trim_right_copy;
    using boost::algorithm::trim_left_copy;
    using boost::algorithm::split;
    using boost::is_any_of;
    using boost::starts_with;

    using std::vector;
    using std::string;

    const string s =
      run(trim_right_copy(run(gcc_path_, "-print-prog-name=cc1plus")), "-v");

    vector<string> lines;
    split(lines, s, is_any_of("\n"));

    vector<string> result;
    bool in_sysinclude = false;
    BOOST_FOREACH(const string& line, lines)
    {
      if (in_sysinclude)
      {
        if (starts_with(line, " "))
        {
          result.push_back(trim_left_copy(line));
        }
        else
        {
          return result;
        }
      }
      else if (starts_with(line, "#include <"))
      {
        in_sysinclude = true;
      }
    }

    return result;
  }
}

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

  vector<string> include_path;
  vector<string> sysinclude_path;
  vector<string> macros;
  vector<string> preprocess;
  string gcc;

  options_description desc("Options");
  desc.add_options()
    ("help", "Display help")
    ("sysinclude,I",
      value(&sysinclude_path),
      "Additional system include directory")
    ("include,i", value(&include_path), "Additional include directory")
    ("define,D", value(&macros), "Define macro (format: name[=[value]])")
    ("preprocess,p", value(&preprocess), "Preprocess code at startup")
    ("gcc,g", value(&gcc), "Use the default sysinclude path of that gcc binary")
    ;

  try
  {
    variables_map vm;
    store(parse_command_line(argc_, argv_, desc), vm);
    notify(vm);

    if (vm.count("help"))
    {
      std::cout << desc << std::endl;
    }
    else
    {
      preshell::config config;
      if (!gcc.empty())
      {
        config.sysinclude_path = get_gcc_default_sysinclude(gcc);
      }
      append(config.sysinclude_path, sysinclude_path);
      append(config.include_path, include_path);
      readline_shell shell(config, macros);

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
  catch (const boost::program_options::error& e_)
  {
    std::cerr << e_.what() << "\n\n" << desc << std::endl;
    return 1;
  }
  catch (const std::exception& e_)
  {
    std::cerr << e_.what() << std::endl;
    return 1;
  }
}

