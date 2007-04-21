

#include <iostream>
//#include <cstdlib>
//#include <cstdio>
//#include "errno.h"
#include "string.h"
#include "perlembed.hpp"

using namespace std;

/*
 * PerlEmbed class
 */

PerlEmbed::PerlEmbed()
  : my_perl(0),
    m_perl_initialized(false),
    m_read_fd(0),
    m_write_fd(0)
{
  my_perl = perl_alloc();
  PL_perl_destruct_level = 1;
  create_pipe();
}

PerlEmbed::~PerlEmbed()
{
  perl_destruct(my_perl);
  perl_free(my_perl);
  close(m_read_fd);
  close(m_write_fd);
}

void                    PerlEmbed::init_perl()
{
  char          *perl_opts[] = { "", "-e", "0" };

  if (m_perl_initialized)
    perl_destruct(my_perl);

  perl_construct(my_perl);
  // Use the last null pointer if you want the perl script to exec in a different env.
  perl_parse(my_perl, NULL, 3, perl_opts, NULL);
  perl_run(my_perl);
  m_perl_initialized = true;
  inject_fd();
}

void                    PerlEmbed::create_pipe()
{
  int                   fd[2];

  //  if (pipe((int *)&fd) == -1)
  if (socketpair(PF_UNIX, SOCK_STREAM, 0,((int *)&fd)) == -1)
    {
      cerr << "Unable to create pipe, error ("
           << strerror(errno) << ")" << endl;
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

  snprintf((char *)&buf, 64, "%d", m_write_fd);
  cmd += "$stdout_fd_magic42 = ";
  cmd += (char *)&buf;
  cmd += "; open (TEST, \">&=$stdout_fd_magic42\"); select TEST;";
  //  cmd += "; TEST->autoflush(1); ";
  //  cmd += "; open (STDOUT, \">&=$stdout_fd_magic42\"); ";
  //  cmd += ";open (STDERR, \">&=$stdout_fd_magic42\");";
  cerr << "Fd injection command :" << endl << cmd << endl;
  perl_eval_pv(cmd.c_str(), TRUE);
  cerr << "Injected" << endl;
}

bool                    PerlEmbed::eval_perl(std::string &a_perl_expr)
{
  perl_eval_pv(a_perl_expr.c_str(), TRUE);
  return (true);
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
  /*  read_size = read(m_read_fd, (char *)&read_buf, 1);
  read_buf[1] = 0;
  cout << "read this : " << read_buf << endl;*/
  out->append((char *)&read_buf, read_size);
  return (*out);
}

/*
 * Main
 */

int main(int ac, char **av)
{
  PerlEmbed p;
  string test("print \"hello world !\\n\n\n\n\n\n\n\";");
  p.init_perl();
  cerr << "Perl init" << endl;
  for (int i = 0; i < 5000; i++)
    p.eval_perl(test);
  cerr << "Command evaluated" << endl;
  cout << p.get_out() << endl;
}
