//
// psp_main.hpp for psp in /home/lessyv/taf/zia/src/psp
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Tue Apr 24 15:07:04 2007   lessyv
// last update   Wed Apr 25 02:50:12 2007   lessyv
//

#ifndef		__HAS_PSP_MAIN_H__
# define	__HAS_PSP_MAIN_H__

#include	"bloc.hpp"
#include	"perlembed.hpp"

class		Psp
{
public:
  Psp();
  ~Psp();
  // returns true : ok need processing
  // false : error encountered, cerr done
  bool		init_psp(const string &, char **);
  // returns number of blocs already found
  int		replace_all_psp();
  // -1 if no more bloc
  int		replace_one_psp_bloc();
  // returns a reference to the final page
  string	&get_computed_page();
  // returns true : psp searching'n'interpreting done
  // false : not computed to the end of page
  bool		psp_done();
private:
  Bloc		*BlocParser;
  PerlEmbed	PerlInterpreter;
  void		apply_psp();

  // contains header, obtained from server, for perl code
};

#endif
