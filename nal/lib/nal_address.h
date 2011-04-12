/******************************************************************************
* This file is part of cbox.
* 
* cbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU LesserGeneral Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* any later version.
* 
* Cbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License
* along with cbox.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/

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

#endif

