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

#include "temp_header.hpp"

#include <iostream>
#include <fstream>

temp_header::temp_header() :
  _path(absolute(boost::filesystem::unique_path()))
{}

temp_header::~temp_header()
{
  remove(_path);
}

void temp_header::create(const std::string& content_)
{
  std::ofstream f(filename().c_str());
  f << content_ << std::endl;
}

std::string temp_header::include_dir() const
{
  return _path.parent_path().native();
}

std::string temp_header::filename() const
{
  return _path.filename().native();
}

std::string temp_header::include_command() const
{
  return "#include <" + filename() + ">\n";
}


