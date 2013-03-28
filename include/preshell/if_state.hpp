#ifndef PRESHELL_IF_STATE_HPP
#define PRESHELL_IF_STATE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace preshell
{
  enum if_state
  {
    if0_then = 0, // 00
    if0_else = 1, // 01
    if1_then = 2, // 10
    if1_else = 3  // 11
  };
}

#endif

