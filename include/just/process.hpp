#ifndef JUST_PROCESS_HPP
#define JUST_PROCESS_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifdef _WIN32
#  include <windows.h>
#else
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <errno.h>
#  include <string.h>
#  include <fcntl.h>
#  include <unistd.h>
#endif

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

#ifdef _WIN32
      inline HANDLE make_inheritable_copy(HANDLE h_)
      {
        const HANDLE cp = GetCurrentProcess();
        HANDLE r = NULL;
        if (DuplicateHandle(cp, h_, cp, &r, 0, TRUE, DUPLICATE_SAME_ACCESS))
        {
          return r;
        }
        else
        {
          return NULL;
        }
      }
#endif
    
      /*
       * fd_t
       */
#ifdef _WIN32
        typedef HANDLE fd_t;
#else
        typedef int fd_t;
#endif
      
      inline fd_t invalid_fd()
      {
#ifdef _WIN32
        return NULL;
#else
        return -1;
#endif
      }

      /*
       * input_file
       */
      class input_file : noncopyable
      {
      public:
        explicit input_file(fd_t fd_) : _fd(fd_), _eof(false) {}

        ~input_file()
        {
          if (_fd != invalid_fd())
          {
            close();
          }
        }

#ifdef _WIN32
        DWORD read(char* buf_, size_t count_)
        {
          DWORD len = 0;
          if (ReadFile(_fd, buf_, count_, &len, NULL))
          {
            _eof = (len == 0);
            return len;
          }
          else
          {
            _eof = true;
            return 0;
          }
        }
#endif

#ifndef _WIN32
        ssize_t read(char* buf_, size_t count_)
        {
          const ssize_t len = ::read(_fd, buf_, count_);
          _eof = (len == 0);
          return len;
        }
#endif
        bool eof() const { return _eof; }

        std::string read()
        {
          std::ostringstream s;
          static const int buffsize = 1024;
          char buff[buffsize];
          while (!eof())
          {
            s.write(buff, read(buff, buffsize));
          }
          return s.str();
        }

        void close()
        {
          assert(_fd != invalid_fd());
#ifdef _WIN32
          CloseHandle(_fd);
#else
          ::close(_fd);
#endif
          _fd = invalid_fd();
        }

#ifndef _WIN32
        void use_as(int fd_)
        {
          dup2(_fd, fd_);
        }
#endif
        
#ifdef _WIN32
        fd_t copy_handle() const { return make_inheritable_copy(_fd); }
#endif
      private:
        fd_t _fd;
        bool _eof;
      };

      /*
       * output_file
       */
      class output_file : noncopyable
      {
      public:
        explicit output_file(fd_t fd_) : _fd(fd_) {}

        ~output_file()
        {
          if (_fd != invalid_fd())
          {
            close();
          }
        }

#ifdef _WIN32
        DWORD write(const char* buff_, size_t count_)
        {
          DWORD len = 0;
          if (WriteFile(_fd, buff_, count_, &len, NULL))
          {
            return len;
          }
          else
          {
            return 0;
          }
        }
#endif

#ifndef _WIN32
        ssize_t write(const char* buff_, size_t count_)
        {
          return ::write(_fd, buff_, count_);
        }
#endif

#ifdef _WIN32
        DWORD write(const std::string& s_)
#else
        ssize_t write(const std::string& s_)
#endif
        {
          return write(s_.c_str(), s_.length());
        }

        void close()
        {
          assert(_fd != invalid_fd());
#ifdef _WIN32
          CloseHandle(_fd);
#else
          ::close(_fd);
#endif
          _fd = invalid_fd();
        }

#ifndef _WIN32
        void use_as(int fd_)
        {
          dup2(_fd, fd_);
        }
#endif

#ifndef _WIN32
        void close_on_exec()
        {
          fcntl(_fd, F_SETFD, fcntl(_fd, F_GETFD) | FD_CLOEXEC);
        }
#endif

#ifdef _WIN32
        fd_t copy_handle() const { return make_inheritable_copy(_fd); }
#endif
      private:
        fd_t _fd;
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
#ifdef _WIN32
          fds()
          {
            CreatePipe(_fd, _fd + 1, NULL, 0);
          }
#endif

#ifndef _WIN32
          fds()
          {
            ::pipe(_fd);
          }
#endif

          fd_t read_fd() const { return _fd[0]; }
          fd_t write_fd() const { return _fd[1]; }
        private:
          fd_t _fd[2];
        };

        input_file input;
        output_file output;

        pipe(fds fds_ = fds()) :
          input(fds_.read_fd()),
          output(fds_.write_fd())
        {}
      };

      /*
       * join
       */
      template <class InputIt>
      std::string join(InputIt begin_, InputIt end_)
      {
        std::ostringstream s;
        bool first = true;
        for (InputIt i = begin_; i != end_; ++i)
        {
          if (first)
          {
            first = false;
          }
          else
          {
            s << ' ';
          }
          s << *i;
        }
        return s.str();
      }
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
#ifdef _WIN32
    template <class Seq>
    output run(const Seq& cmd_, const std::string& input_)
    {
      const std::string cmds = impl::join(cmd_.begin(), cmd_.end());
      std::vector<char> cmd(cmds.begin(), cmds.end());
      cmd.push_back(0);

      impl::pipe standard_input;
      impl::pipe standard_output;
      impl::pipe standard_error;

      STARTUPINFO si;
      si.cb = sizeof(si);
      si.lpReserved = NULL;
      si.lpDesktop = NULL;
      si.lpTitle = NULL;
      si.dwX = 0;
      si.dwY = 0;
      si.dwXSize = 0;
      si.dwYSize = 0;
      si.dwXCountChars = 0;
      si.dwYCountChars = 0;
      si.dwFillAttribute = 0;
      si.dwFlags = STARTF_USESTDHANDLES;
      si.wShowWindow = 0;
      si.cbReserved2 = 0;
      si.lpReserved2 = NULL;
      si.hStdInput = standard_input.input.copy_handle();
      si.hStdOutput = standard_output.output.copy_handle();
      si.hStdError = standard_error.output.copy_handle();

      PROCESS_INFORMATION pi;
      pi.hProcess = 0;
      pi.hThread = 0;
      pi.dwProcessId = 0;
      pi.dwThreadId = 0;

      if (
        CreateProcess(NULL, &cmd[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)
      )
      {
        CloseHandle(si.hStdInput);
        CloseHandle(si.hStdOutput);
        CloseHandle(si.hStdError);

        standard_input.input.close();
        standard_input.output.write(input_);
        standard_input.output.close();

        standard_output.output.close();
        standard_error.output.close();
  
        WaitForSingleObject(pi.hProcess, INFINITE);

        return
          output(standard_output.input.read(), standard_error.input.read());
      }
      else
      {
        const DWORD e = GetLastError();
        std::ostringstream s;
        s << "Error " << e;
        {
          char buff[1024];
          if (
            FormatMessage(
              FORMAT_MESSAGE_FROM_SYSTEM,
              NULL,
              e,
              0,
              buff,
              sizeof(buff),
              0
            )
          )
          {
            s << ": " << buff;
          }
        }
        throw exception(s.str());
      }
    }
#endif

#ifndef _WIN32
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
#endif
  }
}

#endif

