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

class HttpRequest
{
public:
  HttpRequest();
  ~HttpRequest();

  enum RequestType
    {
      HTTP_UNSET,
      HTTP_GET,
      HTTP_HEAD,
      HTTP_POST,
      HTTP_OPTIONS,
      HTTP_TRACE,
      HTTP_PUT,
      HTTP_DELETE,
      HTTP_COUNT
    };

  /**
   * Feed a line of the request to the parser.
   * Return true if the line has been accepted. If false is returned
   * you must stop feeding the request, an error code will be sent back.
   */
  bool                  feed(std::string line);

private:
  void                  process();
  bool                  parse_line(std::string &line);

  // Request components.
  RequestType           m_rtype;
  std::string           m_host;
  std::string           m_referer;
  std::string           m_user_agent;
  std::string           m_con;
  std::string           m_accept;
  std::string           m_charset;
  std::string           m_lang;
  std::string           m_encoding;
  std::string           m_trailer;
  std::string           m_te;
  unsigned int          m_len;

  // Internals
  unsigned int          m_return_count;
};
