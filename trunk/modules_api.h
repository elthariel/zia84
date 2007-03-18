/*
** module_api.h
** Login : <elthariel@gmail.com>
** Started on  Sun Mar 18 04:06:35 2007 Nahlwe
** Zia Modules API
**
** Copyright (C) 2007 Elthariel
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef   	MODULES_API_H_
# define   	MODULES_API_H_

/*
 * Functions the server exposes to the modules
 */

/**
 * zia_get_key allows the module to have its configuration stored
 * into the main zia xml config file. The key is the path of the
 * config key into the xml document relative to the node of this
 * document in the zia config xml file.
 * Example :
 * <zia>
 *   <server>
 *     <port>80</port>
 *   </server>
 *   <module-x>
 *     <subnode-n>
 *       <config-y>your_config_here</config-y>
 *     </subnode-n>
 *   </module-x>
 * </zia>
 *
 * If you want to get the config-y value for your module-x
 * call zia_get_key("subnode-n/config-y");
 *
 * Of course we can't enforce this policy in the way that it is
 * impossible to forbid a module to use open, read, and write :p.
 * In some words, This is just a facility.
 */
char            *zia_get_key(const char *key);

/*
 * Functions the modules exposes to the server
 */

/*
 * This typedef is here in order to make the code more readable.
 * This is a pointer to your module main class/structure. It is
 * also use by the server as an unique identifier for your module.
 */
typedef void *  ZiaModHandle;


/*
 * There are different hook points that could be used to achieve
 * your module's goal(s). For each a function type is defined
 * below. The way how the module register where it want to hooks
 * is described at the bottom of this document
 */

/*
 * This function is called just after the connection has been
 * etablished (the return of the accept function).
 *
 * Args :
 *  - The handle of your module.
 *  - the socket file descriptor.
 *  - A pointer to a buffer (char *). It is used to give back data
 *    that has been read from the socket, and possibly modified by
 *    the module (decryption by ssl module for instance)
 *  - The size of the data the module sends back to the server.
 *
 * Return :
 *  This function must return 0 if the request has to be dropped
 *  or an integer != 0 if the request process could continue to run.
 */
typedef int     (*hook_init_f)(ZiaModHandle, int32_t, char **,
                             uint32_t *);

/*
 * This function is called after the http header has been parsed.
 * It allows the module to perform changes on the request.
 *
 * Args :
 *  - The handle of your module.
 *  - The socket file descriptor.
 *  - A null terminated array of pointer to string (char *)
 *    array[2n] contains the key (method, uri, Length, ...)
 *    array[2n + 1] contains the value of the key.
 *      (GET, /index.html, 42042, ...)
 *
 * Return :
 *  This function must return 0 if the request has to be dropped,
 *  or 1 if the request validity was tested and approved : so the
 *  server won't drop it even if it would normally.
 *  (For example, the request won't be accomplished by the server,
 *  bu by other parts of the module.
 *  You must an integer != (0, 1) if the request process could
 *  continue to run normally, and you allow the server to generate
 *  an error and drop the request if it want to.
 */
typedef int     (*hook_request_f)(ZiaModHandle, int32_t, char **);

/*
 * This function is called after the request has been accepted by
 * the server (of if the preceding hook function returned 1)
 * This hook function must not make change to the headers.
 * this hook point could be used to exec some psp code.
 *
 * Args :
 *  - The handle of the module.
 *  - the socket file descriptor.
 *  - A read only version of the headers sent in the preceding
 *    hook point. See beside for the format.
 *  - A pointer to a pointer to a buffer. The module sent back
 *    the data he has read or generated to the server via this
 *    buffer. The pointer is set to 0 if the module does not
 *    generates or read data, in this case the server would read it
 *    using its own way.
 *  - The size of the buffer if any.
 *
 * Return 0 if success or the number of the error (404, 503, ...)
 */
typedef int     (*hook_readexec_f)(ZiaModHandle, int32_t, const char const**,
                                   char **, uint32_t);

/*
 * This function is called before the data is sent to the client.
 * It allows you to modify the data before they are send, or to send
 * it by yourself. (SSL crypto for example)
 *
 * Args :
 *  - The handle of the module.
 *  - The socket's file descriptor connected to the client.
 *  - The header in the same format as the 2 preceding functions.
 *  - A pointer to a pointer to a buffer that contains the data
 *    which have been read or generated. You could modify it but
 *    you may want a different size of buffer so you must take
 *    care of memeory management. You must free the data you received
 *    and malloc a new chunk of emeory for your data.
 *    You must set it to 0 if your module is sending the data to
 *    the client and you aren't wanting it to be sent twice.
 *  - The size of the data, you must update it if you are
 *    performing changes to the data.
 *
 * Return :
 * ?
 */
typedef void    (*hook_send_f)(ZiaModHandle, int32_t, char **,
                             char **, uint32_t*);

/*
 * This hook function is called after the data has been sent to the
 * client. Just before the connection closed or before you serve
 * a new request from the same client (keep-alive mode).
 * (It could for a stat module).
 *
 * Args :
 *  - The handle.
 *  - The socket's fd.
 *  - the request header in the same format as above.
 *  - the answer header in the same format as the request header.
 */
typedef void    (*hook_sent_f)(ZiaModHandle, int32_t,
                             char **, char **);

/*
 * This function is called just before a socket connection is
 * closed. It allows (using the init hook) the module to keep
 * track of clients using the socket file descriptor number as
 * an id.
 *
 * Args :
 *  - the handle.
 *  - the fd.
 */
typedef void    (*hook_close_f)(ZiaModHandle, int32_t);

/*
 * This struct represent a module in the server's point of view.
 */
typedef struct  module_s
{
  /*
   * A pointer to the class/structure which represent your module
   * from the module point of view
   */
  ZiaModHandle          handle;
  /*
   * the name of the module, it is used in zia_get_key to
   * find your module's config key.
   */
  char                  name[32];
  /*
   * A bit flag which tell the server which hook functions are
   * provided in this structure (and which hook it need to calls.
   * You must OR all the HOOK_XXXXX you want
   * (e.g. HOOK_INIT | HOOK SENT | HOOK_CLOSE, see the macros below)
   */
  uint32_t              hook_flags;

  /*
   * The pointers to the hook functions for your module.
   */
  hook_init_f           hook_init;
  hook_request_f        hoot_request;
  hook_readexec_f       hook_readexec;
  hook_send_f           hook_send;
  hook_sent_f           hook_sent;
  hook_close_f          hook_close;
}               module_t;

#define         HOOK_INIT       1
#define         HOOK_REQUEST    2
#define         HOOK_READEXEC   4
#define         HOOK_SEND       8
#define         HOOK_SENT       16
#define         HOOK_CLOSE      32

/*
 * A single dynamic library can provides more than one module
 * So in order to register all the module in a library the server
 * will call register_instance with mod_number growing from 0 until
 * it returns a null pointer.
 *
 * For each module, register instance must malloc and return
 * a properly filled module_t structure. This memory is owned by
 * the server your should NOT access to it. The only data the module
 * should modify are those inside his ZiaModHandle.
 */
module_t        *register_instance(uint32_t mod_number);

#endif 	    /* !MODULES_API_H_ */
