#pragma once
#include"CRTP.h"

namespace CS225 {
class AbstractElement: public  ObjectCounter< AbstractElement>{
public:
	virtual int Get() const = 0;
	virtual void Set(int new_val) = 0;
	virtual void Print() const = 0;
	virtual AbstractElement* Clone()=0;//virtual constructor
};
}