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

#include "test_shell.hpp"

#include <iostream>

using namespace preshell;

test_shell::test_shell(const preshell::config& config_) :
  shell(config_, std::vector<std::string>())
{
  display_initialisation_diagnostic();
}

void test_shell::display_normal(const std::string& s_) const
{
  _normal << s_;
}

void test_shell::display_info(const std::string& s_) const
{
  _info << s_;
}

void test_shell::display_error(const std::string& s_) const
{
  _error << s_;
}

unsigned int test_shell::width() const
{
  return 80;
}

std::string test_shell::normal() const
{
  return _normal.str();
}

std::string test_shell::info() const
{
  return _info.str();
}

std::string test_shell::error() const
{
  return _error.str();
}

