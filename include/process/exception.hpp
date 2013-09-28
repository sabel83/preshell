#ifndef PROCESS_EXCEPTION_HPP
#define PROCESS_EXCEPTION_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include <string>

namespace process
{
  struct exception : std::runtime_error
  {
    explicit exception(const std::string& reason_) :
      std::runtime_error(reason_)
    {}
  };
}

#endif

