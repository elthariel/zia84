//
// helpers.hpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 09:32:22 2007 Nahlwe
// Last update Tue Mar 13 05:09:58 2007 Nahlwe
//

#ifndef HELP_HPP_
# define HELP_HPP_

#include <string>

class Excp
{
public :
  Excp();
  Excp(std::string str);
  virtual ~Excp();

  virtual const std::string  &what();
private:
  std::string           m_str;
};


class NonCopyable
{
public:
  NonCopyable();
  virtual ~NonCopyable();
  class CopyExcexption : public Excp {};

private:
  NonCopyable(NonCopyable &);

  static CopyExcexption         m_copy_exp;
};

#endif
