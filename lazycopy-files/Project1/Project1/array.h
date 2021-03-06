#ifndef ARRAY_H
#define ARRAY_H
#include "abstract-element.h"
#include "element-factory.h"
#include <iostream>

namespace CS225 {

/*
 * Class Array implements a simple array-like data structure.
 * Objects inside the Array have to be derived from AbstractElement.
 */
	class Array {
		public:
			Array(int * array, unsigned int _size, 
					const ElementFactory* _pElementFactory
					);
			Array(Array const& a);
			~Array();
			Array& operator= (const Array & a);

			int Get(unsigned int pos) const;
			void Set(int id, int pos, int value);

			void Print() const;
		private:
			int* count;
			AbstractElement** data;
			unsigned int size;
			const ElementFactory* pElementFactory;
	private:
		void DeepCopy();
		void DeleteData();
	};
}
#endif
