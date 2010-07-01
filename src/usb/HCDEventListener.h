/*
 * HCDEventListener.h
 *
 *  Created on: Jun 30, 2010
 *      Author: fgervais
 */

#ifndef HCDEVENTLISTENER_H_
#define HCDEVENTLISTENER_H_

class HCDRequest;

class HCDEventListener {
public:
	HCDEventListener();
	virtual ~HCDEventListener();

	virtual void transferCompleted(HCDRequest*) =0;
};

#endif /* HCDEVENTLISTENER_H_ */
