#!/bin/bash
#
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

if \
  [ -e /etc/redhat-release ] || [ -e /etc/fedora-release ] \
  || [ -e /etc/SuSE-release ]
then
  PACKAGE_FORMAT=RPM
else
  PACKAGE_FORMAT=DEB
fi

tools/gcc_default_path > lib/core/default_sysinclude.hpp \
  && tools/gcc_builtin_macros > lib/core/default_defines.hpp \
  \
  && mkdir -p bin \
  && cd bin \
  && cmake .. \
  && make \
  && make test \
  \
  && cpack -G $PACKAGE_FORMAT \
  \
  && echo done

