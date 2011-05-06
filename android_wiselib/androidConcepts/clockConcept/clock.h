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

#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <stdint.h>
using namespace std;

//implementation of clock concept in android platform
class AndroidClockModel
{
public:
	typedef struct timeval time_t;
	int state();

	//returns a struct containing time properties
	time_t time()
	{
		time_t tv;
		gettimeofday(&tv,NULL);
		return tv;
	}
	//returns the total microseconds of time struct argument
	uint32_t microseconds( time_t time )
	{
		uint32_t value = time.tv_sec * 1000000 + time.tv_usec;
		return value;
	}
	//returns the total milliseconds of time struct argument
	uint32_t milliseconds( time_t time )
	{
		uint32_t value = time.tv_sec * 1000 + (uint32_t)(time.tv_usec/1000);
		return value;
	}
	//returns the total seconds of time struct argument
	uint32_t seconds( time_t time )
	{
		return time.tv_sec;
	}

};


#endif
