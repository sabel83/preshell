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

#include <preshell/preshell.hpp>

#include <preshell/wave_context.hpp>
#include <preshell/preshell_preprocessing_hooks.hpp>

#include <boost/wave/cpp_context.hpp>
#include <boost/wave/cpp_exceptions.hpp>

#include <boost/algorithm/string/join.hpp>

#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using namespace preshell;

namespace
{
  bool cancel_preprocessing = false;

  bool recoverable(const boost::wave::cpp_exception& e_)
  {
    return
      e_.is_recoverable()
      && e_.get_errorcode()
        != boost::wave::preprocess_exception::missing_matching_endif;
  }

  std::string format_error(const boost::wave::cpp_exception& e_)
  {
    std::ostringstream s;
    s
      << e_.file_name() << ":" << e_.line_no() << ":" << e_.column_no() << ": "
      << e_.description();
    return s.str();
  }

  template <class It, class Cont>
  bool wave_iterators_equal(It a_, It b_, Cont& warnings_)
  {
    using boost::wave::cpp_exception;
    try
    {
      return a_ == b_;
    }
    catch (const cpp_exception& e)
    {
      if (recoverable(e))
      {
        warnings_.push_back(format_error(e));
        return false;
      }
      else
      {
        throw;
      }
    }
  }

  template <class It, class Cont>
  std::string join(It begin_, It end_, Cont& warnings_)
  {
    using boost::wave::cpp_exception;

    std::ostringstream s;
    try
    {
      It i = begin_;
      while (!cancel_preprocessing && !wave_iterators_equal(i, end_, warnings_))
      {
        s << i->get_value();
        try
        {
          ++i;
        }
        catch (const cpp_exception& e)
        {
          if (recoverable(e))
          {
            warnings_.push_back(format_error(e));
          }
          else
          {
            throw;
          }
        }
      }
    }
    catch (const cpp_exception& e)
    {
      if (
        e.get_errorcode()
        != boost::wave::preprocess_exception::missing_matching_endif
      )
      {
        throw;
      }
    }
    return s.str();
  }

  template <class String>
  bool protected_name(const String& s)
  {
    return
      s == "__DATE__"
      || s == "__STDC__"
      || s == "__TIME__"
      || s == "__LINE__"
      || s == "__FILE__"
      || s == "__cplusplus";
  }

  template <class It, class Context>
  void delete_macros(It begin_, It end_, Context& context_)
  {
    for (; begin_ != end_; ++begin_)
    {
      if (!protected_name(*begin_))
      {
        context_.remove_macro_definition(*begin_, true);
      }
    }
  }

  template <class Context>
  void delete_all_macros(Context& context_)
  {
    const std::vector<token::string_type>
      v(context_.macro_names_begin(), context_.macro_names_end());
    delete_macros(v.begin(), v.end(), context_);
  }

  template <class It, class Context>
  void add_macros(It begin_, It end_, Context& context_)
  {
    for (; begin_ != end_; ++begin_)
    {
      if (!protected_name(begin_->first))
      {
        std::vector<token> parameters(begin_->second.parameters());
        typename Context::token_sequence_type
          definition(
            begin_->second.definition().begin(),
            begin_->second.definition().end()
          );

        const token::string_type name(begin_->first.c_str());

        context_.add_macro_definition(
          name,
          begin_->second.position(),
          begin_->second.has_parameters(),
          parameters,
          definition,
          false
        );
      }
    }
  }

  std::string to_directive(if_state s_)
  {
    switch (s_)
    {
    case if0_then: return "#if 0\n";
    case if0_else: return "#if 0\n#else\n";
    case if1_then: return "#if 1\n";
    case if1_else: return "#if 1\n#else\n";
    }
    return ""; // Keep gcc happy
  }

  template <class It>
  std::string prepare_input(
    const std::string& input_,
    It begin_,
    It end_,
    const std::string& file_,
    int line_
  )
  {
    std::ostringstream s;
    for (; begin_ != end_; ++begin_)
    {
      s << to_directive(*begin_);
    }
    s << "#line " << line_ << " \"";
    string_escape(file_, s);
    s << "\"\n" << input_ << '\n';
    return s.str();
  }
}

namespace
{
  void display_on_stream(std::ostream* out_, std::string s_)
  {
    *out_ << s_;
  }
}

result_ptr preshell::precompile(
  const std::string& input_,
  const context& context_,
  const config& config_,
  indenter& indenter_
)
{
  using boost::bind;

  cancel_preprocessing = false;
  result_ptr r = result_ptr(new result());

  try
  {
    const std::string
      input(
        prepare_input(
          input_,
          context_.if_states.begin(),
          context_.if_states.end(),
          context_.filename,
          context_.line
        )
      );

    std::ostringstream info;
    indenter info_indenter(indenter_, bind(display_on_stream, &info, _1));

    bool log_macro_definitions = false;

    wave_context_ptr context =
      create_context(
        input,
        r->pp_context.if_states,
        config_,
        info_indenter,
        log_macro_definitions
      );
    delete_all_macros(*context);
    add_macros(context_.macros.begin(), context_.macros.end(), *context);

    log_macro_definitions = config_.log_macro_definitions;

    std::list<std::string> warning_list;
    const std::string
      output = join(context->begin(), context->end(), warning_list);
    const std::string warnings = boost::algorithm::join(warning_list, "\n");

    if (cancel_preprocessing)
    {
      r->error =
        (warnings.empty() ? "" : warnings + "\n") + "Operation cancelled";
    }
    else
    {
      // The last character of the output is the extra new line
      r->output =
        output.empty() ? "" : std::string(output.begin(), output.end() - 1);
      r->info = info.str();
      r->error = warnings;
      r->pp_context.macros = get_macros(*context);
      r->pp_context.filename = context->get_main_pos().get_file().c_str();
      r->pp_context.line = context->get_main_pos().get_line();
    }
  }
  catch (const boost::wave::cpp_exception& e)
  {
    r->error = format_error(e);
    r->pp_context = context_;
  }
  catch (const std::exception& e)
  {
    r->error = e.what();
    r->pp_context = context_;
  }
  catch (...)
  {
    r->error = "An unknown exception was thrown";
    r->pp_context = context_;
  }

  return r;
}

void preshell::cancel()
{
  cancel_preprocessing = true;
}

bool preshell::continuation_needed(const std::string& s_, const config& config_)
{
  return s_ != "" && s_[s_.length() - 1] == '\\';
}

void preshell::string_escape(const std::string& s_, std::ostream& out_)
{
  for (std::string::const_iterator i = s_.begin(), e = s_.end(); i != e; ++i)
  {
    switch (*i)
    {
    case '\\':
    case '"':
    case '\'':
      out_ << '\\';
      break;
    default:
      break;
    }
    out_ << *i;
  }
}

std::string preshell::string_escape(const std::string& s_)
{
  std::ostringstream s;
  string_escape(s_, s);
  return s.str();
}

std::string preshell::remove_protected_macro_definitions(const std::string& s_)
{
  std::istringstream is(s_);
  std::ostringstream os;
  for (std::string line; std::getline(is, line);)
  {
    std::istringstream il(line);
    std::string s;
    if (
      !(
        il >> s && (s == "#define" || (s == "#" && il >> s && s == "define"))
        && il >> s && protected_name(s)
      )
    )
    {
      os << line << std::endl;
    }
  }
  return os.str();
}

