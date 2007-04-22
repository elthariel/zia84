
#include <sys/mman.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "string.h"
#include "perlembed.hpp"

using namespace std;

/*
 * PerlEmbed class
 */

const char              *PerlEmbed::m_perl_api = 0;
int                     PerlEmbed::m_perl_api_fd = 0;

PerlEmbed::PerlEmbed()
  : my_perl(0),
    m_perl_initialized(false)
{
  my_perl = perl_alloc();
  PL_perl_destruct_level = 1;
}

PerlEmbed::~PerlEmbed()
{
  perl_destruct(my_perl);
  perl_free(my_perl);
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
  inject_api();
}
/*
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
*/

void                    PerlEmbed::get_perl_api()
{
  struct stat           api_stat;

  if (stat("zia_psp_api.pl", &api_stat) == -1)
    cerr << "Unable to get file stats : " << strerror(errno) << endl;
  else
    if ((m_perl_api_fd = open("zia_psp_api.pl", O_RDONLY)) != -1)
      m_perl_api = (const char *)mmap(0, api_stat.st_size,
                                      PROT_READ, MAP_PRIVATE,
                                      m_perl_api_fd, 0);
    else
      cerr << "Unable to mmap file : " << strerror(errno) << endl;

}

void                    PerlEmbed::inject_api()
{
  if (m_perl_api == 0)
    get_perl_api();
  if (m_perl_api != 0)
    perl_eval_pv(m_perl_api, TRUE);
};

bool                    PerlEmbed::eval_perl(std::string &a_perl_expr)
{
  perl_eval_pv(a_perl_expr.c_str(), TRUE);
  return (true);
}

std::string             &PerlEmbed::get_out()
{
  string                *out = new string;
  SV                    *perl_string;
  char                  *real_perl_string;
  STRLEN                real_perl_string_len;

  perl_string = perl_get_sv("main::__psp_out_buffer_magic42", FALSE);
  if (perl_string != NULL)
    {
      real_perl_string = SvPV(perl_string, real_perl_string_len);
      out->append(real_perl_string, real_perl_string_len);
    }
  return *out;
}

/*
 * Main
 */

int main(int ac, char **av)
{
  PerlEmbed p;
  string test("echo \"hello world !\\n\";");
  p.init_perl();
  cerr << "Perl init" << endl;
  p.eval_perl(test);
  cerr << "Command evaluated : " << endl << test << endl;
  cout << p.get_out() << endl;
}
