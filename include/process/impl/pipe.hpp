#ifndef PROCESS_IMPL_PIPE_HPP
#define PROCESS_IMPL_PIPE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <process/impl/input_file.hpp>
#include <process/impl/output_file.hpp>

#include <unistd.h>

namespace process
{
  namespace impl
  {
    class pipe
    {
    public:
      class fds
      {
      public:
        fds()
        {
          ::pipe(_fd);
        }

        int read_fd() const
        {
          return _fd[0];
        }

        int write_fd() const
        {
          return _fd[1];
        }
      private:
        int _fd[2];
      };

      input_file input;
      output_file output;

      pipe(fds fds_ = fds()) :
        input(fds_.read_fd()),
        output(fds_.write_fd())
      {
      }
    };
  }
}

#endif

