//
// foncteurs.hpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 08:08:39 2007 Nahlwe
// Last update Tue Mar 13 05:52:27 2007 Nahlwe
//

#ifndef IFONC_HPP_
# define IFONC_HPP_

template <class return_type>
class iFoncteur0
{
public:
  virtual return_type   operator()() = 0;
  virtual ~iFoncteur0();
};


template <class return_type>
iFoncteur0<return_type>::~iFoncteur0()
{
}


#endif
