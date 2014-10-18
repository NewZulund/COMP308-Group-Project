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
	model->generatePlanetSurface(1000);
	//model->ReadTexture("images/heightMap.jpg");
	//model->CreateGLPolyGeometry();
}

void Planet::draw(){


	glPushMatrix();
	GLfloat land_specular[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat land_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat land_shininess[] = { 25.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, land_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, land_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, land_shininess);

	glScalef(diameter / 2, diameter /2, diameter /2);
	model->RenderGeometry();

	//Render Water
	glUseProgram(0);
	glColor3f(0,0,0.8f);
	GLfloat water_specular[] = { 0.000, 0.000, 0.000, 1.0 };
	GLfloat water_diffuse[] = { 0.0, 7.0, 0.0, 1.0 };
	GLfloat water_shininess[] = { 25.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, water_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, water_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, water_shininess);

	glColor4f(0.5f,0.5f,1.0f, 0.9f);
	glutSolidSphere((diameter / 10.05f), 100, 100);


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

