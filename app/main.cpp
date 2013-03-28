// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

