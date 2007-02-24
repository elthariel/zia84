//
// dired.cpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:32:22 2007 Nahlwe
// Last update Sat Feb 24 17:50:37 2007 Nahlwe
//

#include <sys/types.h>
#include <dirent.h>

#include "dired.hpp"

using namespace std;

Dired::Dired(std::string a_path)
  : m_path(a_path)
{
  read_directory();
}

Dired::~Dired()
{
}

void                    Dired::read_directory()
{
  DIR                   *dir;
  struct dirent         *direl;

  dir = opendir(m_path.c_str());
  if (dir)
    {
      while (direl = readdir(dir))
        {
          m_dir.push_back(string(direl->d_name))
;
        }
    }
  else
    {
      //FIXME generate appropriate html error
    }
}
