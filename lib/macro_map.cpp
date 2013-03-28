// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/macro_map.hpp>

#include <iostream>

using namespace preshell;

std::ostream& preshell::operator<<(std::ostream& o_, const macro_map& m_)
{
  o_ << "macro_map{" << std::endl;  

  for (macro_map::const_iterator i = m_.begin(), e = m_.end(); i != e; ++i)
  {
    o_ << " {" << i->first << ", " << i->second << "}" << std::endl;
  }

  return o_ << "}";
}


