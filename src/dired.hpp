//
// dired.hpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:31:48 2007 Nahlwe
// Last update Fri Mar 16 08:20:42 2007 Nahlwe
//
#ifndef __DIRED_HPP__
#define __DIRED_HPP__
#include <string>
#include <list>
//#include "http_request.hpp"

class Dired
{
public:
  Dired(std::string a_path);

  std::string                       output();
private:
  Dired();      // Generates html error

  void                          read_directory();
  std::string                   output_header();
  std::string                   output_footer();

  std::string                   m_path;
  std::list<std::string>        m_dir;
};
#endif
