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
  {
    if (!compare(list[i]))
     return (1);
    i++;
  }
  return (0);
}

int	string2::split(std::string token, std::string &chunk)
{

  if (find(token) == npos || find(token) + token.length() >= length())
	return (0);
  chunk = substr(0, find(token));
  *this  = substr(chunk.length() + token.length(),  length());

  return (1);
}

void	string2::itime(void)
{
  time_t	tdate;
  string2	date;
  string2	cday;
  string2	month;
  string2	day;
  string2	hours;
  string2	years;

  tdate = time(0);
  date.append(ctime(&tdate));
  date.split(" ", cday);
  date.split(" ", month);
  date.split(" ", day);
  date.split(" ", hours);
  date.strip("\n");
  date = cday + ", " + day + " " + month + " " + date + " " + hours + " " + "GMT";

  *this += date;
}

void	string2::itoa(unsigned int n)
{
  static int first = 1;
  

  if (!first && !n)
  {
    *this += '\0';
    first = 1;
    return ;
  }
  if (first)
  {
    *this += (!n ? '0' :( n < 0) ? '-' : '\0');
    first = 0;
    if (n < 0)
      n = n * - 1;
  }
  itoa(n / 10);
  *this += '0' + n % 10;
}


void	string2::strip(std::string token)
{
  while(find(token) != npos)
    erase(find(token), token.length());
}
