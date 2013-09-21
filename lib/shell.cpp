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

#include <preshell/shell.hpp>
#include <preshell/preshell.hpp>
#include <preshell/version.hpp>

#include "indenter.hpp"

#include <boost/assign/list_of.hpp>

#include <boost/preprocessor/stringize.hpp>

using namespace preshell;

shell::shell(
  const preshell::config& config_,
  const std::vector<std::string>& macros_
) :
  _config(config_),
  _context(new preshell::result(preshell::context::initial(config_, macros_))),
  _buffer()
{}

shell::~shell() {}

const std::vector<std::string> shell::directives =
  boost::assign::list_of<std::string>
    ("#define")
    ("#elif")
    ("#else")
    ("#endif")
    ("#error")
    ("#if")
    ("#ifdef")
    ("#ifndef")
    ("#include")
    ("#line")
    ("#pragma")
    ("#undef")

    ("_Pragma(\"") // Supported by Wave
  ;

void shell::cancel_operation()
{
  preshell::cancel();
}

const preshell::config& shell::config() const
{
  return _config;
}

const preshell::context& shell::context() const
{
  return _context->pp_context;
}

void shell::display_splash() const
{
  display_normal(
    indenter(80, " * ")
      .raw("/*")
      .left_align("Preprocessor shell " + preshell::version())
      .empty_line()
      .left_align("Based on")
      .left_align(
        preshell::wave_version(),
        " *              ",
        " *   Boost.Wave "
      )
      .left_align(
        preshell::readline_version(),
        " *              ",
        " *   Readline   "
      )
      .empty_line()
      .left_align("Getting help: #pragma wave preshell_help")
      .raw(" */")
      .str()
  );
}

void shell::line_available(const std::string& s_)
{
  _buffer += (_buffer == "" ? "" : "\n") + s_;

  if (!preshell::continuation_needed(_buffer, _config))
  {
    _context = preshell::precompile(_buffer, _context->pp_context, _config);
    _buffer = "";

    if (_context->output != "")
    {
      display_normal(_context->output);
    }
    if (_context->info != "")
    {
      display_info(_context->info);
    }
    if (_context->error != "")
    {
      display_error(_context->error);
    }
  }
}

std::string shell::prompt() const
{
  return
    _buffer == "" ?
      (_context->pp_context.tokens_skipped() ? "#if 0 ...> " : "> ") :
      ">>>> ";
}


