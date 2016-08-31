/**
 * \file
 */

#ifndef NETINET_IN_H
#define NETINET_IN_H

#include <stdint.h>

enum IP_PROTO
{
	IPPROTO_IP =		0,		/* Dummy protocol for TCP. */
	IPPROTO_HOPOPTS =	0,		/* IPv6 Hop-by-Hop options. */
	IPPROTO_ICMP =		1,		/* Internet Control Message Protocol. */
	IPPROTO_IGMP =		2,		/* Internet Group Management Protocol. */
	IPPROTO_IPIP =		4,		/* IPIP tunnels (older KA9Q tunnels use 94). */
	IPPROTO_TCP =		6,		/* Transmission Control Protocol.  */
	IPPROTO_EGP =		8,		/* Exterior Gateway Protocol.  */
	IPPROTO_PUP =		12,		/* PUP protocol. */
	IPPROTO_UDP =		17,		/* User Datagram Protocol.  */
	IPPROTO_IDP =		22,		/* XNS IDP protocol. */
	IPPROTO_TP =		29,		/* SO Transport Protocol Class 4.  */
	IPPROTO_IPV6 =		41,		/* IPv6 header.	*/
	IPPROTO_ROUTING =	43,		/* IPv6 routing header. */
	IPPROTO_FRAGMENT =	44,		/* IPv6 fragmentation header.  */
	IPPROTO_RSVP =		46,		/* Reservation Protocol. */
	IPPROTO_GRE =		47,		/* General Routing Encapsulation.  */
	IPPROTO_ESP =		50,		/* encapsulating security payload.  */
	IPPROTO_AH =		51,		/* authentication header.  */
	IPPROTO_ICMPV6 =	58,		/* ICMPv6.  */
	IPPROTO_NONE =		59,		/* IPv6 no next header. */
	IPPROTO_DSTOPTS =	60,		/* IPv6 destination options. */
	IPPROTO_MTP =		92,		/* Multicast Transport Protocol. */
	IPPROTO_ENCAP = 	98,		/* Encapsulation Header. */
	IPPROTO_PIM =		103,	/* Protocol Independent Multicast.  */
	IPPROTO_COMP =		108,	/* Compression Header Protocol. */
	IPPROTO_SCTP =		132,	/* Stream Control Transmission Protocol. */
	IPPROTO_RAW =		255,	/* Raw IP packets.  */
	IPPROTO_MAX
};

typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

struct in_addr
{
	in_addr_t s_addr;
};

#define INET_ADDRSTRLEN 16

#endif