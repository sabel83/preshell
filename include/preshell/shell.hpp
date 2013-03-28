#ifndef PRESHELL_SHELL_HPP
#define PRESHELL_SHELL_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <preshell/config.hpp>
#include <preshell/context.hpp>
#include <preshell/result.hpp>

#include <vector>
#include <string>

namespace preshell
{
  class shell
  {
  public:
    explicit shell(
      const preshell::config& config_,
      const std::vector<std::string>& macros_
    );
    virtual ~shell();
  
    virtual void display_normal(const std::string& s_) const = 0;
    virtual void display_info(const std::string& s_) const = 0;
    virtual void display_error(const std::string& s_) const = 0;
  
    void display_splash() const;
    void line_available(const std::string& s_);
    std::string prompt() const;
  
    const preshell::config& config() const;
    const preshell::context& context() const;
  
    void cancel_operation();
  
    static const std::vector<std::string> directives;
  private:
    preshell::config _config;
    preshell::result_ptr _context;
    std::string _buffer;
  };
}

#endif

