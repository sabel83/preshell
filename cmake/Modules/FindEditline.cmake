# Preshell - Interactive C/C++ preprocessor shell
# Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# This module will define the following:
#   Editline_FOUND
#   Editline_INCLUDE_DIR
#   Editline_LIBRARY

find_path(Editline_INCLUDE_DIR editline/readline.h)
find_library(Editline_LIBRARY NAMES edit)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set Editline_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  Editline DEFAULT_MSG Editline_LIBRARY Editline_INCLUDE_DIR
)

mark_as_advanced(Editline_INCLUDE_DIR, Editline_LIBRARY)

