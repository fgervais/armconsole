/*
 * Debug.h
 *
 *  Created on: Mar 6, 2010
 *      Author: fgervais
 */

#ifndef DEBUG_H_
#define DEBUG_H_

class Console;

class Debug {
public:
	Debug();
	virtual ~Debug();

	static void writeLine(const char* string);

private:
	static Console* console;
};

#endif /* DEBUG_H_ */
