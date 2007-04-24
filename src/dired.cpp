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
  while ((direl = readdir(dir)))
  {
          m_dir.push_back(string(direl->d_name));
  }
}

std::string                   Dired::output()
{
  string buff;
  list<string>::iterator i;

  buff += output_header();
  for (i = m_dir.begin(); i != m_dir.end(); i++)
    {
       buff += "<a href=\"" + *i + "\">"
           + *i + "</a>" + "<br>\n";
    }
  buff += output_footer();
  return (buff);
}

std::string                    Dired::output_header()
{
  string buff = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n<html>\n<head>\n<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\n<title>Ziadir : " + m_path;
  buff += "</title>\n</head>\n<body>\n<h2>Viewing directory : ";
  buff += m_path + "</h2>\n<br><br>\n";
  return (buff);
}

std::string                    Dired::output_footer()
{
  string buff = "<h3>Zia server on ";
  buff += HttpdConf::get().get_key("/zia/server/name")->c_str();
  buff += "</h3>\n</body>\n</html>\n\n";
  return (buff);
}
