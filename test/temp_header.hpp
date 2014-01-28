#ifndef PRESHELL_TEST_TEMP_HEADER_HPP
#define PRESHELL_TEST_TEMP_HEADER_HPP

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

#include <boost/utility.hpp>

#include <boost/filesystem.hpp>

#include <string>

class temp_header : boost::noncopyable
{
public:
  temp_header();
  ~temp_header();

  void create(const std::string& content_);

  std::string include_dir() const;
  std::string include_command() const;
private:
  boost::filesystem::path _path;

  std::string filename() const;
};

#endif

