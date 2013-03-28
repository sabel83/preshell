// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/macro_definition.hpp>

#include <iostream>

using namespace preshell;

namespace
{
  template <class It>
  void show_tokens(It begin_, It end_, std::ostream& o_)
  {
    bool first = true;
    for (; begin_ != end_; ++begin_)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        o_ << ", ";
      }
      o_ << begin_->get_value();
    }
  }
}

std::ostream& preshell::operator<<(std::ostream& o_, const macro_definition& m_)
{
  if (m_.has_parameters())
  {
    o_ << "(";
    show_tokens(m_.parameters().begin(), m_.parameters().end(), o_);
    o_ << ") ";
  }

  show_tokens(m_.definition().begin(), m_.definition().end(), o_);
  
  return o_ << " @ " << m_.position();
}


