/*
 * GpioPinEventListener.h
 *
 *  Created on: Jan 7, 2010
 *      Author: fgervais
 */

#ifndef GPIOPINEVENTLISTENER_H_
#define GPIOPINEVENTLISTENER_H_

class GpioPin;

class GpioPinEventListener {
public:
	GpioPinEventListener();
	virtual ~GpioPinEventListener();

	virtual void stateChanged(GpioPin* pin) =0;
};

#endif /* GPIOPINEVENTLISTENER_H_ */
