#include <stdlib.h>
#include <stdio.h>
#include "spvector.h"



/* definition of Node is hidden from the client (driver) so that client CANNOT
 * use INTERNALS of the Node.  this is poor-man encapsulations - hiding
 * implementation details.  The main reason for encapsulations -- if I ever
 * decide to change Node struct no client code will break. Example: I decide to
 * change "left" to "Left", all I do is change map.h and map.c, no client code
 * is effected, since clients were FORCED to use getleft(...) function, rather
 * than field name "left".  Also see typedef in map.h
 */
struct ElementNode {
	int    data;
	int    pos;
	struct ElementNode* next;
};

struct RowNode {
	int pos;
	struct ElementNode* elements;
	struct RowNode* next;
};

typedef struct ElementNode ElementNode;
typedef struct RowNode RowNode;

RowNode** createNew(RowNode const* p_h, int i, int j);
int  getInRow(RowNode const* p_h, int i, int j);
/*print functions*/
void printf_elements(ElementNode const* p_e, char *fmt, int dim) {
	int i, last_pos = -1;
	while (p_e) {
		for (i = last_pos + 1; i < p_e->pos; ++i) { printf(fmt, 0); }
		printf(fmt, p_e->data);
		last_pos = p_e->pos;
		p_e = p_e->next;
	}
	for (i = last_pos + 1; i < dim; ++i) { printf(fmt, 0); }
}

void print_elements(ElementNode const* p_e) {
	while (p_e) {
		printf("%i at pos %i, ", p_e->data, p_e->pos);
		p_e = p_e->next;
	}
}

void print_rows(RowNode const* p_r) {
	while (p_r) {
		printf("row %i: ", p_r->pos);
		print_elements(p_r->elements);
		printf("\n");
		p_r = p_r->next;
	}
}

void printf_rows(RowNode const* p_r, char *fmt, int dim) {
	int i, j, last_pos = -1;
	while (p_r) {
		for (i = last_pos + 1; i < p_r->pos; ++i) { /* output row of 0's */
			for (j = 0; j < dim; ++j) printf(fmt, 0);
			printf("\n");
		}
		printf_elements(p_r->elements, fmt, dim);
		printf("\n");
		last_pos = p_r->pos;
		p_r = p_r->next;
	}
	for (i = last_pos + 1; i < dim; ++i) { /* output row of 0's */
		for (j = 0; j < dim; ++j) printf(fmt, 0);
		printf("\n");
	}
}


int insert_element(ElementNode* * p_p_h, int pos, int val) {

	ElementNode*  currentNode = *p_p_h;
	ElementNode*  lastNode = *p_p_h;
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
			delete_element(p_p_h, pos);
			return 0;
		}
		else
		{
			currentNode->data = val;
			if (val == 0)
			{
				delete_element(p_p_h, pos);
			}
			return 0;
		}
	}
	else if (flag == 1)
	{
		if (*p_p_h == NULL)/*new head*/
		{
			if (val == 0)
			{
				return 0;
			}
			(*p_p_h) = (ElementNode*)malloc(sizeof(struct ElementNode));
			(*p_p_h)->data = val;
			(*p_p_h)->pos = pos;
			(*p_p_h)->next = NULL;
			if (val == 0)
			{
				delete_element(p_p_h, pos);
			}
		}
		else/*new tail*/
		{

			currentNode = 0;
			currentNode1 = (ElementNode*)malloc(sizeof(struct ElementNode));
			currentNode1->data = val;
			currentNode1->pos = pos;
			currentNode1->next = NULL;
			lastNode->next = currentNode1;
			if (val == 0)
			{
				delete_element(p_p_h, pos);
			}
		}


	}
	else /*create new node*/
	{
		ElementNode* x = (ElementNode*)malloc(sizeof(struct ElementNode));
		if (x == NULL)
		{
			return 1;
		}

		x->pos = pos;
		x->data = val;
		x->next = currentNode;
		if (lastNode == *p_p_h && lastNode == currentNode)/*insert at head*/
		{
			*p_p_h = x;
		}
		else
		{
			lastNode->next = x;
		}

		if (val == 0)
		{
			delete_element(p_p_h, pos);
		}
		return 0;
	}

	return 0;
}

void delete_element(ElementNode* * p_p_h, int pos) {

	ElementNode* currentNode = *p_p_h;
	ElementNode* lastNode =0;
	while (currentNode != NULL)
	{
		if (currentNode->pos == pos) {
			if (lastNode == 0  && currentNode == *p_p_h)/*delete head */
			{
				*p_p_h= currentNode ->next;
				free(currentNode);
				/*lastNode = NULL;*/
				return;
			}
			else
			{

				lastNode->next = currentNode->next;
				free(currentNode);
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


int  get(ElementNode const* p_h, int pos) {
	
	/*ElementNode* d = p_h;*/
	while (p_h != NULL) {
		if (p_h->pos < pos)
		{
			/*d = d->next;*/
			p_h = p_h->next;
		}
		else if (p_h->pos > pos)
		{
			return 0;
		}
		else {
			return p_h->data;
		}
	}
	return 0;
}



void free_elements(ElementNode* node) {
	ElementNode* temp = node;

	while (node != NULL && node->next != NULL)
	{
		temp = node;
		node = node->next;
		free(temp);

		/*temp = node->next;
		free(node);
		node = temp;*/
	}
	free(node);
	node = NULL;
}


int scalar_product(ElementNode const* l1, ElementNode const* l2) {

	int sum = 0;

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

ElementNode* add(ElementNode const* l1, ElementNode const* l2) {
	ElementNode** tail ;
	ElementNode* newElement;
	ElementNode* head;
	/*tail = (ElementNode**)malloc(sizeof(ElementNode*));*/
	tail = NULL;
	newElement = 0;
	head = 0;
	while (l1 != NULL && l2 != NULL)
	{
		if (l1->pos == l2->pos)
		{
			if (l1->data + l2->data != 0)
			{
				newElement = (ElementNode*)malloc(sizeof(ElementNode));
				newElement->data = l1->data + l2->data;
				newElement->pos = l1->pos;
				newElement->next = NULL;
				/*if (tail == NULL)
				{
				tail = newElement;
				}
				else
				{
				tail->next = newElement;
				}*/
				if (tail == NULL)
				{
					tail = &newElement;
				}
				else
				{
					*tail = newElement;
				}
				
				tail = &(*tail)->next;
			}


			l1 = l1->next;
			l2 = l2->next;
		}
		else if (l1->pos > l2->pos)
		{
			newElement = (ElementNode*)malloc(sizeof(ElementNode));
			newElement->data = l2->data;
			newElement->pos = l2->pos;
			newElement->next = NULL;
			/*if (tail == NULL)
			{
			tail = newElement;
			}
			else
			{
			tail->next = newElement;
			tail = newElement;
			}*/
			if (tail == NULL)
			{
				tail = &newElement;
			}
			else
			{
				*tail = newElement;
			}
			tail = &(*tail)->next;

			l2 = l2->next;
		}
		else
		{

			newElement = (ElementNode*)malloc(sizeof(ElementNode));
			newElement->data = l1->data;
			newElement->pos = l1->pos;
			newElement->next = NULL;
			/*if (tail == NULL)
			{
			tail = newElement;
			}
			else
			{
			tail->next = newElement;
			tail = newElement;
			}*/
			if (tail == NULL)
			{
				tail = &newElement;
			}
			else
			{
				*tail = newElement;
			}
			tail = &(*tail)->next;
			l1 = l1->next;

		}

		if (head == NULL)
		{
			head = newElement;
		}
	}


	if (l1 == NULL && l2 != NULL)
	{
		while (l2 != NULL) {

			newElement = (ElementNode*)malloc(sizeof(ElementNode));
			newElement->data = l2->data;
			newElement->pos = l2->pos;
			newElement->next = NULL;
			/*tail->next = newElement;
			tail = newElement;*/
			*tail = newElement;
			tail = &(*tail)->next;
			l2 = l2->next;
		}
	}
	else if ((l1 != NULL && l2 == NULL))
	{
		while (l1 != NULL) {

			newElement = (ElementNode*)malloc(sizeof(ElementNode));
			newElement->data = l1->data;
			newElement->pos = l1->pos;
			newElement->next = NULL;
			/*tail->next = newElement;
			tail = newElement;*/
			*tail = newElement;
			tail = &(*tail)->next;
			l1 = l1->next;
		}
	}


	return head;
}

int insert_row(RowNode* * p_e, int pos, ElementNode* data) {
	RowNode*  currentNode = *p_e;
	RowNode*  lastNode = *p_e;
	RowNode* currentNode1;
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
			return 2;
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



	if (flag == 1)
	{
		if (*p_e == NULL)/*new head*/
		{
			(*p_e) = (RowNode*)malloc(sizeof(struct RowNode));
			if ((*p_e) == NULL)
			{
				return 1;
			}
			(*p_e)->elements = data;
			(*p_e)->pos = pos;
			(*p_e)->next = NULL;
		}
		else/*new tail*/
		{
			currentNode = 0;
			currentNode1 = (RowNode*)malloc(sizeof(struct RowNode));
			if (currentNode1 == NULL)
			{
				return 1;
			}
			currentNode1->elements = data;
			currentNode1->pos = pos;
			currentNode1->next = NULL;
			lastNode->next = currentNode1;
		}


	}
	else /*create new node*/
	{
		RowNode* x = (RowNode*)malloc(sizeof(struct RowNode));
		if (x == NULL)
		{
			return 1;
		}
		x->pos = pos;
		x->elements = data;
		x->next = currentNode;
		if (lastNode == *p_e && currentNode == *p_e)/*insert at head*/
		{
			*p_e = x;
		}
		else
		{
			lastNode->next = x;
		}

		return 0;
	}

	return 0;

}

void free_rows(RowNode* p_e) {

	RowNode* temp = p_e;
	while (p_e != NULL && p_e->next != NULL)
	{

		temp = p_e;
		p_e = p_e->next;
		free_elements(temp->elements);
		free(temp);

		/*temp = node->next;
		free(node);
		node = temp;*/

	}
	free(p_e);
	p_e = NULL;
}

int insert_element2(RowNode* * p_e, int pos_col, int pos_row, int val) {

	RowNode* currentNode = *p_e;
	RowNode* lastNode = *p_e;
	int index = 0;
	int x = 0;
	do {
		if (*p_e == NULL)
		{
			/*new head*/
			index = 1;
			break;
		}


		if (currentNode != NULL)
		{
			if (currentNode->pos == pos_col)
			{
				/*have col, insert*/
				index = 2;
				break;
			}
			else if (currentNode->pos > pos_col) /*insert new */
			{
				index = 3;
				break;
			}
			else
			{
				lastNode = currentNode;
				currentNode = currentNode->next;
			}
		}
		else /*insert at tail*/
		{
			index = 4;
			break;
		}

	} while (p_e != NULL);

	if (index == 1)
	{
		RowNode* temp = (RowNode*)malloc(sizeof(struct RowNode));
		if (temp == NULL)
		{
			return 1;
		}
		temp->elements = (ElementNode *)malloc(sizeof(ElementNode));
		if (temp->elements == NULL)
		{
			return 1;
		}
		temp->elements = 0;
		temp->pos = pos_col;
		temp->next = NULL;

		x = insert_element(&temp->elements, pos_row, val);
		*p_e = temp;
		return x;
	}
	else if (index == 2)
	{
		x = insert_element(&currentNode->elements, pos_row, val);
		if (currentNode->elements == NULL)
		{
			if (lastNode == *p_e && currentNode == *p_e)/*delete head*/
			{
				*p_e = currentNode->next;
				free(currentNode);
			}
			else
			{
				lastNode->next = currentNode->next;
				free(currentNode);
			}
		  
		}
		return x;
	}
	else if (index == 3)
	{
		RowNode* temp = (RowNode*)malloc(sizeof(struct RowNode));
		if (temp == NULL)
		{
			return 1;
		}
		temp->elements = (ElementNode *)malloc(sizeof(ElementNode));
		if (temp->elements == NULL)
		{
			return 1;
		}
		temp->elements = 0;
		temp->pos = pos_col;
		temp->next = NULL;
		x = insert_element(&temp->elements, pos_row, val);

		if (temp->elements == NULL)
		{
			return 0;
		}
	
		if (lastNode == *p_e && lastNode == currentNode)
		{
			*p_e = temp;
			temp->next = currentNode;
		}
		else
		{
			lastNode->next = temp;
			temp->next = currentNode;
		}
		return x;
	}
	else if (index == 4)
	{
		RowNode* temp = (RowNode*)malloc(sizeof(struct RowNode));
		if (temp == NULL)
		{
			return 1;
		}
		temp->elements = (ElementNode *)malloc(sizeof(ElementNode));
		if (temp->elements == NULL)
		{
			return 1;
		}
		temp->elements = 0;
		temp->pos = pos_col;
		temp->next = NULL;
		x = insert_element(&temp->elements, pos_row, val);
		if (temp->elements == NULL)
		{
			return 0;
		}		
		lastNode->next = temp;
		return x;
	}


	return x;
}

RowNode* transpose(RowNode const* pr) {
	
	RowNode* x =0;
	while (pr != NULL)
	{
		ElementNode* y = pr->elements;
		while (y != NULL)
		{
 			insert_element2(&x,y->pos,pr->pos,y->data);
 			y = y->next;
		}
		pr = pr->next;
	}
	return x;
}

RowNode* mult(RowNode const* h1, RowNode const* h2) {
	
	RowNode* h2Transpose = transpose(h2);
	RowNode* h2TransposeBegin = h2Transpose;
	RowNode* x = 0;
	int product;

	/*scalar_product*/
	while (h1 != NULL)
	{

		while (h2Transpose != NULL) {
			product = scalar_product(h1->elements, h2Transpose->elements);
			insert_element2(&x, h1->pos, h2Transpose->pos, product);
			h2Transpose = h2Transpose->next;
		}
		h2Transpose = h2TransposeBegin;
		h1 = h1->next;
	}

	return x;
}


int  getInRow(RowNode const* p_h, int i, int j) {

	/*ElementNode* d = p_h;*/
	while (p_h != NULL) {
		if (p_h->pos < i)
		{
			/*d = d->next;*/
			p_h = p_h->next;
		}
		else if (p_h->pos > i)
		{
			return 0;
		}
		else {
			return get(p_h->elements, j);
		}
	}
	return 0;
}

RowNode** createNew(RowNode const* p_h, int i, int j) {
	RowNode** d = (RowNode**)malloc(sizeof(RowNode*));
	RowNode const* h ;
	ElementNode* dd ;
	int x;
	int y;
	h = p_h;
	dd = NULL;
	*d = NULL;
	while (h != NULL) {
		dd = h->elements;
		if (h->pos != i)
		{			
			while (dd != NULL)
			{
				if (dd ->pos != j)
				{
					if (h->pos > i)
					{
						x = h->pos -1;
					}
					else
					{
						x = h->pos;
					}
					if (dd->pos>j)
					{
						y = dd->pos - 1;
					}
					else
					{
						y = dd->pos ;
					}
					insert_element2(d, x, y, dd->data);
				}
				dd = dd->next;
			}
		}
		h = h->next;
	}
	return d;
}

/*
 * Calculates the determinant of the list of RowNodes viewed as a matrix.
 * Dimension is required!
 * extra credit, discuss algorithm with me first */
 int  determinant(RowNode const* p, int n) {
	int a, b, c, d;
 	int f = 1;
   	int sum = 0;
	int  i;
	RowNode* h;
	if (n==2)
	{
		 a = getInRow(p, 0, 0);
		 b = getInRow(p, 0, 1);
		 c = getInRow(p, 1, 0);
		 d = getInRow(p, 1, 1);
		 sum = a * d - b * c;
		 return sum;
	}
	else
	{
		
		for ( i = 0; i < n; i++)
		{
			if ( i %2 ==1)
			{
				f = -1;
			}
			else
			{
				f = 1;
			}
		/*	printf("当前Matrix, 第 %d 阶  \n",n);
			printf_rows(p, "%i ", n); printf("\n");*/
		    h = *createNew(p, 0, i);
			/*printf("%d 阶 %d 个元素的Matrix  \n", n, i);
			printf("Matrix  \n");
			printf_rows(h, "%i ", n-1); printf("\n");*/
			sum += f*getInRow(p, 0, i)*determinant(h,n-1);
			/*printf("当前已经求的和为 %d  \n", sum); */
			free(h);
		}
	}
	return sum;
}



