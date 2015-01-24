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

#include <preshell/util.hpp>
#include <preshell/token.hpp>

#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <algorithm>

using namespace preshell;

void preshell::throw_away(std::string)
{}

bool preshell::is_pragma_usage(const std::string& s_)
{
  using boost::wave::cpplexer::lex_iterator;
  using boost::wave::language_support;

  const file_position pos("<std::string>");
  const language_support lng = boost::wave::support_cpp;
  const lex_iterator<token> e;
  const std::string s(s_ + "\n");
  return
    std::find(
      lex_iterator<token>(s.begin(), s.end(), pos, lng),
      e,
      boost::wave::T_PP_PRAGMA
    ) != e;
}


