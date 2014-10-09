/*
 * Planet.cpp
 *
 *  Created on: 17/09/2014
 *      Author: blomfiisaa
 */

#include <GL/glew.h>
#include <GL/glut.h>
#include "Planet.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

G308_Geometry * model = NULL;


Planet::~Planet() {
	// TODO Auto-generated destructor stub
}

Planet::Planet(float diam, float weig) {
	//loat diameter, float weight, float xPos, float yPos
	diameter = diam;
	weight = weig;

	model = new G308_Geometry("sphere");
	model->ReadOBJ("models/sphere.obj");
	model->generatePlanetSurface(100);
	//model->ReadTexture("images/heightMap.jpg");
	//model->CreateGLPolyGeometry();
}

void Planet::draw(){
	glPushMatrix();

	glColor3f(0,1,0);
	GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 5.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


	glScalef(2,2,2);
	model->RenderGeometry();
	//glutSolidSphere(diameter/2,100, 100);

	//glutSolidSphere(50,50,50);

	glPopMatrix();
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

