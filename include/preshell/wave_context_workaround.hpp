#ifndef PRESHELL_WAVE_CONTEXT_WORKAROUND_HPP
#define PRESHELL_WAVE_CONTEXT_WORKAROUND_HPP

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

#include <boost/wave/cpp_context.hpp>

namespace preshell
{
  template <
    class Iterator,
    class LexIterator,
    class InputPolicy,
    class ContextPolicy
  >
  class wave_context_workaround :
    public boost::wave::context<
      Iterator,
      LexIterator,
      InputPolicy,
      ContextPolicy,
      wave_context_workaround<Iterator, LexIterator, InputPolicy, ContextPolicy>
    >
  {
  private:
    typedef
      boost::wave::context<
        Iterator,
        LexIterator,
        InputPolicy,
        ContextPolicy,
        wave_context_workaround
      >
      base;
  public:
    wave_context_workaround(
      const Iterator& first_,
      const Iterator& last_, 
      const char* fname_,
      const ContextPolicy& ctx_policy_
    ) :
      base(first_, last_, fname_, ctx_policy_)
    {}

    const typename base::position_type& get_main_pos() const
    {
      return const_cast<wave_context_workaround*>(this)->base::get_main_pos();
    }
  };
}

#endif

