// Preshell - Interactive C/C++ preprocessor shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <preshell/parse_config.hpp>

#include <just/test.hpp>

namespace
{
  template <int Len>
  preshell::parse_config_result parse_config(
    preshell::config& cfg_,
    const char* (&args_)[Len]
  )
  {
    std::vector<std::string> ignore;
    return preshell::parse_config(cfg_, ignore, ignore, Len, args_);
  }
}

JUST_TEST_CASE(test_save_history_is_disabled_by_default)
{
  const char* args[] = {"preshell"};

  preshell::config cfg = preshell::config::empty;
  parse_config(cfg, args);

  JUST_ASSERT(!preshell::config::empty.enable_save_history);
  JUST_ASSERT(!preshell::config::default_config.enable_save_history);
  JUST_ASSERT(!cfg.enable_save_history);
}

JUST_TEST_CASE(test_enable_save_history)
{
  const char* args[] = {"preshell", "-H"};

  preshell::config cfg = preshell::config::empty;
  parse_config(cfg, args);

  JUST_ASSERT(cfg.enable_save_history);
}


