#ifndef JUST_PROCESS_HPP
#define JUST_PROCESS_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iostream>

namespace just
{
  namespace process
  {
    namespace impl
    {
      /*
       * noncopyable
       */
      class noncopyable
      {
      public:
        noncopyable() {}

      private:
        noncopyable(const noncopyable&);
        noncopyable& operator=(const noncopyable&);
      };

      /*
       * util
       */
      inline const char* c_str(const std::string& s_)
      {
        return s_.c_str();
      }
    
      /*
       * input_file
       */
      class input_file : noncopyable
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

        bool eof() const { return _eof; }

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

      /*
       * output_file
       */
      class output_file : noncopyable
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

      /*
       * pipe
       */
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

          int read_fd() const { return _fd[0]; }
          int write_fd() const { return _fd[1]; }
        private:
          int _fd[2];
        };

        input_file input;
        output_file output;

        pipe(fds fds_ = fds()) :
          input(fds_.read_fd()),
          output(fds_.write_fd())
        {}
      };
    }

    /*
     * exception
     */
    struct exception : std::runtime_error
    {
      explicit exception(const std::string& reason_) :
        std::runtime_error(reason_)
      {}
    };

    /*
     * output
     */
    class output
    {
    public:
      output(const std::string& stdout_, const std::string& stderr_) :
        _out(stdout_),
        _err(stderr_)
      {}

      const std::string& standard_output() const { return _out; }
      const std::string& standard_error() const { return _err; }
    private:
      std::string _out;
      std::string _err;
    };

    /*
     * run
     */
    template <class Seq>
    output run(const Seq& cmd_, const std::string& input_)
    {
      using std::vector;
      using std::transform;
      using std::string;
  
      assert(!cmd_.empty());
  
      vector<const char*> cmd(cmd_.size() + 1, 0);
      transform(cmd_.begin(), cmd_.end(), cmd.begin(), impl::c_str);
  
      impl::pipe standard_input;
      impl::pipe standard_output;
      impl::pipe standard_error;
      impl::pipe error_reporting;
  
      const pid_t pid = fork();
      switch (pid)
      {
      case -1:
        return output("", "");
      case 0: // in child
        standard_input.output.close();
        standard_output.input.close();
        standard_error.input.close();
        error_reporting.input.close();
        error_reporting.output.close_on_exec();
  
        standard_input.input.use_as(STDIN_FILENO);
        standard_output.output.use_as(STDOUT_FILENO);
        standard_error.output.use_as(STDERR_FILENO);
  
        execv(cmd[0], const_cast<char*const*>(&cmd[0]));
        {
          const int err = errno;
          std::ostringstream s;
          s << "Error running";
          for (
            typename Seq::const_iterator i = cmd_.begin(), e = cmd_.end();
            i != e;
            ++i
          )
          {
            s << ' ' << *i;
          }
          s << ": " << strerror(err);
          error_reporting.output.write(s.str());
        }
        _exit(0);
      default: // in parent
        standard_input.input.close();
        standard_input.output.write(input_);
        standard_input.output.close();
  
        standard_output.output.close();
        standard_error.output.close();
  
        error_reporting.output.close();
  
        int status;
        waitpid(pid, &status, 0);
  
        const std::string err = error_reporting.input.read();
        if (err.empty())
        {
          return
            output(standard_output.input.read(), standard_error.input.read());
        }
        else
        {
          throw exception(err);
        }
      }
    }
  }
}

#endif

