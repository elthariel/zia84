//
// http_request.cpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:31:55 2007 Nahlwe
// Last update Fri Mar 16 11:33:14 2007 Nahlwe
//

#include <pthread.h>
#include "http_request.hpp"

using namespace std;


const char * HttpRequest::m_method [] =  { "OPTITONS" , "GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "METHOD" };

HttpRequest::HttpRequest(Socket &sock)
{
  std::string 	buff;
/*  char bu[4096];
  int n;
 */ sock >> buff;
 /* int fd  = open("test", O_RDONLY, 0444);
  n  = read(fd, bu, 4096);
  bu[n] = 0;
  buff += bu;
 */ m_chunk_type = TYPE_HEADER;
  HttpFill(buff);
}

HttpRequest::~HttpRequest()
{

}


int	HttpRequest::split(std::string str, std::string token, std::string chunk)
{
  if (!str.find(token) || str.find(token) + token.length() >= str.length())
	return (0);
  chunk = str.substr(0, str.find(token) + token.length());
  str = str.substr(str.find(token) + token.length(), str.length());
  return (1);
}

int	HttpRequest::is_in_list(std::string str,const char **list)
{
  int	i = 0;

  while (list[i])
   if (str.find(*list[i]))
   return (1);

  return (0);
}

int	HttpRequest::HttpParseChunk(std::string buff)
{
  std::string	chunk;

  if (!buff.length())
       return (0);
  if (m_chunk_type != TYPE_DATA)
  {
    if (!split(buff, "\r\n", chunk))
  	return (0);
   return (HttpSetChunk(chunk));
  }
  return (HttpSetChunk(buff));
}


int	HttpRequest::HttpSetHeader(std::string chunk)
{
  std::string	header_method;

  if (!split(chunk, " ", header_method))
	return (0);
  if (!is_in_list(chunk, m_method))
	return (0);
  m_http_map["METHOD"] = chunk;
  //XXX check method (HTTP 1.1)
  m_http_map[chunk] = header_method;
  m_chunk_type = TYPE_MAP;
  return (1);
}

int	HttpRequest::HttpSetMap(std::string chunk)
{
  std::string	value;

  if (chunk.compare("\r\n"))
  {
    m_chunk_type = TYPE_DATA;
    return (1);
  }
  if (!split(chunk, ":", value))
	return (0);
  m_http_map[chunk] = value;

  return (1);
}

int	HttpRequest::HttpSetData(std::string chunk)
{
  m_data = chunk;

  return (1);
}

int	HttpRequest::HttpSetChunk(std::string chunk)
{
  if (m_chunk_type == TYPE_HEADER)
    return (HttpSetHeader(chunk));
  if (m_chunk_type == TYPE_MAP)
   return (HttpSetMap(chunk));
  if (m_chunk_type == TYPE_DATA)
    return (HttpSetData(chunk));

  return (0);
}

void	HttpRequest::HttpFill(std::string buff)
{
  while (HttpParseChunk(buff));
}


