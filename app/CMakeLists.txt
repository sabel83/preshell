# Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

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

