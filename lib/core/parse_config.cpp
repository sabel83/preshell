// Preshell - Interactive C/C++ preprocessor shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <preshell/parse_config.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <boost/assign/list_of.hpp>

#include <just/process.hpp>

#include <boost/foreach.hpp>

using namespace preshell;

namespace
{
  void append(
    std::vector<std::string>& v1_,
    const std::vector<std::string>& v2_
  )
  {
    v1_.insert(v1_.end(), v2_.begin(), v2_.end());
  }

  std::string run(const std::string& cmd_)
  {
    const std::vector<std::string> cmd = boost::assign::list_of<std::string>
      ("/bin/sh")("-c")(cmd_);
    const just::process::output o = just::process::run(cmd, "");
    return o.standard_output() + o.standard_error();
  }

  std::vector<std::string> get_gcc_default_sysinclude(
    const std::string& gcc_path_
  )
  {
    using boost::algorithm::trim_left_copy;
    using boost::algorithm::split;
    using boost::is_any_of;
    using boost::starts_with;

    using std::vector;
    using std::string;

    const string s = run(gcc_path_ + " -v -xc++ -");

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

  std::string get_gcc_builtin_macros(const std::string& gcc_path_)
  {
    return run(gcc_path_ + "-dM -E -");
  }
}

parse_config_result preshell::parse_config(
  config& cfg_,
  std::vector<std::string>& macros_,
  std::vector<std::string>& preprocess_,

  int argc_,
  const char* argv_[],

  std::ostream* out_, 
  std::ostream* err_
)
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
  string gcc;

  options_description desc("Options");
  desc.add_options()
    ("help", "Display help")
    ("sysinclude,I",
      value(&sysinclude_path),
      "Additional system include directory")
    ("include,i", value(&include_path), "Additional include directory")
    ("define,D", value(&macros_), "Define macro (format: name[=[value]])")
    ("preprocess,p", value(&preprocess_), "Preprocess code at startup")
    ("logdef,l", "Log macro definitions and undefinitions")
    ("no-warning,w", "Disable warning messages")
    ("suppress-empty-lines,e", "Suppress empty lines in output")
    ("gcc,g",
      value(&gcc),
      "Use the default sysinclude path of that gcc or clang binary")
    ("save_history,H", "Enable #pragma wave save_history(...)")
    ;

  try
  {
    variables_map vm;
    store(parse_command_line(argc_, argv_, desc), vm);
    notify(vm);

    if (vm.count("help"))
    {
      if (*out_)
      {
        *out_ << desc << std::endl;
      }
      return exit_without_error;
    }
    else
    {
      if (!gcc.empty())
      {
        cfg_.sysinclude_path = get_gcc_default_sysinclude(gcc);
        cfg_.builtin_macro_definitions = get_gcc_builtin_macros(gcc);
      }
      append(cfg_.sysinclude_path, sysinclude_path);
      append(cfg_.include_path, include_path);
      cfg_.log_macro_definitions = vm.count("logdef") || vm.count("l");
      cfg_.enable_warnings = !(vm.count("no-warning") || vm.count("w"));
      cfg_.suppress_empty_lines_in_output =
        vm.count("suppress-empty-lines") || vm.count("e");
      cfg_.enable_save_history = vm.count("save_history") || vm.count("H");

      return run_shell;
    }
  }
  catch (const boost::program_options::error& e_)
  {
    if (err_)
    {
      *err_ << e_.what() << "\n\n" << desc << std::endl;
    }
    return exit_with_error;
  }
  catch (const std::exception& e_)
  {
    if (err_)
    {
      *err_ << e_.what() << std::endl;
    }
    return exit_with_error;
  }
}


