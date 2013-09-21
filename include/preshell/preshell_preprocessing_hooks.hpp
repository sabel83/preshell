#ifndef PRESHELL_PRESHELL_PREPROCESSING_HOOKS_HPP
#define PRESHELL_PRESHELL_PREPROCESSING_HOOKS_HPP

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

#include <preshell/if_state.hpp>
#include <preshell/preshell.hpp>

#include <boost/wave/cpp_context.hpp>

#include <list>
#include <iostream>
#include <set>

namespace preshell
{
  class preshell_preprocessing_hooks :
    public boost::wave::context_policies::default_preprocessing_hooks
  {
  public:
    preshell_preprocessing_hooks(
      std::list<if_state>& if_states_,
      std::ostream& info_channel_
    ) :
      _if_states(if_states_),
      _info_channel(info_channel_)
    {}

    template <class Context, class Container>
    bool interpret_pragma(
      const Context& ctx_,
      Container& pending_,
      const typename Context::token_type& option_,
      const Container& values_,
      const typename Context::token_type& pragma_token_
    )
    {
      if (option_.get_value() == "quit")
      {
        exit(0);
      }
      else if (option_.get_value() == "preshell_help")
      {
        _info_channel <<
          "Usage:\n"
          "\n"
          "  The preprocessor shell preprocesses the code you give it and\n"
          "  displays the preprocessed result. The shell provides command\n"
          "  history and tab-completion for preprocessor directives and macro\n"
          "  names. Some pragmas are also provided to inspect or update the \n"
          "  state of the preprocessor.\n"
          "\n"
          "  When the last character of a line is '\\', the shell does not\n"
          "  process it, but waits for the next line and processes the two\n"
          "  lines together.\n"
          "\n"
          "  Any preprocessing can be interrupted by pressing Ctrl-C, which\n"
          "  gives you the prompt back.\n"
          "\n"
          "  You can quit by pressing Ctrl-D.\n"
          "\n"
          "Available pragmas:\n"
          "\n"
          "  #pragma wave macros\n"
          "    Displays the list of defined macros.\n"
          "\n"
          "  #pragma wave macro_names\n"
          "    Displays the list of defined macro names.\n"
          "\n"
          "  #pragma wave preshell_help\n"
          "    Displays this help.\n"
          "\n"
          "  #pragma wave quit\n"
          "    Terminates the preprocessor.\n"
          ;
        return true;
      }
      else if (option_.get_value() == "macro_names")
      {
        const std::set<std::string> names = macro_names(ctx_);
        bool first = true;
        for (
          std::set<std::string>::const_iterator
            i = names.begin(),
            e = names.end();
          i != e;
          ++i
        )
        {
          if (first)
          {
            first = false;
          }
          else
          {
            _info_channel << std::endl;
          }
          _info_channel << *i;
        }
        return true;
      }
      else if (option_.get_value() == "macros")
      {
        const std::set<std::string> names = macro_names(ctx_);
        bool first = true;
        for (
          std::set<std::string>::const_iterator
            i = names.begin(),
            e = names.end();
          i != e;
          ++i
        )
        {
          if (first)
          {
            first = false;
          }
          else
          {
            _info_channel << std::endl;
          }

          if (*i == "__FILE__")
          {
            _info_channel << "__FILE__ \"";
            string_escape(ctx_.get_main_pos().get_file().c_str(),_info_channel);
            _info_channel << "\"";
          }
          else if (*i == "__LINE__")
          {
            _info_channel << "__LINE__ " << ctx_.get_main_pos().get_line();
          }
          else
          {
            bool is_function_style;
            bool is_predefined;
            typename Context::position_type position;
            std::vector<typename Context::token_type> parameters;
            typename Context::token_sequence_type definition;

            ctx_.get_macro_definition(
              *i,
              is_function_style,
              is_predefined,
              position,
              parameters,
              definition
            );

            _info_channel << *i;
            if (is_function_style)
            {
              _info_channel << "(";
              dump_tokens(parameters.begin(), parameters.end(), ", ");
              _info_channel << ")";
            }
            _info_channel << " ";
            dump_tokens(definition.begin(), definition.end(), "");
          }
        }
        return true;
      }

      return false;
    }

    template <class Context, class Token, class Container>
    bool evaluated_conditional_expression(
      const Context& ctx_,
      const Token& directive_,
      const Container& expression_,
      bool expression_value_
    )
    {
      if (directive_ == boost::wave::T_PP_ELIF && !_if_states.empty())
      {
        _if_states.back() = expression_value_ ? if1_then : if0_then;
      }
      else
      {
        _if_states.push_back(expression_value_ ? if1_then : if0_then);
      }

      return
        base::evaluated_conditional_expression(
          ctx_,
          directive_,
          expression_,
          expression_value_
        );
    }

    template <class Context, class Token>
    bool found_directive(const Context& ctx_, const Token& directive_)
    {
      if (
        directive_ == boost::wave::T_PP_ELIF
        && !_if_states.empty()
        && _if_states.back() == if1_then
      )
      {
        _if_states.back() = if1_else;
      }
      return base::found_directive(ctx_, directive_);
    }

    template <class Context, class Token>
    const Token& generated_token(const Context& ctx_, const Token& token_)
    {
      process_token(token_);
      return base::generated_token(ctx_, token_);
    }

    template <class Context, class Token>
    void skipped_token(const Context& ctx_, const Token& token_)
    {
      process_token(token_);
    }

  private:
    typedef boost::wave::context_policies::default_preprocessing_hooks base;

    std::list<if_state>& _if_states;
    std::ostream& _info_channel;

    template <class Token>
    void process_token(const Token& token_)
    {
      if (token_ == boost::wave::T_PP_ENDIF)
      {
        if (!_if_states.empty())
        {
          _if_states.pop_back();
        }
      }
      else if (token_ == boost::wave::T_PP_ELSE && !_if_states.empty())
      {
        switch (_if_states.back())
        {
        case if0_then: _if_states.back() = if0_else; break;
        case if1_then: _if_states.back() = if1_else; break;
        default:
          // ignore
          break;
        }
      }
    }

    template <class It>
    void dump_tokens(It begin_, It end_, const std::string& separator_)
    {
      bool first2 = true;
      for (; begin_ != end_; ++begin_)
      {
        if (first2)
        {
          first2 = false;
        }
        else
        {
          _info_channel << separator_;
        }
        _info_channel << begin_->get_value();
      }
    }

    template <class Context>
    static std::set<std::string> macro_names(const Context& ctx_)
    {
      std::set<std::string> names;
      for (
        typename Context::const_name_iterator
          i = ctx_.macro_names_begin(),
          e = ctx_.macro_names_end();
        i != e;
        ++i
      )
      {
        names.insert(i->c_str());
      }
      names.insert("__LINE__");
      names.insert("__FILE__");
      return names;
    }
  };
}

#endif

