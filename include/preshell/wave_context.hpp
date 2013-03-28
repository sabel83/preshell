#ifndef PRESHELL_WAVE_CONTEXT_HPP
#define PRESHELL_WAVE_CONTEXT_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/wave_context_workaround.hpp>
#include <preshell/preshell_preprocessing_hooks.hpp>
#include <preshell/if_state.hpp>
#include <preshell/macro_map.hpp>
#include <preshell/config.hpp>

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
    std::ostream& info_channel_,
    const config& config_
  );

  macro_map get_macros(const wave_context& context_);
}

#endif

