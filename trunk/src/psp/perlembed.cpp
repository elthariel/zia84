

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "errno.h"
#include "string.h"
#include "perlembed.hpp"

using namespace std;

/*
 * PerlEmbed class
 */

PerlEmbed::PerlEmbed()
  : m_perl(0),
    m_perl_initialized(false),
    m_read_fd(0),
    m_write_fd(0)
{
  m_perl = perl_alloc();

  create_pipe();
}

PerlEmbed::~PerlEmbed()
{
  perl_destruct(m_perl);
  perl_free(m_perl);
}

void                    PerlEmbed::init_perl()
{
  char          *perl_opts[] = { "", "-e", "0" };

  if (m_perl_initialized)
    perl_destruct(m_perl);

  perl_construct(m_perl);
  // Use the last null pointer if you want the perl script to exec in a different env.
  perl_parse(m_perl, NULL, 3, perl_opts, NULL);
  perl_run(m_perl);
  m_perl_initialized = true;
  inject_fd();
}

void                    PerlEmbed::create_pipe()
{
  int                   fd[2];

  if (pipe((int *)&fd) == -1)
    {
      cerr << "Unable to create pipe, error (" << strerror(errno) << ")" << endl;
      //FIXME Generate an error
    }
  else
    {
      m_read_fd = fd[0];
      m_write_fd = fd[1];
    }
}

void                    PerlEmbed::inject_fd()
{
  string                cmd;
  char                  buf[64];

  snprintf((char *)&buf, 32, "%d", m_write_fd);
  cmd += "$stdout_fd_magic42 = ";
  cmd += (char *)&buf;
  cmd += "; open (STDOUT, \"<&=$stdout_fd_magic42\"); ";
  cmd += "open (STDERR, \"<&=$stdout_fd_magic42\");";
  perl_eval_pv(cmd.c_str(), TRUE);
}

bool                    PerlEmbed::eval_perl(std::string &a_perl_expr)
{
  perl_eval_pv(a_perl_expr.c_str(), TRUE);
}

std::string             &PerlEmbed::get_out()
{
  char                  read_buf[1024];
  string                *out = new string();
  int                   read_size;

  while((read_size = read(m_read_fd, (char *)&read_buf, 1024)) == 1024)
    {
      out->append((char *)&read_buf, read_size);
    }
  out->append((char *)&read_buf, read_size);
  return (*out);
}

/*
 * Main
 */
