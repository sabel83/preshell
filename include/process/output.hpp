#ifndef PROCESS_OUTPUT_HPP
#define PROCESS_OUTPUT_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>

namespace process
{
  class output
  {
  public:
    output(const std::string& stdout_, const std::string& stderr_) :
      _out(stdout_),
      _err(stderr_)
    {
    }

    const std::string& standard_output() const
    {
      return _out;
    }

    const std::string& standard_error() const
    {
      return _err;
    }
  private:
    std::string _out;
    std::string _err;
  };
}

#endif

