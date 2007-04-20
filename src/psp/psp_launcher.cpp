//
// psp_launcher.cpp for psp in /home/loic/taf/zia/zia84/src/psp
// 
// made by Christophe Malinge
//         <maling_c@lse.epita.fr>
// 
// started on    Fri Apr 20 04:51:02 2007   loic
// last update   Fri Apr 20 06:54:49 2007   loic
//

#include <iostream>
#include "psp_launcher.hpp"

using namespace std;

const char *Bloc::op_begin = "<?psp";
const char *Bloc::op_end = "psp?>";

Bloc::Bloc()
{
  pos_bloc_begin = -1;
  pos_bloc_end = -1;
  pos_orig = 0;
}

Bloc::~Bloc()
{
}

int	Bloc::case_sensitive_match(char c1, char c2)
{
  if (LOWER(c1) == LOWER(c2))
    return (1);
  else
    return (0);
}

void	Bloc::find_code_to_replace()
{
  int	i;
  int	in_bloc; // number of bloc found * 2
  int	checking;

  pos_bloc_end = -1;
  checking = 0;
  in_bloc = 0;
  for (i = pos_orig; i < page.length() && in_bloc < 2; i++)
    {
      if (case_sensitive_match(page[i], op_begin[checking]) && page[i])
	{
	  checking++;
	  in_bloc++;
	  while (case_sensitive_match(page[i + checking], op_begin[checking]))
	    checking++;
	  if (checking == strlen(op_begin)) // Is it a real opening bloc
	    {
	      pos_bloc_begin = i;
	      i += checking;
	    }
	  else
	    in_bloc--;
	  checking = 0;
	}
      if (case_sensitive_match(page[i], op_end[checking]) && page[i])
	{
	  if (in_bloc % 2) // end bloc mark found but bloc not opened
	    ;//cerr << "Bloc not opened, line [XUX], stop" << endl;
	  in_bloc++;
	  while (case_sensitive_match(page[i + checking], op_end[checking]))
	    checking++;
	  if (checking == strlen(op_end))
	    {
	      pos_bloc_end = i;
	      i += checking;
	    }
	  else
	    in_bloc--;
	  checking = 0;
	}
    }
}

int		main()
{
  Bloc		*test = new Bloc();

  test->page = "h,?<\nreth<jer?fg'?>lkjaerlgkjhaer'lkj'lkjsdfg\n\\nsdgawsg\n\\nasdgasdgasdgsdg\n<?pspsdfgaedfglkjhasdg;h}{\n[[][oudthgsfg\nfdsgasdfgasdfgpsp?>\n[sdfgaskjdhgkh;\nvsdcgvsd\nasdgASDG<?PSPsdgasdgasdg\ngsdagasdg'pjwergwrg3456\n5ry3ywg35y35gpsp?>wredgelrjh\ndfsagvkjhasdfg";
  test->find_code_to_replace();

}
