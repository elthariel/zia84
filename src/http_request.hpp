//
// http_request.hpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 15:32:05 2007 Nahlwe
// Last update Sat Feb 24 15:32:08 2007 Nahlwe
//

#include <string>
#include "conf.hpp"
#include "socket.hpp"

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
  ~HttpRequest();

int	split(std::string str, std::string token, std::string chunk);
int	is_in_list(std::string str,const char **list);
int	HttpParseChunk(std::string buff);
int	HttpSetHeader(std::string chunk);
int	HttpSetMap(std::string chunk);
int	HttpSetData(std::string chunk);
int	HttpSetChunk(std::string chunk);
void	HttpFill(std::string buff);

  static  const char *m_method[];
  typedef __gnu_cxx::hash_map<std::string,
                            std::string,
                            __gnu_cxx::hash<std::string> > HttpMap;

  /**
   * Feed a line of the request to the parser.
   * Return true if the line has been accepted. If false is returned
   * you must stop feeding the request, an error code will be sent back.
   */
  bool                  feed(std::string line);
  int			m_chunk_type; 
private:
  void                  process();
  bool                  parse_line(std::string &line);
  enum 			chunk_type { TYPE_HEADER, TYPE_MAP, TYPE_DATA };
  std::string		m_data;
  HttpMap		m_http_map;
  // Internals
  unsigned int          m_return_count;
};
