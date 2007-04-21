/*
 * Embedded perl core
 */

#ifndef PERLEMBED_HPP_
# define PERLEMBED_HPP_

#include <EXTERN.h>
#include <perl.h>
#include <string>

class PerlEmbed
{
public:
  PerlEmbed();
  ~PerlEmbed();

  /*
   * Init perl and clean perl env, call it before each psp file
   */
  void                  init_perl();

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
  void                  create_pipe();
  void                  inject_fd();

  PerlInterpreter       *m_perl;
  bool                  m_perl_initialized;
  int                   m_read_fd, m_write_fd;
};

#endif
