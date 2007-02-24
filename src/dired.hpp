//
// dired.hpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:31:48 2007 Nahlwe
// Last update Sat Feb 24 16:10:32 2007 Nahlwe
//

#include <string>
#include <list>
#include "http_request.hpp"

class Dired
{
public:
  Dired(std::string a_path);

  //void                        output(ofstream qqch);
private:
  ~Dired(); // Generate an html error.

  void                          read_directory();

  std::string                   m_path;
  std::list<std::string>        m_dir;
};
