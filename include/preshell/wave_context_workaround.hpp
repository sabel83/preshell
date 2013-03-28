#ifndef PRESHELL_WAVE_CONTEXT_WORKAROUND_HPP
#define PRESHELL_WAVE_CONTEXT_WORKAROUND_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

