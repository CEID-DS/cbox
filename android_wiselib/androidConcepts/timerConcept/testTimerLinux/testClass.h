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

#ifndef TEST_CLASS_H
#define TEST_CLASS_H

#include <iostream>
#include <string.h>
using namespace std;


class testClass
{
	public:
		void charHelloWorld(void *pointer);
		void stringHelloWorld(void *pointer);
};

void testClass::charHelloWorld(void *pointer)
{

	cout << (char *)pointer << endl;
}

void testClass::stringHelloWorld(void *pointer)
{

	cout << *static_cast<std::string*>(pointer) << endl;
}

#endif

