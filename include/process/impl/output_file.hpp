#ifndef PROCESS_IMPL_OUTPUT_FILE_HPP
#define PROCESS_IMPL_OUTPUT_FILE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/utility.hpp>

#include <unistd.h>
#include <fcntl.h>

#include <cassert>

namespace process
{
  namespace impl
  {
    class output_file : boost::noncopyable
    {
    public:
      explicit output_file(int fd_) : _fd(fd_) {}

      ~output_file()
      {
        if (_fd != -1)
        {
          close();
        }
      }

      ssize_t write(const char* buff_, size_t count_)
      {
        return ::write(_fd, buff_, count_);
      }

      ssize_t write(const std::string& s_)
      {
        return write(s_.c_str(), s_.length());
      }

      void close()
      {
        assert(_fd != -1);
        ::close(_fd);
        _fd = -1;
      }

      void use_as(int fd_)
      {
        dup2(_fd, fd_);
      }

      void close_on_exec()
      {
        fcntl(_fd, F_SETFD, fcntl(_fd, F_GETFD) | FD_CLOEXEC);
      }
    private:
      int _fd;
    };
  }
}

#endif

