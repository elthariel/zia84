//
// http_res.hpp for zia in /home/elthariel/code/zia/src
//
// Made by Nahlwe
// Login   <elthariel@epita.fr>
//
// Started on  Sat Feb 24 17:51:13 2007 Nahlwe
// Last update Sat Feb 24 18:11:55 2007 Nahlwe
//

#include <iostream>

/*
 * An interface for ressource that can be sent back to the client.
 * We need to define with Jason an interface to his socket lib
 * streams ?
 */
class iHttpRes
{
public:
  iHttpRes();

  virtual void          output(ofstream &stream) = 0;

private:

};

class HttpError : public iHttpRes
{
public:
  HttpError();

  void          set_error(unsigned int error);
  void          set_error(Code error);
  const Code    error() const;

  enum Code
    {
      Continue          = 100,
      SwitchingProtocol,
      OK                = 200,
      Created,
      Accepted,
      NonAuthoInfo,
      NoContent,
      ResetContent,
      PartialContent,
      MultipleChoices   = 300,
      MovedPermanently,
      Found,
      SeeOther,
      NotModified,
      UseProxy,
      TemporaryRedirected,
      BadRequest        = 400,
      Unauthorized,
      PaymentRequired,
      Forbidden,
      NotFound,
      MethodNotAllowed,
      NotAcceptable,
      ProxyAuthRequired,
      RequestTimeout,
      Conflict,
      Gone,
      LengthRequired,
      PreconditionFailed,
      EntityTooLarge,
      UriTooLarge,
      UnsupportedMedia,
      BadRange,
      ExpectationFailed,
      InternalServerError       = 500,
      NotImplemented,
      BadGateway,
      ServiceUnavailable,
      GatewayTimeout,
      UnsupportedHttpVersion
    };

private:

  Code          m_error;
};
