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
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 5.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);



	//Generate Random Texture
	GLuint heightMap;



	glEnable(GL_TEXTURE_2D);


	glBindTexture(GL_TEXTURE_2D, heightMap);



	glutSolidSphere(diameter/2,100, 100);
}

//G308 read texture method - Written by TJ
void Planet::ReadTexture(const char* filename) {
	char file[50];
	strncpy(file, filename, 51);

	unsigned int i;
	for (i = 0; i < strlen(filename); i++) {
		if (filename[i] == '.') {
			break;
		}
	}

	char extension[5];
	strcpy(extension, &filename[i + 1]);
	if (strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0)
		loadTextureFromJPEG(file, &HMInfo);
	else {
		printf("Invalid format. Only supports JPEG and PNG.\n");
		exit(1);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &heightMap);
	glBindTexture(GL_TEXTURE_2D, heightMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Only useful for PNG files, since JPEG doesn't support alpha
	if (HMInfo.hasAlpha) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, HMInfo.width, HMInfo.height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, HMInfo.textureData);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, HMInfo.width, HMInfo.height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, HMInfo.textureData);
	}
	//Once the texture has been loaded by GL, we don't need this anymore.
	free(HMInfo.textureData);

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

