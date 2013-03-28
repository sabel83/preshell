// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/wave_context.hpp>
#include <preshell/preshell.hpp>

using namespace preshell;

namespace
{
  template <class It>
  macro_map get_macros_impl(
    It begin_,
    It end_,
    const wave_context& context_
  )
  {
    macro_map result;
    for (; begin_ != end_; ++begin_)
    {
      result.insert(
        std::make_pair(
          std::string(begin_->c_str()),
          macro_definition(*begin_, context_)
        )
      );
    }
    // Wave doesn't seem to export it
    if (result.find("__LINE__") == result.end())
    {
      std::ostringstream s;
      s << context_.get_main_pos().get_line();
      const file_position p("<Unknown>", 1, 1);
      result.insert(
        std::make_pair(
          std::string("__LINE__"),
          macro_definition(
            p,
            std::list<token>(
              1,
              token(boost::wave::T_INTLIT, s.str().c_str(), p)
            )
          )
        )
      );
    }
    if (result.find("__FILE__") == result.end())
    {
      const file_position p("<Unknown>", 1, 1);
      std::ostringstream s;
      s << "\"";
      string_escape(context_.get_main_pos().get_file().c_str(), s);
      s << "\"";
      result.insert(
        std::make_pair(
          std::string("__FILE__"),
          macro_definition(
            p,
            std::list<token>(
              1,
              token(boost::wave::T_STRINGLIT, s.str().c_str(), p)
            )
          )
        )
      );
    }
    return result;
  }
}

wave_context_ptr preshell::create_context(
  const std::string& input_,
  std::list<if_state>& if_states_,
  std::ostream& info_channel_,
  const config& config_
)
{
  wave_context_ptr
    context(
      new wave_context(
        input_.begin(),
        input_.end(),
        "<Unknown>",
        preshell_preprocessing_hooks(if_states_, info_channel_)
      )
    );
  context->set_language(
    boost::wave::language_support(
      boost::wave::support_cpp | boost::wave::support_c99
    )
  );

  typedef std::vector<std::string>::const_iterator it;

  for (
    it i = config_.include_path.begin(), e = config_.include_path.end();
    i != e;
    ++i
  )
  {
    context->add_include_path(i->c_str());
  }

  for (
    it i = config_.sysinclude_path.begin(), e = config_.sysinclude_path.end();
    i != e;
    ++i
  )
  {
    context->add_sysinclude_path(i->c_str());
  }

  return context;
}

macro_map preshell::get_macros(const wave_context& context_)
{
  return get_macros_impl(
    context_.macro_names_begin(),
    context_.macro_names_end(),
    context_
  );
}


