#ifndef PRESHELL_MACRO_MAP_HPP
#define PRESHELL_MACRO_MAP_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/macro_definition.hpp>
#include <preshell/string.hpp>

#include <map>
#include <string>
#include <iosfwd>

namespace preshell
{
  typedef std::map<std::string, macro_definition> macro_map;

  std::ostream& operator<<(std::ostream& o_, const macro_map& m_);
}

#endif

