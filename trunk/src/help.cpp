//
// helpers.cpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 09:37:19 2007 Nahlwe
// Last update Thu Mar  8 19:04:11 2007 Nahlwe
//

#include <pthread.h>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <typeinfo>
#include "help.hpp"

using namespace std;

/*
 * Excp class
 */

Excp::Excp()
{
  m_str = typeid(*this).name();
}

Excp::Excp(string str)
  : m_str(str)
{
}

Excp::~Excp()
{
}

const string            &Excp::what()
{
  return m_str;
}

/*
 * NonCopyable helper class
 */

NonCopyable::CopyExcexption NonCopyable::m_copy_exp = NonCopyable::CopyExcexption();

NonCopyable::NonCopyable()
{
}

NonCopyable::~NonCopyable()
{
}

NonCopyable::NonCopyable(NonCopyable &a_copied)
{
  throw NonCopyable::m_copy_exp;
}
