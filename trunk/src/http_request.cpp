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
/**/
/*FOR TEST PURPOSE ONLY */
void HttpRequest::HttpTest()
{
 HttpMap::iterator	i;

  cerr << "chunk type: " << m_chunk_type << endl; 
  cerr << "MAP:" << endl;
  for (i = m_http_map.begin(); i != m_http_map.end(); i++)
    {
      cerr << (*i).first << ":" << (*i).second <<  endl;
    }
  cerr << "DATA:" << endl << 	m_data << endl;
}


HttpRequest::HttpRequest(char *filename)
{
  string2 	buff;
  char 	bu[4096];
  int 	n;
  int 	fd  = open(filename, O_RDONLY, 0444);

  n  = read(fd, bu, 4096);
  bu[n] = 0;
  buff += bu;
  m_chunk_type = TYPE_HEADER;
  HttpFill(buff);
}


HttpRequest::HttpRequest(Socket &sock)
{
  string2	buff;

/* ici verifier s il lis bien le buff ou sa coupe avant petre pour sa coupe avant le \ */
  sock >> buff;
  m_chunk_type = TYPE_HEADER;
  HttpFill(buff);
  // ici ou set l erreur avec le systeme d erreur si non set une variable d'env
  HttpCheckRequest();

  /*peut etre si le get est bon creer directement le nom de file 
  et le type 
  mettre des flags ou une fonction qui retourne ce ki fo
  comme ca ds le worker on check ke les flags si creer le header verifie si pas d erruer si non envoie page error
  apres check si get ou post ....
  peut etre gerer l erreur 
  */
}

HttpRequest::~HttpRequest()
{
  
}

int	HttpRequest::HttpCheckRequest(void)
{
  string::size_type	pos;
  string	chunk;
  string2	subchunk;
  string2	chunk2;
/*
 *
 * Parse pour savoir si le httprequest est bien generer si non renvoie bad request
 *
 * */

  chunk = m_http_map["method"];
  chunk2.append(chunk);
  if (!chunk2.is_in_list(m_method))
    return (0);
  chunk = m_http_map[m_http_map["method"]];
  if ((pos = chunk.rfind("HTTP/")) == string::npos)
    return (0);
  chunk2 = chunk.substr(pos, chunk.length());
  if (!chunk2.split("/", subchunk))
    return (0);
  if (subchunk.find("HTTP") == string::npos)
    return (0);
  if (chunk2.length() != 3)
    return (0);
  if ((pos = chunk2.find("1.1")) == string::npos)
    return (0);

  /*
   * check les map du header
   */
  
  return (1);
}

void	HttpRequest::HttpFile(FilePath &file)
{
  std::string chunk;
  string2  chunk2;
  struct   stat st;

  /* ici on a deja checker que le get est bon normalllement
   * si le fichier est pas bon on peut renvoyer un bad file 
   */
  file.Path = HttpdConf::get().get_key("/zia/server/root")->c_str();
  chunk = m_http_map[m_http_map["method"]];
  chunk2.append(chunk);
  chunk2.split(" ", chunk);
  if (!chunk.compare("/"))
   file.Path += chunk + "index.html";
  else
   file.Path += "/" + chunk;
  if (stat(file.Path.c_str(), &st)  == -1)
  {
  //XXX pas renvoyer le fichier mais construire une reponse bad
  file.Path = HttpdConf::get().get_key("/zia/server/root")->c_str();
  file.Path += "/error.html";
  }
}

int	HttpRequest::HttpParseChunk(string2 &buff)
{
  string2	chunk;

  if (!buff.length())
       return (0);
  if (m_chunk_type != TYPE_DATA)
  {
    if (!buff.split("\r\n", chunk))
  	return (HttpSetChunk(buff));
   return (HttpSetChunk(chunk));
  }
  return (HttpSetChunk(buff));
}


int	HttpRequest::HttpSetHeader(string2 chunk)
{
  string2	header_method;

  if (!chunk.split(" ", header_method))
	return (0);
 
  if (!chunk.is_in_list(m_method))
	return (0);
  
  m_http_map["method"] = header_method;
  m_http_map[header_method] = chunk;
  m_chunk_type = TYPE_MAP;
  return (1);
}

int	HttpRequest::HttpSetMap(string2 chunk)
{
  string2	key;

  if (!chunk.length())
  {
    m_chunk_type = TYPE_DATA;
    return (1);
  }
  if (!chunk.split(":", key))
     return (0);
  key.tolower();
  key.strip(" ");
  key.strip("	");
  m_http_map[key] = chunk;
  
  return (1);
}

int	HttpRequest::HttpSetData(string2 &chunk)
{
  m_data = chunk;
  chunk = "";

  return (1);
}

int	HttpRequest::HttpSetChunk(string2 &chunk)
{
  if (m_chunk_type == TYPE_HEADER)
    return (HttpSetHeader(chunk));
  if (m_chunk_type == TYPE_MAP)
   return (HttpSetMap(chunk));
  if (m_chunk_type == TYPE_DATA)
    return (HttpSetData(chunk));
  return (0);
}

void	HttpRequest::HttpFill(string2 buff)
{
  while (HttpParseChunk(buff));
}

