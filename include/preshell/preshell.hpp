#ifndef PRESHELL_PRESHELL_HPP
#define PRESHELL_PRESHELL_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/result.hpp>
#include <preshell/context.hpp>
#include <preshell/config.hpp>

#include <string>
#include <iosfwd>

namespace preshell
{
  result_ptr precompile(
    const std::string& input_,
    const context& context_,
    const config& config_
  );
  void cancel();

  std::string wave_version();

  bool continuation_needed(const std::string& s_, const config& config_);

  // Utility
  void string_escape(const std::string& s_, std::ostream& out_);
}

#endif

