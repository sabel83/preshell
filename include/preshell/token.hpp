#ifndef PRESHELL_TOKEN_HPP
#define PRESHELL_TOKEN_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/file_position.hpp>

#include <boost/wave/cpplexer/cpp_lex_token.hpp>

namespace preshell
{
  typedef boost::wave::cpplexer::lex_token<file_position> token;
}

#endif

