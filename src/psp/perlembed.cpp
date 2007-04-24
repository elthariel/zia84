
#include <sys/mman.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "string.h"
#include "perlembed.hpp"
#include "psp_launcher.hpp"
#include <fstream>

using namespace std;

/*
 * PerlEmbed class
 */

//TODO
// 1 - psp_api.cpp like
//       module ez !@# compliant
//
// 2 - envs & session ;
//       cooking ... for us
//       document root and other for module
//
// 3 - objet Psp
//       generalisant l'acces a
//         perlembed
//         + psp parser
//

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

void                    PerlEmbed::init_perl(char **a_env)
{
  char          *perl_opts[] = { "", "-e", "0", "-mCGI" };

  if (m_perl_initialized)
    perl_destruct(my_perl);

  perl_construct(my_perl);
  // Use the last null pointer if you want the perl script to exec in a different env.
  perl_parse(my_perl, NULL, 4, perl_opts, a_env);
  perl_run(my_perl);
  m_perl_initialized = true;
  inject_api();
}

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
      perl_eval_pv("&__psp_clean_out_buffer", TRUE);
    }
  return *out;
}

/*
 * Main
 */

int		main(int ac, char **av)
{
  PerlEmbed	PerlInterpreter;
  string	*proceed;
  Bloc		*BlocParser = new Bloc();
  fstream	f_page;
  char		buff[98765];

  f_page.open("test_page.html", ios::in);
  if (f_page == false)
    cerr << "Unable to open src/psp/test_page.html" << endl;
  f_page.read(buff, 98765);

  proceed = new string(buff);
  BlocParser->s_out = proceed;

  PerlInterpreter.init_perl();
  //cerr << "Perl init" << endl;

  //cerr << "Parser init" << endl;
  //cout << "page: " << *(BlocParser->s_out) << endl;
  while (!BlocParser->parsing_ended())
    {
      // FIXME loop trop de fois ... mais protected dans apply_code
      BlocParser->find_code_to_replace();
      cout << BlocParser->get_bloc_code() << endl;
      PerlInterpreter.eval_perl(BlocParser->get_bloc_code());
      //cerr << "Command evaluated : " << endl << BlocParser->get_bloc_code() << endl;
      BlocParser->apply_code(PerlInterpreter.get_out());
    }

  cout << "[PSP]page modif:\n" << *(BlocParser->s_out) << endl;
  printf("[PSP] %i blocs found and maybe properly replaced ...\n", BlocParser->blocs_count);
  f_page.close();
  delete BlocParser->s_out; // FIXME a faire dans le moteur
  delete BlocParser;
  //XX delete PerlInterpreter.get_out() *out
}
