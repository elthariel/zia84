//
// psp_launcher.cpp for psp in /home/loic/taf/zia/zia84/src/psp
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Fri Apr 20 04:51:02 2007   loic
// last update   Fri Apr 20 10:28:47 2007   loic
//

#include <iostream>
#include "psp_launcher.hpp"

using namespace std;

const char *Bloc::op_begin = "<?psp";
const char *Bloc::op_end = "psp?>";

/**
 * -> find_code_to_replace
 *	parse un bloc crunchy
 *		<- (str)page (globale ? heritee ?)
 *	parseme de plusieurs groupe de 2 balises
 *		<- (str)op_begin
 *		<- (str)op_end
 *      enregistre la position du premier bloc entoure par ces balises pour le renvoyer
 *	ne tient pas compte de la casse pSPsPSpPSP
 *              -> pos_bloc_begin
 *              -> pos_bloc_end
 *	et decale la position de recherche initiale pour un futur appel
 *              -> pos_orig
 * -> get_bloc_code
 *	retourne le prochain bloc de code a interpreter
 *		-> (str) s_bloc_code
 *	
 *
 * TODO string Bloc::interpret_bloc() { PERLIZE(s_bloc_code->c_str) ... return(bloc_to_insert) }
 */

Bloc::Bloc()
{
  pos_bloc_begin = -1;
  pos_bloc_end = -1;
  pos_orig = 0;
  blocs_count = 0;
  parse_ended = 0;
}

Bloc::~Bloc()
{
  delete	s_out;
}

int	Bloc::case_sensitive_match(char c1, char c2)
{
  if (LOWER(c1) == LOWER(c2))
    return (1);
  else
    return (0);
}

void		Bloc::shift_counters(int actual_pos, int bloc_found)
{ // bloc_found == bloc_ok,
  // plutot le gerer avec blocs_count, qui sera
  // donc a diviser par deux pour chier une erreur si jamais une seule balise ouvrante et magie, moins dlignes
  if (!bloc_found || pos_bloc_begin == -1 || pos_bloc_end == -1)
    parse_ended = 1;//TODO cerr en consequence
  if (pos_bloc_begin != -1 && pos_bloc_end != -1)\
    {
      pos_orig = pos_bloc_end + strlen(op_end);
      blocs_count++;
      //pos de prochain start trop grande, servira a rien de relooper
      if (pos_orig >= s_page->length() - ( strlen(op_begin) + strlen(op_end)))
	  actual_pos == s_page->length();
    }
  if (actual_pos == s_page->length())
    {
      parse_ended = 1;
      pos_orig = s_page->length();
    }
}

void	Bloc::find_code_to_replace()
{
  int	i;
  int	bloc_ok;
  int	checking;

  pos_bloc_begin = -1;
  pos_bloc_end = -1;
  checking = 0;
  bloc_ok = 0;
  for (i = pos_orig; i < s_page->length() && bloc_ok < 2; i++)
    {
      if (case_sensitive_match(s_page->c_str()[i], op_begin[checking]) && s_page->c_str()[i])
	{
	  checking++;
	  bloc_ok++;
	  while (case_sensitive_match(s_page->c_str()[i + checking], op_begin[checking]))
	    checking++;
	  if (checking == strlen(op_begin)) // Is it a real opening bloc
	    {
	      pos_bloc_begin = i;
	      i += checking;
	    }
	  else
	    bloc_ok--;
	  checking = 0;
	}
      if (case_sensitive_match(s_page->c_str()[i], op_end[checking]) && s_page->c_str()[i])
	{
	  if (!(bloc_ok % 2)) // end bloc mark found but bloc not opened
	    parse_ended = 1; // TODO cerr << "Bloc not opened, line [XUX], stop" << endl;
	  bloc_ok++;
	  while (case_sensitive_match(s_page->c_str()[i + checking], op_end[checking]))
	    checking++;
	  if (checking == strlen(op_end))
 	    {
	      pos_bloc_end = i;
	      i += checking;
	    }
	  else
	    bloc_ok--;
	  checking = 0;
	}
    }
  shift_counters(i, bloc_ok);
}

string		Bloc::get_bloc_code()
{
  unsigned int	size;

  if (!parsing_ended())
    {
      size = pos_bloc_end - pos_bloc_begin - strlen(op_begin);
      s_bloc_code = s_page->substr(pos_bloc_begin + strlen(op_begin), size);
    }
  return (s_bloc_code);
}

bool		Bloc::parsing_ended()
{
  return (parse_ended);
}

void		Bloc::apply_code()
{
  char		*tmp_str;

  sprintf(tmp_str, "[%i] replaced [%i]", blocs_count, blocs_count);
  s_bloc_to_insert = new string(tmp_str);
  free(tmp_str);
  s_out->replace(pos_bloc_begin, pos_bloc_end - pos_bloc_begin + strlen(op_begin), s_bloc_to_insert);
  //TODO delete s_bloc_to_insert
}

// BENCHeurTeSTeUR
int		main()
{
  Bloc		*test = new Bloc();

  test->s_page = new string("h,?<\nreth<jer?fg'?>lkjaerlgkjhaer'lkj'lkjsdfg\n\\nsdgawsg\n\\nasdgasdgasdgsdg\n<?pspsdfgaedfglkjhasdg;h}{\n[[][oudthgsfg\nfdsgasdfgasdfgpsp?>\n[sdfgaskjdhgkh;\nvsdcgvsd\nasdgASDG<?PSPsdgasdgasdg\ngsdagasdg'pjwergwrg3456\n5ry3ywg35y35gpsp?>wredgelrjh\ndfsagvkjhasdfg");
  test->s_out = new string(test->s_page->c_str());
  cout << "orig: " << *test->s_out << endl << endl;
  while (!test->parsing_ended())
    {
      test->find_code_to_replace();
      if (!test->parsing_ended())
	{
	  test->get_bloc_code();
	  test->apply_code();
	  delete (test->s_bloc_to_insert);
	  cout << " : " << *test->s_out << endl;
	}
    }
}
