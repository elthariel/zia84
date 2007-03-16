//
// dired.hpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:31:48 2007 Nahlwe
// Last update Fri Mar 16 08:20:42 2007 Nahlwe
//

#include <string>
#include <list>
//#include "http_request.hpp"

class Dired
{
public:
  Dired(std::string a_path);

  void                        output();
private:
  Dired();      // Generates html error

  void                          read_directory();
  void                          output_header();
  void                          output_footer();

  std::string                   m_path;
  std::list<std::string>        m_dir;
};
