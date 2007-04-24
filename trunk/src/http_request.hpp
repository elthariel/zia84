//
// http_request.hpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:32:05 2007 Nahlwe
// Last update Mon Apr 23 10:25:45 2007 
//
#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__
#include <string>
#include <iostream>
#include <ctime>
#include <sys/types.h>
#ifdef XNIX
# include <sys/wait.h>
#endif
#ifdef WIN_32
#endif
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "conf.hpp"
#include "socket.hpp"
#include "parsing.hpp"


extern	char **environ;

struct status_s
{
  int	code;
  const char *reason;
};


class HttpRequest
{
friend class RequestAdapter;

public:
  HttpRequest(Socket &);
  HttpRequest(char *filename);
  ~HttpRequest();

  int	HttpCheckHttpMap(void);
  int	HttpSetFile(void);
  int	HttpParseChunk(string2 &buff);
  int	HttpSetHeader(string2 chunk);
  int	HttpSetMap(string2 chunk);
  int	HttpSetData(Socket &a_socket);
  int	HttpSetChunk(string2 &chunk);
  void	HttpFill(string2 buff);
  void  HttpTest();
  int	HttpCheckRequest();
  std::string HttpGetStatus(void);
  std::string  HttpCreateHeader();
  std::string  HttpGetCGI();
  std::string HttpGetReason();
  static  const status_s m_reason[];
  static  const char *m_method[];
  typedef __gnu_cxx::hash_map<std::string,
                            std::string,
                            __gnu_cxx::hash<std::string> > HttpMap;
  int			m_status;
  int			reqcgi;
  int			reqfile;
  int			m_chunk_type;
  HttpMap		m_http_map;
  std::string		m_data;
private:
  enum 			chunk_type { TYPE_HEADER, TYPE_MAP, TYPE_DATA };
  // Internals
  unsigned int          m_return_count;
};

#endif
