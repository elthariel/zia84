#include "parsing.hpp"
#include <iostream>

using namespace std;

string2::string2() :std::string()
{
}

string2::string2(std::string str) :std::string(str)
{
}

string2::string2(std::string &str) :std::string(str)
{
}

string2::string2(const char *str) :std::string(str)
{
}

int	string2::is_in(char c, string2 list)
{
  std::string::iterator	ilist;
  
  for (ilist = list.begin(); ilist < list.end(); ilist++)
  	if (c == *ilist)
	  return (1);
  return (0);
}

int	string2::findtoken(string2 &str, std::string::iterator &istr,  string2 type, string2 delimitor, string2 space, tokenlist &ltl , size_t size)
{
  int 	ntoken = 0;
  string2  ctoken;
  tokenlist *tl;
  tl = new tokenlist;

  if (!type.length())
  {
    type = ALPH_NUM;
  }
  for (; istr < str.end() && size; size--) // surcharger plus simple
  {
    if (is_in(*istr, space))
    {
      istr++;
    }
    else if (is_in(*istr, type))
    {
      ctoken += *(istr); 
      istr++;
    }
    else if (is_in(*istr, delimitor))
    {
      istr++;
      if (tl)
      {
       (*tl)[ntoken] = ctoken;
       tl = new tokenlist;
      }
      ntoken++; 
    }
    else
      return (0);
  }
  if (!ctoken.length())
    (*tl)[ntoken] = ctoken;
    
  return (ntoken);
}


int string2::tokenize(string2 exp, ...)
{
 /* form = *c*&t1.*&t1&d1 
  * exp = *$1*$2@.$2@
  * ?
  * ** ds le cas d une etoile
  * $1  type alpha | num
  * &1 delimitor 1
  * %space = ' '
  * @renvoie le token
  * mail : *$1@$@.??$1, "09 az AZ" delim "@"
  */
  int	ntype = 0;
  int	ndelimitor = 0;
  int	nspace = 0;
  tokenlist type;
  tokenlist delimitor;
  tokenlist space;
  va_list arg;
  string2 str = this;
  string2::iterator istr;
  string2::iterator iexp;
  ltl = new listtokenlist; 


  va_start(exp, arg);
  for (iexp = exp.begin(); iexp != exp.end();)
  {
    if (*iexp == '*')
    {
      iexp++;
      if (*iexp == '$')
      {
        iexp++;
  	tmplist = new listtokenlist; 
	if (findtoken(exp, iexp, "0123456789", "", "", tmplist));
          if (atoi(ltl[0][0]) > ntype)
      	    ntype = atoi(ltl[0]0[]);
	delete ltl;
      }  
      else if (*iexp == '&') 
      {
  	tmplist = new listtokenlist; 
	if (findtoken(exp, iexp, "0123456789", "", "", tmplist));
          if (atoi(ltl[0][0]) > ndelimitor)
      	    ndelimitor = atoi(ltl[0][0]);
	delete ltl;
      }
      else if (*iexp == '%')
      {
        tmplist = new tmplist;
	if (findtoken(exp, iexp, "0123456789", "", "", tmplist));
          if (atoi(ltl[0][0]) > nspace)
      	    nspace = atoi(ltl[0][0]);
	delete tmplist;    
      }
      else
      iexp++;
    }
  }
  for (int k = 0; k < ntype; k++)
  va_arg(type[k], arg);
  for (int k = 0; k < ntype; k++)
  va_arg(delimitor[k], arg);
  for (int k = 0; k < ntype; k++)
  va_args(space[k], arg);
  va_end(arg);

  for (iexp = exp.begin(), istr = str.begin(); iexp != exp.end() && istr != istr.end;)
  {
    int addtolist = 0;
    int size = 0;
	
    if ((*iexp = '*'  && *(iexp + 1) != '*') || *iexp = '?')
    {
      while (findtoken(exp, *(iexp + 1), "$&%@", "", "")) 
      {
        if (*(iexp + 1) == '@')
          addtolist = 1;
        while (*iexp == '?')
        {
          size++;
          iexp++;
        }
        if (*(iexp + 1)  == '$')
        {
	  tmplist = new tmplist;
	  iexp++;
 	  if (findtoken(exp, iexp, "0123456789", "", "", tmplist));
            ntype = atoi(token);
	  iexp += token.length();
	  delete tmplist;
        }
        if (*(iexp + 1) == '&')
        {
	  tmplist = new tmplist;
	  iexpp++;
 	  if (findtoken(exp, iexp, "0123456789", "", "", tmplist));
            ndelimitor = atoi(token);
	  iexp += token.length();  
	  delete tmplist;
        }
        if (*(iexp + 1) == '%')
        {
	  tmplist = new tmplist;
	  iexpp++;
 	  if (findtoken(exp, iexp, "0123456789", "", "", tmplist));
            nspace = atoi(token);
	  iexp += token.length();  
	  delete tmplist;
        }
        if (*(iexp + 1) == '@')
        {
	  iexp++;
          addtolist = 1;
        }/* par default a "" ??
        if (!ntype) 
        {
	  type[0] = "";
        }
	if (!ndelimitor)
	{
	  delimitor[0] = "";
	}
	if (!space)
	{
	  space[0] = "";
	}*/
	if (size)
	{
          if (!findtoken(str, istr, type[ntype], delimitor[ndelimitor], space[nspace], ltl, size))
    	    return (0);
	}
	else if (addtolist)
	{
          if (!findtoken(str, istr, type[ntype], delimitor[ndelimitor], space[nspace], ltl))
    	    return (0);
	}
	else if (addtolist && token)
	{
          if (!findtoken(str, istr, type[ntype], delimitor[ndelimitor], space[nspace], ltl, size))
    	    return (0);
	}
	else
	{
        if (!findtoken(str, istr, type[ntype], delimitor[ndelimitor], space[nspace]))
    	  return (0);
	}
      }
    }
    else 
    {
      if (*iexp != *istr)
        return (0);
      str++;	
      iexp++;	
    }
  }
  return (tokenlist);
}

void	string2::tolower()
{
  std::string::iterator i;

  for (i = begin(); i != end(); i++)
      if (*i >= 'A' && *i <= 'Z')
        *(i) = (*i) + 0x20; 
}

int	string2::is_in_list(const char **list)
{
  int	i = 0;
 
  while (list[i])
   if (find(*list[i]) == npos)
   return (1);

  return (0);
}

int	string2::split(std::string token, std::string &chunk)
{

  if (find(token) == npos && find(token) + token.length() >= length())
	return (0);
  chunk = substr(0, find(token));
  *this  = substr(chunk.length() + token.length(),  length());

  return (1);
}

void	string2::strip(std::string token)
{
  while(find(token) != npos)
    erase(find(token), token.length());
}
