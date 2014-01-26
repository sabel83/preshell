#ifndef PRESHELL_PRESHELL_HPP
#define PRESHELL_PRESHELL_HPP

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

#include <preshell/result.hpp>
#include <preshell/context.hpp>
#include <preshell/config.hpp>
#include <preshell/indenter.hpp>

#include <boost/wave/cpp_exceptions.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <list>

namespace preshell
{
  result_ptr precompile(
    const std::string& input_,
    const context& context_,
    const config& config_,
    indenter& indenter_,
    const std::list<std::string>& history_
  );
  void cancel();

  bool continuation_needed(const std::string& s_, const config& config_);

  // Utility
  void string_escape(const std::string& s_, std::ostream& out_);
  std::string string_escape(const std::string& s_);

  std::string remove_protected_macro_definitions(const std::string& s_);

  bool recoverable(const boost::wave::cpp_exception& e_);
  std::string format_error(const boost::wave::cpp_exception& e_);

  template <class It, class Cont>
  bool wave_iterators_equal(It a_, It b_, Cont* warnings_)
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
        if (warnings_)
        {
          warnings_->push_back(format_error(e));
        }
        return false;
      }
      else
      {
        throw;
      }
    }
  }

  template <class It, class Cont>
  std::string join(
    It begin_,
    It end_,
    Cont* warnings_,
    const bool& cancel_preprocessing_,
    bool suppress_empty_lines_
  )
  {
    using boost::wave::cpp_exception;

    std::ostringstream s;
    try
    {
      std::ostringstream current_line;
      It i = begin_;
      while (
        !cancel_preprocessing_ && !wave_iterators_equal(i, end_, warnings_)
      )
      {
        if (IS_CATEGORY(*i, boost::wave::EOLTokenType))
        {
          const std::string l = current_line.str();
          current_line.str(std::string());
          if (!(l.empty() && suppress_empty_lines_))
          {
            s << l << i->get_value();
          }
        }
        else if (*i == boost::wave::T_CPPCOMMENT)
        {
          s << current_line.str() << i->get_value();
          current_line.str(std::string());
        }
        else
        {
          current_line << i->get_value();
        }
        try
        {
          ++i;
        }
        catch (const cpp_exception& e)
        {
          if (recoverable(e))
          {
            if (warnings_)
            {
              warnings_->push_back(format_error(e));
            }
          }
          else
          {
            throw;
          }
        }
      }
      const std::string l = current_line.str();
      if (!l.empty())
      {
        s << l;
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

}

#endif

