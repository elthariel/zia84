//
// dired.cpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:32:22 2007 Nahlwe
// Last update Fri Mar 16 11:37:00 2007 Nahlwe
//

#include <pthread.h>
#include <sys/types.h>
#include <iostream>
#include <dirent.h>

#include "conf.hpp"
#include "dired.hpp"

using namespace std;

Dired::Dired(std::string a_path)
  : m_path(a_path)
{
  read_directory();
}

Dired::Dired()
{
}

void                    Dired::read_directory()
{
  DIR                   *dir;
  struct dirent         *direl;

  dir = opendir(m_path.c_str());
  if (dir)
    {
      while ((direl = readdir(dir)))
        {
          m_dir.push_back(string(direl->d_name));
        }
    }
  else
    {
      //FIXME generate appropriate html error
    }
}

void                    Dired::output()
{
  list<string>::iterator i;

  output_header();
  for (i = m_dir.begin(); i != m_dir.end(); i++)
    {
      cout << "<a href=\"" << *i << "\">"
           << *i << "</a>" << "<br>"<< endl;
    }
  output_footer();
}

void                    Dired::output_header()
{
  cout << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">"
       << endl << "<html>" << endl
       << "<head>" << endl
       << "<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">"
       << endl << "<title>Ziadir : "
       << m_path << "</title>" << endl
       << "</head>" << endl
       << "<body>" << endl
       << "<h2>Viewing directory : " << m_path << "</h2>" << endl
       << "<br><br>" << endl;
}

void                    Dired::output_footer()
{
  cout << "<h3>Zia server on " << HttpdConf::get().get_key("/zia/server/name")
       <<"</h3>" << endl
       << "</body>" << endl
       << "</html>" << endl << endl;
}
