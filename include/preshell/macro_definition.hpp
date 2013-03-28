#ifndef PRESHELL_MACRO_DEFINITION_HPP
#define PRESHELL_MACRO_DEFINITION_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

