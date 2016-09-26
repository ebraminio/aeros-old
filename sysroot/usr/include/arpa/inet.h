/**
 * \file
 */

#ifndef ARPA_INET_H
#define ARPA_INET_H

#include <netinet/in.h>
#include <sys/socket.h>

uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);

#define ntohl(x)	__builtin_bswap32(x)
#define ntohs(x)	__builtin_bswap16(x)
#define htonl(x)	__builtin_bswap32(x)
#define htons(x)	__builtin_bswap16(x)

in_addr_t inet_addr(const char* cp);

/**
 * \brief IPv4 address manipulation
 *
 * The inet_ntoa() shunction shall convert the Internet host address specified
 * by \a in to a string in the Internet standard dot notation.
 *
 * \note The inet_ntoa() function need no to be thread-safe.
 * \TODO Move this documentation block
 */
char* inet_ntoa(struct in_addr in);
const char* inet_ntop(int af, const void *src, char *dst, socklen_t size);

int inet_pton(int af, const char *src, void *dst);

int inet_aton(const char *cp, struct in_addr *inp);
in_addr_t inet_network(const char *cp);
struct in_addr inet_makeaddr(int net, int host);
in_addr_t inet_lnaof(struct in_addr in);
in_addr_t inet_netof(struct in_addr in);

#endif