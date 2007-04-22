//
// http_request.hpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:32:05 2007 Nahlwe
// Last update Sat Feb 24 15:32:08 2007 Nahlwe
//
#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__
#include <string>
#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include "conf.hpp"
#include "socket.hpp"
#include "parsing.hpp"

extern	char **environ;
/*
class Uri
{
public:
  Uri(std::string a_uri);

  const std::string     &proto() const;
  const std::string     &host() const;
  const std::string     &path() const;
  const UriType         &type() const;

  enum UriType
    {
      UriAbs_Host,
      UriAbs,
      UriRel,
      UriTypeCount
    };

private:
  Uri();

  UriType               m_type;
  std::string           m_proto;
  std::string           m_host;
  std::string           m_path;
};
*/



class HttpRequest
{
public:
  HttpRequest(Socket &);
  HttpRequest(char *filename);
  ~HttpRequest();

  int	HttpSetFile(void);
  int	HttpParseChunk(string2 &buff);
  int	HttpSetHeader(string2 chunk);
  int	HttpSetMap(string2 chunk);
  int	HttpSetData(string2 &chunk);
  int	HttpSetChunk(string2 &chunk);
  void	HttpFill(string2 buff);
  void  HttpTest();
  int	HttpCheckRequest();
  std::string  HttpCreateHeader();
  std::string  HttpGetCGI();

  static  const char *m_method[];
  typedef __gnu_cxx::hash_map<std::string,
                            std::string,
                            __gnu_cxx::hash<std::string> > HttpMap;

  /**
   * Feed a line of the request to the parser.
   * Return true if the line has been accepted. If false is returned
   * you must stop feeding the request, an error code will be sent back.
   */
  int			reqcgi;
  int			reqfile;
  int			m_chunk_type; 
  HttpMap		m_http_map;
private:
  enum 			chunk_type { TYPE_HEADER, TYPE_MAP, TYPE_DATA };
  std::string		m_data;
  // Internals
  unsigned int          m_return_count;
};

#endif
