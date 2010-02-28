/*
 * Math.cpp
 *
 *  Created on: Feb 27, 2010
 *      Author: fgervais
 */

#include "Math.h"

Math::Math() {

}

Math::~Math() {

}

float Math::sin(float x) {
	float result;

	//always wrap input angle to -PI..PI
	if (x < -3.14159265)
	    x += 6.28318531;
	else
	if (x >  3.14159265)
	    x -= 6.28318531;

	//compute sine
	if (x < 0)
	{
		result = 1.27323954 * x + .405284735 * x * x;

	    if (result < 0)
	    	result = .225 * (result *-result - result) + result;
	    else
	    	result = .225 * (result * result - result) + result;
	}
	else
	{
		result = 1.27323954 * x - 0.405284735 * x * x;

	    if (result < 0)
	    	result = .225 * (result *-result - result) + result;
	    else
	    	result = .225 * (result * result - result) + result;
	}
	return result;
}
