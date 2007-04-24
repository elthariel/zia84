//
// psp_main.cpp for psp in /home/loic/taf/zia/zia84
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Tue Apr 24 15:02:20 2007   loic
// last update   Tue Apr 24 17:45:14 2007   loic
//

#include <iostream>
#include <fstream> //just needed to test replacing <?psp psp?> code in an html FILE

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

void		Psp::set_local_page(string &page)
{
// XX this code open a file
// XX Here we need the raw buffer corresponding to the html page
// we must get the page requested to PERLIZE it
  fstream	m_filestream;
  char		filename[] = "test_page.html";

  if (page.length() > 0)
    return ;
  //  if (page != NULL)	     
  m_filestream.open(filename);
  if (m_filestream == false)
    {
      cerr << "Unable to open " << filename << endl;
      return ;
    }
  m_filestream.read(local_page, 98765);
  page.append(local_page);
  // in this case delete object page
}

bool		Psp::init(string &page)
{
  if (BlocParser != NULL)
    {
      cerr << "[PSP] already init" << endl;
      return false;
    }
  if (page.length() == 0)
    set_local_page(page);
  // XX obtain it from API raw buffer
  // corresponding to the web page readed on filesystem
  if (page.length() > 0)
    BlocParser->init(page);
  else
    cerr << "No HTML page found" << endl;
  PerlInterpreter.init_perl();
  cout << "[PSP] ok to proceed" << endl;
  return true;
}

void		Psp::apply_psp()
{
  BlocParser->find_code_to_replace();
  PerlInterpreter.eval_perl(BlocParser->get_last_bloc_code());
  BlocParser->apply_code(BlocParser->get_last_bloc_code());
}

int	Psp::replace_all_psp(string &page)
{
  if (BlocParser == NULL)
    init(page);
  while (!BlocParser->parsing_ended())
    {
      replace_one_psp_bloc(page);
    }
  return (BlocParser->get_blocs_found());
}

//AND int	Psp::replace_one_psp_bloc()
int	Psp::replace_one_psp_bloc(string &page)
{
  //to check just if page is not set
  if (BlocParser == NULL)
    init(page);
  apply_psp();
  printf("[PSP] source code %i : %s\n",
	 BlocParser->get_blocs_found(),
	 BlocParser->get_last_bloc_code().c_str());
  if (BlocParser->parsing_ended())
    return (-1);
  return (BlocParser->get_blocs_found());
}
