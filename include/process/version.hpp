#ifndef PROCESS_VERSION_HPP
#define PROCESS_VERSION_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu) 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifdef PROCESS_MAJOR_VERSION
  #error PROCESS_MAJOR_VERSION already defined
#endif

#ifdef PROCESS_MINOR_VERSION
  #error PROCESS_MINOR_VERSION already defined
#endif

#ifdef PROCESS_PATCH_VERSION
  #error PROCESS_PATCH_VERSION already defined
#endif

#define PROCESS_MAJOR_VERSION 0
#define PROCESS_MINOR_VERSION 1
#define PROCESS_PATCH_VERSION 0

#endif

