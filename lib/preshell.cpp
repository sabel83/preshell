// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/preshell.hpp>

#include <preshell/wave_context.hpp>
#include <preshell/preshell_preprocessing_hooks.hpp>

#include <boost/wave/cpp_context.hpp>
#include <boost/wave/cpp_exceptions.hpp>

#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using namespace preshell;

namespace
{
  bool cancel_preprocessing = false;

  template <class It>
  std::string join(It begin_, It end_)
  {
    std::ostringstream s;
    try
    {
      for (It i = begin_; !cancel_preprocessing && i != end_; ++i)
      {
        s << i->get_value();
      }
    }
    catch (const boost::wave::cpp_exception& e)
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

  template <char C>
  std::string remove(const std::string& s_)
  {
    std::ostringstream s;
    for (std::string::const_iterator i = s_.begin(), e = s_.end(); i != e; ++i)
    {
      if (*i != C)
      {
        s << *i;
      }
    }
    return s.str();
  }
}

result_ptr preshell::precompile(
  const std::string& input_,
  const context& context_,
  const config& config_
)
{
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

    wave_context_ptr context =
      create_context(input, r->pp_context.if_states, info, config_);
    delete_all_macros(*context);
    add_macros(context_.macros.begin(), context_.macros.end(), *context);

    const std::string output = join(context->begin(), context->end());

    if (cancel_preprocessing)
    {
      r->error = "ERROR: cancelled";
    }
    else
    {
      // The last character of the output is the extra new line
      r->output =
        output == "" ? "" : std::string(output.begin(), output.end() - 1);
      r->info = info.str();
      r->pp_context.macros = get_macros(*context);
      r->pp_context.filename = context->get_main_pos().get_file().c_str();
      r->pp_context.line = context->get_main_pos().get_line();
    }
  }
  catch (const boost::wave::cpp_exception& e)
  {
    std::ostringstream s;
    s << "ERROR: Wave exception: " << e.description();
    r->error = s.str();
    r->pp_context = context_;
  }
  catch (const std::exception& e)
  {
    std::ostringstream s;
    s << "ERROR: " << e.what();
    r->error = s.str();
    r->pp_context = context_;
  }
  catch (...)
  {
    std::ostringstream s;
    s << "ERROR: an unknown exception was thrown";
    r->error = s.str();
    r->pp_context = context_;
  }

  return r;
}

void preshell::cancel()
{
  cancel_preprocessing = true;
}

std::string preshell::wave_version()
{
  std::list<if_state> if_states;
  std::ostringstream s;
  return
    remove<'"'>(
      create_context("", if_states, s, config())->get_version_string()
    );
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


