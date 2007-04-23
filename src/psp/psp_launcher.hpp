//
// psp_launcher.hpp for psp in /home/loic/taf/zia/zia84/src/psp
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Fri Apr 20 04:37:44 2007   loic
// last update   Mon Apr 23 13:47:42 2007   loic
//

#ifndef	__PSP_LAUNCHER_H__
#define	__PSP_LAUNCHER_H__

#include <string>

using namespace std;

#define	LOWER(Ch)	(((Ch) >= 'A' && (Ch) <= 'Z') ? \
                         ((Ch) += 'a' - 'A') : (Ch))

// : public Bidul (pour recup la page ou un bloc qui devrait contenir du code)
class		Bloc
{
public:
  string	*s_page; // A recup plutot depuis perl lta... (heritage ? non)
  string	*s_out; // crade, modif dans la chaine originele ?
  string	s_bloc_code;
  string	*s_bloc_to_insert; // from interpreter
  int		blocs_count;
  unsigned int	pos_orig;

  Bloc(); //un autre prenant 1 parametre ; la position dans la page du prochain bloc
  ~Bloc();
  void		find_code_to_replace();
  void		apply_code();
  string	get_bloc_code();
  bool		parsing_ended();
protected:
  int		case_sensitive_match(char c1, char c2);
  void		shift_counters();
private:
  static const char	*op_begin;
  static const char	*op_end;
  int			pos_bloc_begin;
  int			pos_bloc_end;
  bool			parse_ended; // number of bloc found * 2
};

#endif
