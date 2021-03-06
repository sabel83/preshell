#ifndef PRESHELL_TEST_TEST_SHELL_HPP
#define PRESHELL_TEST_TEST_SHELL_HPP

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

#include <preshell/shell.hpp>

#include <vector>
#include <string>
#include <sstream>

class test_shell : public preshell::shell
{
public:
  test_shell(const preshell::config& config_);
  test_shell(std::vector<std::string>& history_);

  virtual void add_history(const std::string& s_);
  
  virtual void display_normal(const std::string& s_) const;
  virtual void display_info(const std::string& s_) const;
  virtual void display_error(const std::string& s_) const;

  virtual unsigned int width() const;

  std::string normal() const;
  std::string info() const;
  std::string error() const;
private:
  mutable std::ostringstream _normal;
  mutable std::ostringstream _info;
  mutable std::ostringstream _error;

  std::vector<std::string>* _history; // no ownership
};

#endif

