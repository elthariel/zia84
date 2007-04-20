/*
 * psp_parse_tst.c for psp in /home/lessyv/code/zia/psp
 * 
 * made by Christophe Malinge
 *         <maling_c@lse.epita.fr>
 * 
 * started on    Fri Apr 20 01:41:35 2007   lessyv
 * last update   Fri Apr 20 04:28:00 2007   lessyv
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	OP_BEGIN	"<?psp"
#define	OP_END		"psp?>"
#define	LOWER(Ch)	(((Ch) >= 'A' && (Ch) <= 'Z') ?                        \
                         ((Ch) += 'a' - 'A') : (Ch))

// TODO Make me see the pozerful object world
typedef struct {
  int	begin;
  int	end;
  //char  *bloc to insert
  //int   end of mega block -> stop search
}	s_bloc;

int		case_sensitive_match(char c1, char c2)
{
  if (LOWER(c1) == LOWER(c2))
    return (1);
  else
    return (0);
}

void		find_code_to_replace(char *str, int start_pos, s_bloc *found)
{
  int		i;
  int		in_bloc; // number of bloc found * 2
  int		checking;

  found->end = -1; // TODO found should become an object ...
  for (i = start_pos, checking = 0, in_bloc = 0; str[i] && in_bloc < 2; i++)
    {
      if (case_sensitive_match(str[i], OP_BEGIN[checking]) && str[i])
	{
	  checking++;
	  in_bloc++;
	  while (case_sensitive_match(str[i + checking], OP_BEGIN[checking]))
	    checking++;
	  if (checking == strlen(OP_BEGIN)) // Is it a real opening bloc
	    {
	      found->begin = i;
	      i += checking;
	    }
	  else
	    in_bloc--;
	  checking = 0;
	}
      if (case_sensitive_match(str[i], OP_END[checking]) && str[i])
	{
	  if (in_bloc % 2) // end bloc mark found but bloc not opened
	    ;//cerr << "Bloc not opened, line [XUX], stop" << endl;
	  in_bloc++;
	  while (case_sensitive_match(str[i + checking], OP_END[checking]))
	    checking++;
	  if (checking == strlen(OP_END))
	    {
	      found->end = i;
	      i += checking;
	    }
	  else
	    in_bloc--;
	  checking = 0;
	}
    }
}

int		apply_code(char *pure_page)
{
  int		i;
  int		bloc_end_here;
  int		bloc_size;
  s_bloc	Bloc;
  char		*script;

  // TODO err managment
  bloc_end_here = 0;
  Bloc.begin = -1;
  Bloc.end = 0;
  while (Bloc.end != -1)
    {
      find_code_to_replace(pure_page, bloc_end_here, &Bloc);
      if (Bloc.end > 0)
	{
	  bloc_end_here = Bloc.end + strlen(OP_END);
	  Bloc.begin += strlen(OP_BEGIN);
	  bloc_size = Bloc.end - Bloc.begin;
	  script = calloc(sizeof(char *), bloc_size + 1);
	  for (i = 0; i < bloc_size && pure_page[Bloc.begin + i]; i++)
	    {
	      script[i] = pure_page[Bloc.begin + i];
	    }
	  script[i] = 0;
	  printf("FROM %i to %i -> do it PERL !! : %s\n", Bloc.begin, Bloc.end, script);
	  free(script);
	}
    }
  return (0);
}

//bencheurtesteur

#define	STR_SIZE	512
#define PRINTABLE_CHAR	94
#define	BLOC_TO_INSERT	3

int	main()
{
  char	c;
  char	*str;
  int	i;
  int	blocs = BLOC_TO_INSERT * 2;

  srandom(getpid());
  str = calloc(sizeof(char *), STR_SIZE + 1);
  // filling string

  for (i = 0; i < STR_SIZE; i++)
    {
      if (i < (STR_SIZE - ((blocs - 1) * (STR_SIZE / 6))))
	{
	  if (random() % 80)
	    str[i] = (random() % PRINTABLE_CHAR) + 32;
	  else
	    str[i] = '\n';
	}
      else if (!(blocs % 2))
	{
	  printf("inserting bloc @ %i", i);
	  blocs--;
	  str[i++] = '<';
	  str[i++] = '?';
	  str[i++] = 'p';
	  str[i++] = 'S';
	  str[i] = 'P';
	}
      else
	{
	  printf(", closing @ %i\n", i);
	  blocs--;
	  str[i++] = 'P';
	  str[i++] = 's';
	  str[i++] = 'P';
	  str[i++] = '?';
	  str[i] = '>';
	}
    }
  printf("\nOrig str: %s\n\n", str);
  str[STR_SIZE] = 0;
  //printf("%s\n", str);
  apply_code(str);
}
