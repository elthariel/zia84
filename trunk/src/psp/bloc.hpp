//
// bloc.hpp for psp in /home/lessyv/taf/zia/src/psp
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Fri Apr 20 04:37:44 2007   lessyv
// last update   Tue Apr 24 19:56:52 2007   lessyv
//

#ifndef	__BLOC_H__
#define	__BLOC_H__

#include <string>

using namespace std;

#define	LOWER(Ch)	(((Ch) >= 'A' && (Ch) <= 'Z') ? \
                         ((Ch) += 'a' - 'A') : (Ch))

// : public Bidul (pour recup la page ou un bloc qui devrait contenir du code)
class		Bloc
{
public:
  //string	*s_page; // A recup plutot depuis perl lta... (heritage ? non)
  string	*s_out; // crade, need a constructor
  unsigned int	pos_orig;

  Bloc(); //un autre prenant 1 parametre ; la position dans la page du prochain bloc
  ~Bloc();
  void		init(string &);
  void		find_code_to_replace();
  void		apply_code(string &);
  bool		parsing_ended();
  string	&get_last_bloc_code();
  string	&get_computed_page();
  int		get_blocs_found();
protected:
  int		case_sensitive_match(char c1, char c2);

private:
  string	&get_bloc_code();
  void		shift_counters(string &);

  static const char	*op_begin;
  static const char	*op_end;
  string		s_bloc_code; // last found bloc
  int			blocs_count;
  int			pos_bloc_begin;
  int			pos_bloc_end;
  bool			parse_ended;
};

#endif
