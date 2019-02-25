#include "array.h"

CS225::Array::Array(int * array, 
		unsigned int _size, 
		const ElementFactory* _pElementFactory
		) 
	:  count(new int(1)), data(new AbstractElement*[_size]),size(_size),
		pElementFactory(_pElementFactory)
	
{

	pElementFactory = _pElementFactory;
	for (unsigned int i=0;i<size;++i) {
		data[i] = pElementFactory->MakeElement( 1, array[i] );
	}
}

int CS225::Array::Get(unsigned int pos) const { return data[pos]->Get(); }

void CS225::Array::Set(int id, int pos, int value) { 
	//setter will delete the old and create new 
	//slightly inefficient if new and old have the same type
	//need deep copy once
	DeepCopy();
	delete data[pos];
	data[pos] = pElementFactory->MakeElement(id,value); 
}

void CS225::Array::Print() const {
	for (unsigned int i=0;i<size;++i) data[i]->Print(); 
	std::cout << std::endl;
}
//lazy copy, just count ++
CS225::Array::Array(Array const& a):count(a.count) , data(a.data), size(a.size), pElementFactory(a.pElementFactory){

	 ++(*count);
 }

//Assignmen, need check if the data is just used in self
CS225::Array& CS225::Array::operator= (const Array & a) {
	if (*count == 1)
	{
		DeleteData();
	}
	else
	{
		--*count;
	}
	
	size = a.size;
	data = a.data;
	pElementFactory = a.pElementFactory;
	count = a.count;
	++*count;
	return *this;
}

CS225::Array::~Array() {
	if (*count == 1)
	{
		DeleteData();
	}
	else
	{
		--*count;
	}

}

//deep copy
void CS225::Array::DeepCopy() {
	if (*count == 1)
	{
		return;
	}
	--*count;
	count = new int(1);
	AbstractElement** temp = data;
	data = new AbstractElement*[size];
	for (size_t i = 0; i < size; ++i)
	{
		data[i] = temp[i]->Clone();
	}
}

//delete all data
void CS225::Array::DeleteData() {
	
	for (size_t i = 0; i < size; ++i)
	{
		delete (data[i]);
	}
	delete[] data;
	delete count;
}