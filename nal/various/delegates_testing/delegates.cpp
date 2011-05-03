#include <iostream>

using namespace std;

class delegate{

	typedef void (*stub_type)(void* object_ptr, int);
	
	stub_type stub_ptr;
	
	//calls the function
	template <class T, void (T::*TMethod)(int)>
	inline static void method_stub(void* object_ptr,int a1){
		T* p= static_cast<T*>(object_ptr);
		return (p->*TMethod)(a1);
	}	
public:

	void* object_ptr;
	
	//creates a new delegate and sets object pointer and the stub pointer
	template <class T, void (T::*TMethod)(int)>
	inline static delegate from_method(T* object_ptr){
		delegate d;
		d.object_ptr = object_ptr;
		d.stub_ptr = &method_stub<T, TMethod>;

		return d;
	
	}
	
	//overloaded operator () to call the function using the delegate like this d_object(interger_number)
	inline void operator()(int a1) const{
	      return (*stub_ptr)(object_ptr, a1);
   	}
};

//test classes with test member functions
class Test1{
	public:
		void to_be_registered1(int number){
			cout << "Class: Test1 function:to_be_registered1 Number is: " << number << endl;
		}
		
		void to_be_registered2(int number){
			cout << "Class: Test1 function:to_be_registered2 Number is: " << number << endl;
		}
};

class Test2{
	public:
		void to_be_registered3(int number){
			cout << "Class: Test2 function:to_be_registered3 Number is: " << number << endl;
		}
		
		void to_be_registered4(int number){
			cout << "Class: Test2 function:to_be_registered4 Number is: " << number << endl;
		}
};


int main(void){

	//create test objects
	Test1 tst1,tst2;
	Test2 tst3,tst4;

	//creating various delegates
	delegate d1 = delegate::from_method<Test1,&Test1::to_be_registered2>(&tst1);
	delegate d2 = delegate::from_method<Test2,&Test2::to_be_registered4>(&tst3);
	delegate d3 = delegate::from_method<Test1,&Test1::to_be_registered1>(&tst2);
	delegate d4 = delegate::from_method<Test2,&Test2::to_be_registered3>(&tst4);
	
	//running functions using delegates
	d1(10);
	d2(20);
	d3(30);
	d4(40);
		
	return 0;
}

