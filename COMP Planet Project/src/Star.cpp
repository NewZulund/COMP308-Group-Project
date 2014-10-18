/*
 * Star.cpp
 *
 *  Created on: 17/09/2014
 *      Author: blomfiisaa
 */

#include "Star.h"

int winWidth = 0;
int winHeight = 0;

Star::Star(float diam, float weig) {
	diameter = diam;
	weight = weig;
}

Star::~Star() {

}

void Star::draw(){
	glutSolidSphere(2, 30, 30);
}

void Star::draw (int windowWidth, int windowHeight){
	winWidth = windowWidth;
	winHeight = windowHeight;

	initialize();

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);//Set the clear color to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();//Reset view
	renderToTexture();

	processStar();//Draw the star on top of the blur

	drawBlur(25, 0.02f);//Draw the blur effect

	glFlush();

	deInitialize();
}

void Star::renderToTexture(){
	glViewport(0, 0, 128, 128);//Set viewport to match texture size

	processStar();//Render star

	glBindTexture(GL_TEXTURE_2D, blurTexture);

	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, 0, 128, 128, 0);//Copy ViewPort to blur texture (from 0, 0 to 128, 128, no border)

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//Set the clear color to black. (0, 0, 0.5, 0.5) is medium blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, winWidth, winHeight);
}

void Star::processStar(){
	GLfloat glfMaterialColor[]={0.3f,0.8f,0.3f,0.5f};
	GLfloat specular[]={0.5f,0.5f,0.5,0.5f};

	glLoadIdentity();//Reset the ModelView matrix
	gluLookAt(0, 5, 200, 0, 0, 0, 0, 1, 0);//Eye position (0,5,50) Center of scene (0,0,0) Up on y-axis.
	glPushMatrix();//Push the ModelView matrix

	//do translations and rotations
	//	glTranslatef(0,0,-50);                      // Translate 50 Units Into The Screen
	//	glRotatef(angle/2.0f,1,0,0);                    // Rotate By angle/2 On The X-Axis
	//	glRotatef(angle/3.0f,0,1,0);                    // Rotate By angle/3 On The Y-Axi

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, glfMaterialColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	glutSolidSphere(10, 100, 100);

	glPopMatrix();
}

void Star::drawBlur(int times, float inc){
	float spost = 0.0f;//Starting texture coordinate offset
	float alphainc = 0.9/times;//Fade speed for alpha blending
	float alpha = 0.2f;//Starting alpha value

	//Disable autotexture coordinates
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_2D);//Enable 2D texture mapping
	glDisable(GL_DEPTH_TEST);//Disable depth testing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);//Set blending mode
	glEnable(GL_BLEND);//Enable blending
	glBindTexture(GL_TEXTURE_2D, blurTexture);

	orthoView();

	alphainc = alpha/times;

	glBegin(GL_QUADS);
	for(int num=0; num<times; num++){
		glColor4f(1.0f, 1.0f, 1.0f, alpha);
		glTexCoord2f(0+spost,1-spost);//texture coordinate (0, 1)
		glVertex2f(0,0);//first vertex (0, 0)

		glTexCoord2f(0+spost,0+spost);//texture coordinate (0, 0)
		glVertex2f(0, winHeight);//second vertex (0, windowHeight)

		glTexCoord2f(1-spost,0+spost);//texture coordinate (1, 0)
		glVertex2f(winWidth, winHeight);//third vertex

		glTexCoord2f(1-spost,1-spost);//texture coordinate (1, 1)
		glVertex2f(winWidth, 0);//fourth vertex (windowWidth, 0)

		spost += inc;
		alpha = alpha - alphainc;
	}
	glEnd();

	perspectiveView();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);//Disable 2D texture mapping
	glDisable(GL_BLEND);//Disable blending
	glBindTexture(GL_TEXTURE_2D, 0);//Unbind blurTexture
}

//Returns the texture number
GLuint Star::emptyTexture(){
	GLuint texNum;//texture ID
	unsigned int* data;//texture data

	//Create storage space for texture data. 128*128 is the width*height size of the texture. Multiply by 4*(unsigned int) for (x,y,z,a)
	data = (unsigned int*) calloc(((128*128)*4*sizeof(unsigned int)), 1);

	glGenTextures(1, &texNum);//Create a texture
	glBindTexture(GL_TEXTURE_2D, texNum);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//Build texture using information in data (?, ?, internalFormat: , width, height, no border, format: The format of the texel data. Must match internalFormat, pointer to the image data)

	//Will be stretching the image so changing magnificatoin and minification to GL_LINEAR. GL_NEAREST looks bad stretched
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete[] data;

	return texNum;
}

void Star::orthoView(){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();//Reset the Projection matrix
	glOrtho(0, winWidth, winHeight, 0, -1, 1);//Select orthogonal mode (width*height)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();//Reset the ModelView matrix
}

void Star::perspectiveView(){
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


int Star::initialize(){
	angle = 0.0f;

	blurTexture = emptyTexture();

	glViewport(0, 0, winWidth, winHeight);//Set up a Viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//Reset Projection matrix
	gluPerspective(50, (float)winWidth/(float)winHeight, 5, 2000);//Set up perspective
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//Reset ModelView matrix

	glEnable(GL_DEPTH_TEST);//Enable depth testing

	GLfloat global_ambient[4]={0.2f, 0.2f,  0.2f, 1.0f};		// Set Ambient Lighting To Fairly Dark Light (No Color)
	GLfloat light0pos[4]=     {0.0f, 5.0f, 10.0f, 1.0f};		// Set The Light Position
	GLfloat light0ambient[4]= {0.2f, 0.2f,  0.2f, 1.0f};		// More Ambient Light
	GLfloat light0diffuse[4]= {0.3f, 0.3f,  0.3f, 1.0f};		// Set The Diffuse Light A Bit Brighter
	GLfloat light0specular[4]={0.8f, 0.8f,  0.8f, 1.0f};		// Fairly Bright Specular Lighting

	GLfloat lmodel_ambient[]= {0.2f,0.2f,0.2f,1.0f};		// And More Ambient Light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);		//Set ambient light model

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);		// Set The Global Ambient Light Model
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);				// Set The Lights Position
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);			// Set The Ambient Light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0diffuse);			// Set The Diffuse Light
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0specular);			// Set Up Specular Lighting
	glEnable(GL_LIGHTING);										// Enable Lighting
	glEnable(GL_LIGHT0);										// Enable Light0

	glShadeModel(GL_SMOOTH);									// Select Smooth Shading

	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5);						// Set The Clear Color To Black

	return 1;												// Return TRUE (Initialization Successful)
}

void Star::deInitialize (){
	glDeleteTextures(1, &blurTexture);
}

float Star::getDiameter(){
	return diameter;
}

float Star::getWeight(){
	return 1.0f;
}

G308_Point Star::getPosition(){

	return G308_Point();
}

G308_Point Star::getVelocity(){

	return G308_Point();
}



