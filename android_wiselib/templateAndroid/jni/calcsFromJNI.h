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

#ifndef CALC_H
#define CALC_H



template <typename T>
class calc
{
  public:
    T multiply(T x, T y);
    T add(T x, T y);
};
template <typename T>
T calc< T >::multiply(T x,T y)
{
  return x*y;
}
template <typename T>
T calc< T >::add(T x, T y)
{
  return x+y;
}

#endif

