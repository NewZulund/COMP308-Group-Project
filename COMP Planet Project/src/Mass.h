/*
 * Mass.h
 *
 *  Created on: 17/09/2014
 *      Author: blomfiisaa
 */

#ifndef MASS_H_
#define MASS_H_

#include "define.h"
#include "G308_Geometry.h"

class Mass {
public:
	Mass();
	virtual ~Mass();
	virtual void draw() = 0;
	virtual float getDiameter() = 0;
	virtual float getWeight() = 0;
	virtual float getPosition() = 0;
	//virtual G308_Point getVelocity() = 0;

	GLfloat diameter;
	GLfloat weight;
	GLfloat xPos;
	GLfloat yPos;
};

#endif /* MASS_H_ */
