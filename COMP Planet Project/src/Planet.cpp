/*
 * Planet.cpp
 *
 *  Created on: 17/09/2014
 *      Author: blomfiisaa
 */

#include "Planet.h"

Planet::~Planet() {
	// TODO Auto-generated destructor stub
}

Planet::Planet(float diam, float weig, float xP, float yP) {
	//loat diameter, float weight, float xPos, float yPos
	// TODO Auto-generated constructor stub
}

void Planet::draw(){

	glutSolidSphere(diameter/2,20, 20);
}
float Planet::getDiameter(){

	//TODO
	return 1.0f;
}
float Planet::getWeight(){

	//TODO
	return 1.0f;
}
float Planet::getPosition(){

	//TODO
	return 1.0f;
}
G308_Point Planet::getVelocity(){

	//TODO
	return G308_Point();
}

