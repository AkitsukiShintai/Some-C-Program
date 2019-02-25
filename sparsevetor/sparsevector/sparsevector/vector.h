#pragma once
#ifndef VECTOR_H
#define VECTOR_H
namespace CS225 {
	
	class ElementProxy;
	struct ElementNode {
		int data;
		int pos;
		struct ElementNode *next;
	};
	//forward declaration
	//std::ostream& operator<<(std::ostream &out, const SparseVector&v);
	//class ElementProxy;


	//ElementProxy::ElementProxy(SparseVector& v, long pos) 

	class SparseVector {
	private:
		ElementNode* pHead;
		long dimension;
	public:
		SparseVector();
		SparseVector(int);
		SparseVector(const SparseVector &c);
		~SparseVector();
		int Get(long pos) const;
		void Insert(int val, long pos);
		void Delete(long pos);
		void PrintRaw() const;
		friend std::ostream& operator<<(std::ostream &out, const SparseVector&v);//implemented		
		//friend std::ostream& operator<<(std::ostream &out, const SparseVector&v);
	     ElementProxy operator[](long pos);
		 ElementProxy operator[](long pos) const;
		 const SparseVector &  operator=(const SparseVector &);
		// ElementProxy operator[](long pos) const;
		friend const SparseVector  operator+(const SparseVector &, const SparseVector &);
		friend const SparseVector operator * (const int&, const SparseVector &);
		friend int  operator * (const SparseVector &, const SparseVector &);
		//friend const ElementProxy  operator -= (const SparseVector &, const SparseVector &);
		//SparseVector& operator*(SparseVector&,int);
			//......	
	};
	const SparseVector  operator+(const SparseVector &, const SparseVector &);

	const SparseVector operator * (const int&, const SparseVector &);
	const SparseVector operator * (const SparseVector &, const int&);
	int  operator * (const SparseVector &, const SparseVector &);

	//const ElementProxy  operator -= (const ElementProxy &, const SparseVector &);

	class ElementProxy {
	public:
		ElementProxy( SparseVector& v, long pos) :v(v), pos(pos) {};
		//ElementProxy(const SparseVector& v, long pos) :v(const_cast<SparseVector&> (v)), pos(pos);

		operator int() const;
		friend const ElementProxy&  operator -= (const ElementProxy & a, const int & b);
		 friend const ElementProxy&  operator += (const ElementProxy & a, const int & b);

		ElementProxy& operator =(int);
		ElementProxy& operator =(const ElementProxy&);
		

		// .......
	private:
		SparseVector& v;
		long pos;

	};
	const ElementProxy&  operator -= (const ElementProxy & a, const int & b);
    const ElementProxy&  operator += (const ElementProxy & a, const int & b);

}

#endif // !VECTOR_H

