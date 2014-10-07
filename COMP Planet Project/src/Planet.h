/*
 * Planet.h
 *
 *  Created on: 17/09/2014
 *      Author: blomfiisaa
 */

#ifndef PLANET_H_
#define PLANET_H_


#include "define.h"
#include "G308_Geometry.h"
#include "G308_ImageLoader.h"
#include "Mass.h"

class Planet : public Mass{
public:
	virtual ~Planet();
	Planet(float diam, float weig, float xP, float yP);
	virtual void draw();
	virtual float getDiameter();
	virtual float getWeight();
	virtual G308_Point getPosition();
	virtual G308_Point getVelocity();
private:
	GLuint heightMap;
	texInfo HMInfo;
};

#endif /* PLANET_H_ */
