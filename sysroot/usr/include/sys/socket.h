/**
 *  \file
 */

#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H

#include <stddef.h>

#define AF_UNSPEC	0
#define AF_UNIX		1	/**< Unix domain sockets */
#define AF_INET		2	/**< Internet IP Protocol */
#define AF_BRIDGE	7	/**< Multiprotocol bridge */
#define AF_INET6	10	/**< IP version 6 */

#define PF_UNSPEC	AF_UNSPEC
#define PF_UNIX		AF_UNIX
#define PF_INET		AF_INET
#define PF_BRIDGE	AF_BRIDGE
#define PF_INET6	AF_INET6

typedef size_t socklen_t;

#endif