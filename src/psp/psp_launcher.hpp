//
// psp_launcher.hpp for psp in /home/loic/taf/zia/zia84/src/psp
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Fri Apr 20 04:37:44 2007   loic
// last update   Fri Apr 20 05:42:39 2007   loic
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
  int		pos_bloc_begin;
  int		pos_bloc_end;
  int		pos_orig;
  string	page; // A recup plutot depuis l'heritage...
  string	bloc_orig;
  //string	bloc_to_insert  (from interpreter)

  Bloc(); //un autre prenant 1 parametre ; la position dans la page du prochain bloc
  ~Bloc();
  int		case_sensitive_match(char c1, char c2);
  void		find_code_to_replace();
  int		apply_code();
private:
  static const char	*op_begin;
  static const char	*op_end;
};

#endif
