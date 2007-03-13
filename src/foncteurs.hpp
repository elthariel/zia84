//
// foncteurs.hpp for zia in /home/elthariel/code/httpd/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Fri Feb 23 08:08:39 2007 Nahlwe
// Last update Thu Mar  8 18:20:10 2007 Nahlwe
//

template <class return_type>
class iFoncteur0
{
public:
  virtual return_type   operator()() = 0;
  virtual ~iFoncteur0();
};


/*
template <class return_type>
class foncteur0\
{
public:
  typedef return_type (&Fun)();

  foncteur0(Fun a_fun);
  virtual return_type   operator()();

private:
  Fun                   m_fun;
};



template <class return_type, class arg0>
class foncteur1
{
public:
  typedef return_type (&Fun)(arg0);

  foncteur0(Fun a_fun);
  virtual return_type   operator()(arg0);

private:
  Fun                   m_fun;
};
*/
