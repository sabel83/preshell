#ifndef PRESHELL_MACRO_DEFINITION_HPP
#define PRESHELL_MACRO_DEFINITION_HPP

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

#include <preshell/token.hpp>
#include <preshell/file_position.hpp>

#include <boost/operators.hpp>

#include <vector>
#include <list>
#include <iosfwd>

namespace preshell
{
  class macro_definition : boost::equality_comparable<macro_definition>
  {
  public:
    macro_definition() {}

    macro_definition(
      const file_position& position_,
      const std::list<token>& definition_
    ) :
      _position(position_),
      _has_parameters(false),
      _parameters(),
      _definition(definition_)
    {}

    macro_definition(
      const file_position& position_,
      const std::vector<token>& parameters_,
      const std::list<token>& definition_
    ) :
      _position(position_),
      _has_parameters(true),
      _parameters(parameters_),
      _definition(definition_)
    {}

    template <class String, class Context>
    macro_definition(const String& name_, const Context& context_)
    {
      bool ignore;
      typename Context::token_sequence_type def;

      context_.get_macro_definition(
        name_,
        _has_parameters,
        ignore,
        _position,
        _parameters,
        def
      );
      _definition = std::list<token>(def.begin(), def.end());
    }

    const file_position& position() const { return _position; }
    bool has_parameters() const { return _has_parameters; }
    const std::vector<token>& parameters() const { return _parameters; }
    const std::list<token>& definition() const { return _definition; }

    bool operator==(const macro_definition& m_) const
    {
      return
        _position == m_._position &&
        _has_parameters == m_._has_parameters &&
        _parameters == m_._parameters &&
        _definition == m_._definition;
    }
  private:
    file_position _position;
    bool _has_parameters;
    std::vector<token> _parameters;
    std::list<token> _definition;
  };

  std::ostream& operator<<(std::ostream& o_, const macro_definition& m_);
}

#endif

