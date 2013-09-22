#ifndef PRESHELL_WAVE_CONTEXT_HPP
#define PRESHELL_WAVE_CONTEXT_HPP

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

#include <preshell/wave_context_workaround.hpp>
#include <preshell/preshell_preprocessing_hooks.hpp>
#include <preshell/if_state.hpp>
#include <preshell/macro_map.hpp>
#include <preshell/config.hpp>
#include <preshell/indenter.hpp>

#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <boost/shared_ptr.hpp>

#include <list>
#include <string>

namespace preshell
{
  typedef
    wave_context_workaround<
      std::string::const_iterator,
      boost::wave::cpplexer::lex_iterator<token>,
      boost::wave::iteration_context_policies::load_file_to_string,
      preshell_preprocessing_hooks
    >
    wave_context;

  typedef boost::shared_ptr<wave_context> wave_context_ptr;

  wave_context_ptr create_context(
    const std::string& input_,
    std::list<if_state>& if_states_,
    const config& config_,
    indenter& indenter_
  );

  macro_map get_macros(const wave_context& context_);
}

#endif

