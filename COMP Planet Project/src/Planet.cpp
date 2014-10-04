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
	diameter = diam;
	weight = weig;
	xPos = xP;
	yPos = yP;
}

void Planet::draw(){
	glColor3f(1,1,1);
	glutSolidSphere(diameter/2,20, 20);
}
float Planet::getDiameter(){
	return diameter;
}
float Planet::getWeight(){
	return weight;
}
G308_Point Planet::getPosition(){
	G308_Point p = {xPos,yPos, 0};
	return p;
}
G308_Point Planet::getVelocity(){
	//TODO velocity
	return G308_Point();
}

