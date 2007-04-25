//
// psp_main.cpp for psp in /home/lessyv/taf/zia/src/psp
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Tue Apr 24 15:02:20 2007   lessyv
// last update   Wed Apr 25 08:57:47 2007   loic
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
  PerlInterpreter.init_perl(0); // oO mgep
  return true;
}

void		Psp::apply_psp()
{
  BlocParser->find_code_to_replace();
  PerlInterpreter.eval_perl(BlocParser->get_last_bloc_code());
  BlocParser->apply_code(BlocParser->get_last_bloc_code());
}

const string	&Psp::replace_all_psp()
{
  int		i;

  i = BlocParser->get_blocs_found();
  if (BlocParser == NULL)
    {
      cerr << "[PSP] Not initialized" << endl;
      return 0;
    }
  while (i != -1)
    {
      cout << "Bloc...";
      apply_psp();
      if (i == BlocParser->get_blocs_found())
	i = -1;
      else
	i = BlocParser->get_blocs_found();
//       replace_one_psp_bloc();
      cout << "...bloC" << endl;
    }
  return (BlocParser->get_computed_page());

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
  cout << "Bloc... IN" << endl;
  if (!BlocParser->parsing_ended())
    return (-1);
  cout << "Bloc... IN 2" << endl;
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
