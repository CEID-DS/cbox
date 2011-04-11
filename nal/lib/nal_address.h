#ifndef NAL_H
#define NAL_H

/* address structure for info about connection's establishment */
struct addr {

	char[16] ipv4_addr;	/* allocated string containing the ipv4 address */
	char[40] ipv6_addr;	/* allocated string containing the ipv6 address */
	char[18] mac_addr;	/* allocated string containing the mac (hex) address */

	/* possible other info
	about connection */

	/* it could also be used only
        one char[40] to contain the ip (for any interface)
        and not three */

	};

