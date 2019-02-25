#include <iostream>
#include"vector.h"
namespace CS225 {
	/* this is the only place where dimension is used */
	std::ostream& operator<<(std::ostream &out, const SparseVector&v) {
		int i, last_pos = -1;
		ElementNode* p_e = v.pHead;
		while (p_e) {
			for (i = last_pos + 1; i < p_e->pos; ++i) out << " " << "0";
			out << " " << p_e->data;
			last_pos = p_e->pos;
			p_e = p_e->next;
		}
		for (i = last_pos + 1; i < v.dimension; ++i) out << " " << "0";

		return out;
	}

	SparseVector::SparseVector():pHead(NULL), dimension (0){

	}

	SparseVector::SparseVector(int dim): pHead(NULL), dimension(dim){

	}

	SparseVector::SparseVector(const SparseVector &c): pHead(NULL), dimension(0) {
		//printf("cc");
		dimension = c.dimension;
		pHead = NULL;
		ElementNode* d = c.pHead;
		while (d != NULL)
		{
			Insert(d->data, d->pos);
			d = d->next;
		}
	}

	const SparseVector &  SparseVector::operator=(const SparseVector &c) {
		if (&c == this)
		{
			return *this;
		}
		ElementNode* temp = this->pHead;

		while (pHead != NULL && pHead->next != NULL)
		{
			temp = pHead;
			pHead = pHead->next;
			delete(temp);

			/*temp = node->next;
			delete(node);
			node = temp;*/
		}
		delete(pHead);
		pHead = NULL;

		dimension = c.dimension;
		pHead = NULL;
		ElementNode* d = c.pHead;
		while (d != NULL)
		{
			Insert(d->data, d->pos);
			d = d->next;
		}
		return *this;
	}

	SparseVector::~SparseVector() {
		ElementNode* temp = pHead;

		while (pHead != NULL && pHead->next != NULL)
		{
			temp = pHead;
			pHead = pHead->next;
			delete(temp);

			/*temp = node->next;
			delete(node);
			node = temp;*/
		}
		delete(pHead);
		pHead = NULL;

	}

	void SparseVector::PrintRaw() const { //used for grading
		ElementNode* curr = pHead;
		std::cout << "Raw vector: ";
		while (curr) {
			std::cout << "(" << curr->data << ", " << curr->pos << ")";
			curr = curr->next;
		}
		std::cout << std::endl;
	}

	void SparseVector::Insert(int val, long pos) {
		if (val == 0) { Delete(pos); return; } //you may change this line
		if (pos >= dimension) {
			dimension = pos + 1; // automatically set dimension (it effects operator<< only)
		}
		ElementNode*  currentNode = pHead;
		ElementNode*  lastNode = pHead;
		ElementNode* currentNode1;
		int flag = -1;
		do
		{
			if (currentNode == 0)
			{
				/*list tail*/
				flag = 1;
				break;
			}

			if (currentNode->pos == pos)
			{
				/*pos is a node*/
				flag = 0;
				break;
			}
			else
			{
				if (currentNode->pos > pos)
				{
					break;
				}
			}
			lastNode = currentNode;
			currentNode = currentNode->next;
		} while (1);



		if (flag == 0)
		{
			if (currentNode->data == 0) {
				Delete(pos);
				return;
			}
			else
			{
				currentNode->data = val;
				if (val == 0)
				{
					Delete(pos);
				}
				return;
			}
		}
		else if (flag == 1)
		{
			if (pHead == NULL)/*new head*/
			{
				if (val == 0)
				{
					return;
				}
				pHead =new ElementNode();
				pHead->data = val;
				pHead->pos = pos;
				pHead->next = NULL;
				if (val == 0)
				{
					Delete(pos);
				}
			}
			else/*new tail*/
			{

				currentNode = 0;
				currentNode1 = new ElementNode();
				currentNode1->data = val;
				currentNode1->pos = pos;
				currentNode1->next = NULL;
				lastNode->next = currentNode1;
				if (val == 0)
				{
					Delete(pos);
				}
			}


		}
		else /*create new node*/
		{
			ElementNode* x = new ElementNode();
			if (x == NULL)
			{
				return;
			}

			x->pos = pos;
			x->data = val;
			x->next = currentNode;
			if (lastNode == pHead && lastNode == currentNode)/*insert at head*/
			{
				pHead = x;
			}
			else
			{
				lastNode->next = x;
			}

			if (val == 0)
			{
				Delete(pos);
			}
			return;
		}

		return;



	}

	void SparseVector::Delete(long pos) {

		ElementNode* currentNode = pHead;
		ElementNode* lastNode = 0;
		while (currentNode != NULL)
		{
			if (currentNode->pos == pos) {
				if (lastNode == 0 && currentNode == pHead)/*delete head */
				{
					pHead = currentNode->next;
					delete(currentNode);
					/*lastNode = NULL;*/
					return;
				}
				else
				{

					lastNode->next = currentNode->next;
					delete(currentNode);
					/*currentNode = NULL;*/
					return;
				}
			}
			else
			{
				lastNode = currentNode;
				currentNode = currentNode->next;
			}
		}
	}

	int SparseVector::Get(long pos) const {
		ElementNode* d = pHead;
		while (d != NULL) {
			if (d->pos < pos)
			{
				/*d = d->next;*/
				d = d->next;
			}
			else if (d->pos > pos)
			{
				return 0;
			}
			else {
				return d->data;
			}
		}
		return 0;
	}

   ElementProxy SparseVector::operator[](long pos) {

		ElementProxy t(*this, pos);
		return t;
	}
   ElementProxy SparseVector::operator[](long pos) const {

	   ElementProxy t(const_cast<SparseVector&>(*this), pos);
	   return t;
   }
	const SparseVector operator+(const SparseVector &a, const SparseVector &b){
		ElementNode* l2 = a.pHead;
		ElementNode* l1 = b.pHead;

		SparseVector x ;
		while (l1 != NULL && l2 != NULL)
		{
			if (l1->pos == l2->pos)
			{
				if (l1->data + l2->data != 0)
				{
					x.Insert(l1->data + l2->data, l1->pos);
				}

				l1 = l1->next;
				l2 = l2->next;
			}
			else if (l1->pos > l2->pos)
			{

				x.Insert(l2->data, l2->pos);
				l2 = l2->next;
			}
			else
			{

				x.Insert(l1->data, l1->pos);
				l1 = l1->next;
			}

		}

			while (l2 != NULL) {

				x.Insert(l2->data, l2->pos);
				l2 = l2->next;
				x.dimension = a.dimension;
			}


			while (l1 != NULL) {
				x.Insert(l1->data, l1->pos);
				l1 = l1->next;
				x.dimension = b.dimension;
			}

		return x;
	}

	const SparseVector operator * (const int& a , const SparseVector &b) {
		ElementNode* d = b.pHead;
		SparseVector c ;
		while (d != NULL)
		{
			
			c.Insert(d->data*a, d->pos);
			d = d->next;
		}
		c.dimension = b.dimension;
		return c;
	}

	const SparseVector operator * (SparseVector const&b,const int &a) {
		//SparseVector c* = b * a;
		return a*b;
	}

	int operator * (const SparseVector & a, const SparseVector & b) {
		int sum = 0;
		ElementNode* l1 = a.pHead;
		ElementNode* l2 = b.pHead;
		while (l1 != NULL && l2 != NULL)
		{
			if (l1->pos == l2->pos)
			{
				sum += l1->data* l2->data;
				l1 = l1->next;
				l2 = l2->next;
			}
			else if (l1->pos >= l2->pos)
			{
				l2 = l2->next;
			}
			else
			{
				l1 = l1->next;
			}
		}
		return sum;
	}

	


	//ElementProxy part

	ElementProxy::operator int() const {
		int t = this->v.Get(pos);
		return t;
	}
	
	ElementProxy& ElementProxy::operator =(int vlaue) {
		this->v.Insert(vlaue, pos);
		return *this;
	}
	const ElementProxy&  operator -= (const ElementProxy & a, const int & b) {
		a.v.Insert(a.v.Get(a.pos) - b, a.pos);
		return a;
	}
	
	const ElementProxy&  operator += (const ElementProxy & a, const int & b) {
		a -= -b;
		return a;
	}

	ElementProxy& ElementProxy::operator =(const ElementProxy&b) {
		this->v.Insert(b.v.Get(b.pos),this->pos);
		return *this;
	}

}