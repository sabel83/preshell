#ifndef PRESHELL_PRESHELL_HPP
#define PRESHELL_PRESHELL_HPP

// Preshell - Interactive C/C++ preprocessor shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

