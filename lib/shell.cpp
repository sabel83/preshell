// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/shell.hpp>
#include <preshell/preshell.hpp>
#include <preshell/version.hpp>

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
    "/*\n"
    " *  Preprocessor shell "
      BOOST_PP_STRINGIZE(PRESHELL_MAJOR)
      "." BOOST_PP_STRINGIZE(PRESHELL_MINOR) "\n"
    " *\n"
    " *  Based on Boost.Wave " + preshell::wave_version() + "\n"
    " *\n"
    " *  Getting help: #pragma wave preshell_help\n"
    " */\n"
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


