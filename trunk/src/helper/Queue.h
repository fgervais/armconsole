/*
 * Queue.h
 *
 *  Created on: Mar 31, 2009
 *      Author: fgervais
 */

#ifndef QUEUE_H_
#define QUEUE_H_

template <typename T>
class Queue {
private:
	T* array;
	int count;
	int head;
	int tail;
	int size;
public:
	int addElement(const T& element);
	T& getElement();
	int isEmpty();
	int isFull();
	Queue(int size);
	virtual ~Queue();
};

/*
 * Why are the function implementation here ?
 *
 * See : http://www.parashift.com/c++-faq-lite/templates.html
 */

template <typename T>
Queue<T>::Queue(int size) {
	/* Create the queue internal array of constant pointers */
	array = new T[size];

	// Var init
	count = 0;
	head = 0;
	tail = 0;
	this->size = size;

}

template <typename T>
Queue<T>::~Queue() {
	/* Free memory */
	delete[] array;
}

/**
 * Add one element in the queue.
 *
 * Return 0 if data is successfully added.
 * Return 1 otherwise.
 */
template <typename T>
int Queue<T>::addElement(const T& element) {
	if(count < size) {
		count++;
		tail++;
		tail %= size;
		array[tail] = element;
		return 0;
	}
	else {
		return 1;
	}
}

template <typename T>
T& Queue<T>::getElement() {
	if(count > 0) {
		count--;
		head++;
		head %= size;
	}
	// Return a reference to the actual element
	return array[head];
}

template <typename T>
int Queue<T>::isEmpty() {
	if(count == 0) {
		return 1;
	} else {
		return 0;
	}
}

template <typename T>
int Queue<T>::isFull() {
	if(count == size) {
		return 1;
	} else {
		return 0;
	}
}

#endif /* QUEUE_H_ */
