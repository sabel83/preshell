#ifndef PROCESS_IMPL_INPUT_FILE_HPP
#define PROCESS_IMPL_INPUT_FILE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/utility.hpp>

#include <unistd.h>

#include <sstream>
#include <iostream>
#include <cassert>

namespace process
{
  namespace impl
  {
    class input_file : boost::noncopyable
    {
    public:
      explicit input_file(int fd_) : _fd(fd_), _eof(false) {}

      ~input_file()
      {
        if (_fd != -1)
        {
          close();
        }
      }

      ssize_t read(char* buf_, size_t count_)
      {
        const ssize_t len = ::read(_fd, buf_, count_);
        _eof = (len == 0);
        return len;
      }

      bool eof() const
      {
        return _eof;
      }

      std::string read()
      {
        std::ostringstream s;
        static const int buffsize = 1024;
        char buff[buffsize];
        while (!eof())
        {
          const ssize_t len = read(buff, buffsize);
          s.write(buff, len);
        }
        return s.str();
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
    private:
      int _fd;
      bool _eof;
    };
  }
}

#endif

