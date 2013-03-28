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

aux_source_directory(. SOURCES)
add_executable(preshell ${SOURCES})

# Editline
find_package(Editline)
include_directories(${Editline_INCLUDE_DIR})

# Preshell
target_link_libraries(preshell preshell_lib)

# Wave
target_link_libraries(preshell
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_THREAD_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${Boost_WAVE_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

# Program_options
target_link_libraries(preshell ${Boost_PROGRAM_OPTIONS_LIBRARIES})

# Editline
target_link_libraries(preshell ${Editline_LIBRARY})

install(TARGETS preshell DESTINATION bin)

