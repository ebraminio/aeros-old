/**
 *  \file
 */

#ifndef NETINET_IP_H
#define NETINET_IP_H

struct iphdr
{
	uint8_t ihl : 4;			/**< Header lengh in 32bits units */
	uint8_t version : 4;		/**< On Big Endian machines, version field come before ihl */
	uint8_t tos;				/**< Bits 0-5 : DSCP, bits 6-7 ECN */
	uint16_t tot_len;
	uint16_t id;
	uint16_t frag_off;			/**< First 3bits contains flags */
	uint8_t ttl;
	uint8_t protocol;
	uint16_t check;				/**< Checksum of header, as computed by ip_checksum */
	uint32_t saddr;
	uint32_t daddr;
	// Options start here
} __attribute__ ((packed));

#endif