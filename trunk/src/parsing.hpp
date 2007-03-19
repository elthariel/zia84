#ifndef	__PARSE_H__
#define	__PARSE_H__
#include <stdarg.h>
#include <iostream>
#include <vector>


class	string2 :public std::string
{
public:
typedef   std::vector<string2 > tokenlist;
typedef   std::vector<tokenlist *> listtokenlist; 
		string2();
		string2(std::string);
		string2(std::string &);
		string2(const char *str);
void		tolower();
void		strip(std::string token);
int		is_in_list(const char **list);
int		split(std::string token, std::string &chunk);
int		is_in(char c, string2 list);
int		findtoken(string2 &str, std::string::iterator &istr, string2 type, string2 delimitor, string2 space);
int		findtoken(string2 &str, std::string::iterator &istr, string2 type, string2 delimitor, string2 space, tokenlist &tl);
int		findtoken(string2 &str, std::string::iterator &istr, string2 type, string2 delimitor, string2 space, size_t size);
int		findtoken(string2 &str, std::string::iterator &istr, string2 type, string2 delimitor, string2 space, tokenlist &tl, size_t size);
listtokenlist	tokenize(string2 exp, ...);
#define ALPH_NUM "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"	
};
#endif
