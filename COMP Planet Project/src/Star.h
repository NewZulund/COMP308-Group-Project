/*
 * Star.h
 *
 *  Created on: 17/09/2014
 *      Author: blomfiisaa
 */

#ifndef STAR_H_
#define STAR_H_

#include "Mass.h"

class Star: public Mass {
public:
	Star(float diam, float weig, float xP, float yP);
	virtual ~Star();
	virtual void draw();
	virtual float getDiameter();
	virtual float getWeight();
	virtual G308_Point getPosition();
	virtual G308_Point getVelocity();
};

#endif /* STAR_H_ */
