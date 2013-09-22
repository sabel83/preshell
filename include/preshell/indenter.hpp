#ifndef PRESHELL_INDENTER_HPP
#define PRESHELL_INDENTER_HPP

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

#include <boost/function.hpp>

#include <string>
#include <sstream>

namespace preshell
{
  class indenter
  {
  public:
    typedef boost::function<unsigned int()> get_width_t;
    typedef boost::function<void(std::string)> output_t;

    indenter(const get_width_t& get_width_, const output_t& out_);
    ~indenter();
  
    indenter& left_align(
      const std::string& s_,
      const std::string& line_prefix_,
      const std::string& first_line_prefix_
    );
  
    indenter& left_align(
      const std::string& s_,
      const std::string& line_prefix_
    );
  
    indenter& raw(const std::string& s_);
  
    indenter& left_align(const std::string& s_);
  
    indenter& empty_line();

    // return type is void since this should be the final call of a chain of
    // method calls
    void flush();
  private:
    get_width_t _get_width;
    output_t _out;
    std::ostringstream _buff;
  };
}

#endif

