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

#include <preshell/preshell_preprocessing_hooks.hpp>
#include <preshell/version.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <fstream>

using namespace preshell;

preshell_preprocessing_hooks::preshell_preprocessing_hooks(
  std::list<if_state>& if_states_,
  indenter& info_indenter_,
  indenter& error_indenter_,
  const bool& log_macro_definitions_,
  const std::list<std::string>& history_,
  const bool& enable_save_history_,
  bool& replay_history_
) :
  _if_states(if_states_),
  _info_indenter(info_indenter_),
  _error_indenter(error_indenter_),
  _log_macro_definitions(log_macro_definitions_),
  _history(history_),
  _enable_save_history(enable_save_history_),
  _replay_history(replay_history_)
{}

void preshell_preprocessing_hooks::display_help()
{
  _info_indenter
    .left_align("Usage:")
    .empty_line()
    .left_align(
      "The preprocessor shell preprocesses the code you give it and displays"
      " the preprocessed result. The shell provides command history and"
      " tab-completion for preprocessor directives and macro names. Some"
      " pragmas are also provided to inspect or update the state of the"
      " preprocessor."
    )
    .empty_line()
    .left_align(
      "When the last character of a line is '\\', the shell does not process "
      " it, but waits for the next line and processes the two lines together."
    )
    .empty_line()
    .left_align(
      "Any preprocessing can be interrupted by pressing Ctrl-C, which gives"
      " you the prompt back."
    )
    .empty_line()
    .left_align("You can quit by pressing Ctrl-D.")
    .empty_line()
    .left_align("Available pragmas:")
    .empty_line()
    .left_align("#pragma wave macros", "  ")
    .left_align("Displays the list of defined macros.", "    ")
    .empty_line()
    .left_align("#pragma wave macro_names", "  ")
    .left_align("Displays the list of defined macro names.", "    ")
    .empty_line()
    .left_align("#pragma wave preshell_help", "  ")
    .left_align("Displays this help.", "    ")
    .empty_line()
    .left_align("#pragma wave save_history(<path>)", "  ")
    .left_align("Saves the shell's history into a file.", "    ");
  if (!_enable_save_history)
  {
    _info_indenter
      .left_align(
        "This feature is currently disabled. You can enable it by using the "
        "-H command-line option.",
        "    "
      );
  }
  _info_indenter
    .empty_line()
    .left_align("#pragma wave quit", "  ")
    .left_align("Terminates the preprocessor.", "    ")
    .flush()
  ;
}

void preshell_preprocessing_hooks::display_macro_names(
  const std::set<std::string>& names_
)
{
  _info_indenter
    .raw(boost::algorithm::join(names_, "\n"))
    .flush();
}

void preshell_preprocessing_hooks::save_history(
  const std::string& filename_
)
{
  std::ofstream f(filename_.c_str());
  if (f)
  {
    f
      << "// Created using Preprocessor shell " << version() << std::endl
      << std::endl;
    BOOST_FOREACH(const std::string& l, _history)
    {
      f << l << std::endl;
      if (!f)
      {
        _error_indenter
          .left_align("Error writing to file " + filename_)
          .flush();
        return;
      }
    }
  }
  else
  {
    _error_indenter
      .left_align("Error opening file " + filename_)
      .flush();
  }
}


