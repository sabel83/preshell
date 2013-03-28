#ifndef PRESHELL_CONTEXT_HPP
#define PRESHELL_CONTEXT_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/macro_map.hpp>
#include <preshell/if_state.hpp>
#include <preshell/config.hpp>

#include <list>
#include <vector>
#include <string>

namespace preshell
{
  struct context
  {
    macro_map macros;
    std::list<if_state> if_states;

    std::string filename;
    int line;

    context() :
      macros(),
      if_states(),
      filename("<stdin>"),
      line(1)
    {}
    context(const macro_map& macros_, const std::string& filename_, int line_) :
      macros(macros_),
      if_states(),
      filename(filename_),
      line(line_)
    {}

    bool tokens_skipped() const;

    static context initial(
      const config& config_,
      const std::vector<std::string>& macros_
    );
  };
}

#endif

