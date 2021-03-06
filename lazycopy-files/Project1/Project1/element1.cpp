#include "element1.h"

CS225::Element1::Element1(int _val) : val(_val) { }

int CS225::Element1::Get() const { return val; }

void CS225::Element1::Set(int new_val) { val=new_val; }

void CS225::Element1::Print() const { 
  std::cout << "[" << Get() << "] "; 
}

CS225::Element1::Element1(Element1& a):val ( a.val) {
	
}

//get a new instance of self
 CS225::Element1* CS225::Element1::Clone() {
	 Element1* t = new Element1(val);
	 return t;
}