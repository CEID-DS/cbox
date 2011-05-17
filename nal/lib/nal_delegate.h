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

#ifndef NAL_DELEGATE_H
#define NAL_DELEGATE_H

#include <iostream>

class delegate{

	typedef void (*stub_type)(void* object_ptr,char *, int);
	
	stub_type stub_ptr;
	
	//calls the function
	template <class T, void (T::*TMethod)(char *,int)>
	inline static void method_stub(void* object_ptr,char *pointer,int a1){
		T* p= static_cast<T*>(object_ptr);
		return (p->*TMethod)(pointer,a1);
	}	
public:

	void* object_ptr;
	
	//creates a new delegate and sets object pointer and the stub pointer
	template <class T, void (T::*TMethod)(char *,int)>
	inline static delegate from_method(T* object_ptr){
		delegate d;
		d.object_ptr = object_ptr;
		d.stub_ptr = &method_stub<T, TMethod>;

		return d;
	
	}
	
	//overloaded operator () to call the function using the delegate like this d_object(interger_number)
	inline void operator()(char *pointer, int a1) const{
	      return (*stub_ptr)(object_ptr, pointer, a1);
   	}
};

#endif


