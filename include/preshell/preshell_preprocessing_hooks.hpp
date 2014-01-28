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
#include <preshell/indenter.hpp>
#include <preshell/util.hpp>

#include <boost/wave/cpp_context.hpp>

#include <boost/range/adaptors.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <boost/bind.hpp>

#include <list>
#include <iostream>
#include <set>
#include <map>
#include <sstream>

namespace preshell
{
  class preshell_preprocessing_hooks :
    public boost::wave::context_policies::default_preprocessing_hooks
  {
  private:
    void save_history(const std::string& filename_);
  public:
    preshell_preprocessing_hooks(
      std::list<if_state>& if_states_,
      indenter& info_indenter_,
      indenter& error_indenter_,
      const bool& log_macro_definitions_,
      const std::list<std::string>& history_,
      const bool& enable_save_history_,
      bool& replay_history_
    );

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
        display_help();
        return true;
      }
      else if (option_.get_value() == "macro_names")
      {
        display_macro_names(macro_names(ctx_));
        return true;
      }
      else if (option_.get_value() == "macros")
      {
        using boost::algorithm::join;
        using boost::adaptors::transformed;
        using boost::bind;

        _info_indenter
          .raw(
            join(
              macro_names(ctx_)
                | transformed(bind(macro_definition<Context>, &ctx_, _1)),
              "\n"
            )
          )
          .flush();
        return true;
      }
      else if (option_.get_value() == "save_history")
      {
        if (values_.empty())
        {
          _error_indenter
            .left_align("Usage: save_history(<filename>)")
            .flush();
        }
        else if (!_enable_save_history)
        {
          _error_indenter.left_align(
            "Saving history is disabled. You can enable it by the "
            "-H command line argument."
          ).flush();
        }
        else
        {
          save_history(token_list_to_string(values_.begin(), values_.end()));
        }
        return true;
      }
      else if (option_.get_value() == "replay_history")
      {
        _replay_history = true;
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

    template <class Context, class Token, class Parameters, class Definition>
    void defined_macro(
      const Context& ctx_,
      const Token& macro_name_,
      bool is_functionlike_,
      const Parameters& parameters_,
      const Definition& definition_,
      bool is_predefined_
    )
    {
      if (_log_macro_definitions)
      {
        std::ostringstream s;
        s
          << macro_name_.get_position().get_file()
          << ":" << macro_name_.get_position().get_line()
          << ":" << macro_name_.get_position().get_column()
          << ": #define " << macro_name_.get_value();
        _info_indenter.raw(s.str()).flush();
      }
    }

    template <class Context, class Token>
    void undefined_macro(const Context& ctx_, const Token& macro_name_)
    {
      if (_log_macro_definitions)
      {
        std::ostringstream s;
        s
          << macro_name_.get_position().get_file()
          << ":" << macro_name_.get_position().get_line()
          << ":" << macro_name_.get_position().get_column()
          << ": #undef " << macro_name_.get_value();
        _info_indenter.raw(s.str()).flush();
      }
    }

  private:
    typedef boost::wave::context_policies::default_preprocessing_hooks base;

    std::list<if_state>& _if_states;
    indenter& _info_indenter;
    indenter& _error_indenter;
    const bool& _log_macro_definitions;
    const std::list<std::string>& _history;
    const bool& _enable_save_history;
    bool& _replay_history;

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

    template <class Context>
    static std::string macro_definition(
      const Context* ctx_,
      const std::string& name_
    )
    {
      std::ostringstream os;

      if (name_ == "__FILE__")
      {
        os << "__FILE__ \"";
        string_escape(ctx_->get_main_pos().get_file().c_str(), os);
        os << "\"";
      }
      else if (name_ == "__LINE__")
      {
        os << "__LINE__ " << ctx_->get_main_pos().get_line();
      }
      else
      {
        using boost::algorithm::join;
        using boost::adaptors::transformed;
        using boost::bind;

        bool is_function_style;
        bool is_predefined;
        typename Context::position_type position;
        std::vector<typename Context::token_type> parameters;
        typename Context::token_sequence_type definition;

        ctx_->get_macro_definition(
          name_,
          is_function_style,
          is_predefined,
          position,
          parameters,
          definition
        );

        os << name_;
        if (is_function_style)
        {
          os
            << "("
            <<
              join(
                parameters
                  | transformed(bind(&Context::token_type::get_value, _1)),
                ", "
              )
            << ")";
        }
        os
          << " "
          <<
            join(
              definition
                | transformed(bind(&Context::token_type::get_value, _1)),
              ""
            );
      }

      return os.str();
    }

    template <class Context>
    static std::set<std::string> macro_names(const Context& ctx_)
    {
      using boost::bind;
      using boost::make_transform_iterator;

      std::set<std::string>
        names(
          make_transform_iterator(
            ctx_.macro_names_begin(),
            bind(&Context::string_type::c_str, _1)
          ),
          make_transform_iterator(
            ctx_.macro_names_end(),
            bind(&Context::string_type::c_str, _1)
          )
        );
      names.insert("__LINE__");
      names.insert("__FILE__");
      return names;
    }

    void display_help();
    void display_macro_names(const std::set<std::string>& names_);
  };
}

#endif

