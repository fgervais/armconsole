/*
 * Vector.h
 *
 *  Created on: Oct 12, 2009
 *      Author: fgervais
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "ListNode.h"

template <typename T>
class Vector {
private:
	int count;
	ListNode<T>* head;
	ListNode<T>* tail;
public:
	Vector();
	virtual ~Vector();

	void addElement(const T& element);
	T& getElement(int position);
	int size();
};

/*
 * Why are the function implementation here ?
 *
 * See : http://www.parashift.com/c++-faq-lite/templates.html
 */

template <typename T>
Vector<T>::Vector() {
	// Basic initialization
	count = 0;
	head = 0;
	tail = head;
}

template <typename T>
Vector<T>::~Vector() {
	/* Free memory */

}

/**
 * @brief Add an element to the vector.
 *
 * @param element Element to be added.
 */
template <typename T>
void Vector<T>::addElement(const T& element) {
	ListNode<T> *node = new ListNode<T>(element,0);

	// Adding the first element
	if(head == 0) {
		head = node;
		tail = head;
	}
	else {
		tail->setNext(node);
		tail = node;
	}
	count++;
}

/**
 * @brief Return the element at the specified position.
 *
 * Note 1: As this function returns a reference, it
 * cannot just return null in case the position is
 * grater than the amount of element in the vector.
 *
 * The best way to handle this would be to throw an
 * exception but this would make the executable way
 * too big. So for now it does nothing.
 *
 * Note 2: A complex and efficient algorithm could be
 * inserted here. This one is simple and is optimized
 * for sequential access like browsing the whole vector
 * with a for loop.
 *
 * @param position Position of the required element.
 */
template <typename T>
T& Vector<T>::getElement(int position) {
	static ListNode<T>* iterator = head;
	static int iteratorPosition = 0;
	
	if(position < count) {
		//TODO: Raise an exception?
	}

	if(position < iteratorPosition) {
		// Reset iterator
		iteratorPosition = 0;
		iterator = head;
	}
	
	// Browse through the linked list
	for(; iteratorPosition<position; iteratorPosition++) {
		iterator = iterator->next();
	}

	// Return a non constant type
	return const_cast<T&>(iterator->getElement());
}

/**
 * @brief Return the size of the vector.
 */
template <typename T>
int Vector<T>::size() {
	return count;
}

#endif /* VECTOR_H_ */
