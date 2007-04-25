/*
 * Embedded perl core
 */

#ifndef PERLEMBED_HPP_
# define PERLEMBED_HPP_

extern "C"
{
#include <EXTERN.h>
#include <perl.h>
}
#include <string>

class PerlEmbed
{
public:
  PerlEmbed();
  ~PerlEmbed();

  /*
   * Init perl and clean perl env, call it before each psp file
   */
  void                  init_perl(char **a_env); // = NULL );
  // a faire a chaque debut instance de page

  /*
   * Cause the perl code in the string to be evaluated by the current perl
   * interpreter.
   */
  bool                  eval_perl(std::string &);

  /*
   * Get everything the perl statements wrote on stderr & stdout.
   */
  std::string           &get_out();

  protected:
  void                  get_perl_api();
  void                  inject_api();

  PerlInterpreter       *my_perl;
  bool                  m_perl_initialized;
  static const char     *m_perl_api;
  static int            m_perl_api_fd;
};

#endif
