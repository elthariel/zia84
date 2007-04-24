//
// psp_main.hpp for psp in /home/lessyv/taf/zia/zia84
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Tue Apr 24 15:07:04 2007   loic
// last update   Tue Apr 24 17:45:21 2007   loic
//

#ifndef		__HAS_PSP_MAIN_H__
# define	__HAS_PSP_MAIN_H__

#include	"bloc.hpp"
#include	"perlembed.hpp"

class		Psp
{
public:
//HTML filled of PERL original source code
  Psp();
  ~Psp();
  // return number of blocs found
  int		replace_all_psp(string &);
  // -1 if no more bloc
  int		replace_one_psp_bloc(string &);

private:
  bool		init(string &);
  char		local_page[98765];

  Bloc		*BlocParser;
  PerlEmbed	PerlInterpreter;
  void		set_local_page(string &);
  void		apply_psp();
};

#endif
