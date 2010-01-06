/*
 * ListNode.h
 *
 *  Created on: Oct 12, 2009
 *      Author: fgervais
 */

#ifndef LISTNODE_H_
#define LISTNODE_H_

template <typename T>
class ListNode {
private:
	T element;
	ListNode<T>* nextNode;
public:
	ListNode(const T& element, ListNode<T>* next);
	virtual ~ListNode();

	ListNode<T>* next();
	void setNext(ListNode<T>* next);
	const T& getElement();
};

/*
 * Why are the function implementation here ?
 *
 * See : http://www.parashift.com/c++-faq-lite/templates.html
 */

template <typename T>
ListNode<T>::ListNode(const T& element, ListNode<T>* next) {
	this->element = element;
	this->nextNode = next;
}

template <typename T>
ListNode<T>::~ListNode() {

}

template <typename T>
ListNode<T>* ListNode<T>::next() {
	return nextNode;
}

template <typename T>
void ListNode<T>::setNext(ListNode<T>* next) {
	this->nextNode = next;
}

template <typename T>
const T& ListNode<T>::getElement() {
	return element;
}

#endif /* LISTNODE_H_ */
