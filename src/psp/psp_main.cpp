//
// psp_main.cpp for psp in /home/lessyv/taf/zia/src/psp
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Tue Apr 24 15:02:20 2007   lessyv
// last update   Wed Apr 25 06:59:54 2007   loic
//

#include <iostream>

#include "psp_main.hpp"
#include "bloc.hpp"
#include "perlembed.hpp"

Psp::Psp()
{
  BlocParser = new Bloc();
}

Psp::~Psp()
{
  delete BlocParser;
}

bool		Psp::init_psp(const string &real_page, char **env)
{
  if (real_page.length() <= 0)
    {
      cerr << "[PSP] Input buffer: No data" << endl;
      return false;
    }
  BlocParser->init(real_page);
  PerlInterpreter.init_perl(env);
  cout << "[PSP] OK to proceed" << endl;
  return true;
}

void		Psp::apply_psp()
{
  BlocParser->find_code_to_replace();
  PerlInterpreter.eval_perl(BlocParser->get_last_bloc_code());
  BlocParser->apply_code(BlocParser->get_last_bloc_code());
}

int	Psp::replace_all_psp()
{
  if (BlocParser == NULL)
    {
      cerr << "[PSP] Not initialized" << endl;
      return -1;
    }
  while (!BlocParser->parsing_ended())
    {
      replace_one_psp_bloc();
    }
  return (BlocParser->get_blocs_found());
}

//AND int	Psp::replace_one_psp_bloc()
int	Psp::replace_one_psp_bloc()
{
  if (BlocParser == NULL)
    {
      cerr << "[PSP] Not initialized" << endl;
      return -1;
    }
  apply_psp();
  printf("[PSP] source code %i : %s\n",
	 BlocParser->get_blocs_found(),
	 BlocParser->get_last_bloc_code().c_str());
  if (BlocParser->parsing_ended())
    {
      cout << "[PSPPSP] End encountered" << endl;
      return (-1);
    }
  return (BlocParser->get_blocs_found());
}

string	&Psp::get_computed_page()
{
  return (BlocParser->get_computed_page());
}

bool	Psp::psp_done()
{
  return (BlocParser->parsing_ended());
}
