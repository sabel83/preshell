#ifndef PRESHELL_RESULT_HPP
#define PRESHELL_RESULT_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/context.hpp>

#include <boost/shared_ptr.hpp>

#include <string>

namespace preshell
{
  struct result
  {
    std::string output;
    std::string error;
    std::string info;

    context pp_context;

    result() {}
    result(const context& ctx_) :
      output(),
      error(),
      pp_context(ctx_)
    {}
  };

  typedef boost::shared_ptr<result> result_ptr;
}

#endif

