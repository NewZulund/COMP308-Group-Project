//---------------------------------------------------------------------------
//
// Copyright (c) 2012 Taehyun Rhee
//
// This software is provided 'as-is' for assignment of COMP308
// in ECS, Victoria University of Wellington,
// without any express or implied warranty.
// In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glut.h>

// My definition
#include "define.h"
#include "G308_Geometry.h"
#include "Mass.h"
#include "Planet.h"
#include <iostream>
#include <math.h>

using namespace std;

// Global Variables
GLuint g_mainWnd;
GLuint g_nWinWidth = G308_WIN_WIDTH;
GLuint g_nWinHeight = G308_WIN_HEIGHT;
G308_Geometry* g_pGeometry = NULL;
Mass* planets[20];

void G308_Display2();
void G308_Mouse2(int button, int state, int x, int y);

void G308_Display();
void G308_Reshape(int w, int h);
void G308_SetCamera();
void G308_SetLight();
void G308_Draw3D();
void G308_Draw2D();
void G308_Mouse(int button, int state, int x, int y);
void G308_KeyboardCall(unsigned char key, int x, int y);
void drawPoints();
void drawText(char * string, float x, float y);
void G308_KeyboardSpecialCall(int key, int x, int y);
void animate(int);
void redisplay();

static int TIMERMSECS = 50;

float xRot = 0;
float yRot = 0;
float zRot = 0;
float zoom = 0;

void mouseMenu(int);
MENU_TYPE animMode = MENU_STOP;

static const int TRUE = 1;
static const int FALSE = 0;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_nWinWidth, g_nWinHeight);
	g_mainWnd = glutCreateWindow("COMP308 Assignment1");
	glutDisplayFunc(G308_Display);
	glutReshapeFunc(G308_Reshape);
	glutMouseFunc(G308_Mouse);
	glutKeyboardFunc(G308_KeyboardCall);

	G308_SetLight();
	G308_SetCamera();

	//Mouse Menu
	glutCreateMenu(mouseMenu);
	glutAddMenuEntry("Play", MENU_PLAY);
	glutAddMenuEntry("Pause", MENU_PAUSE);
	glutAddMenuEntry("Stop", MENU_STOP);
	glutAddMenuEntry("Reset", MENU_RESET);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutTimerFunc(TIMERMSECS, animate, 0);


	Planet * plan = new Planet(10,1,1,1);
	planets[0] = plan;
	G308_SetLight();
	G308_SetCamera();
	G308_Draw3D();
	G308_Draw2D();
	glutMainLoop();

	return 0;
}

void G308_Draw3D() {
	glutSetWindow(g_mainWnd);
	float offsetHeight = G308_WIN_HEIGHT / 2;
	float offsetWidth = G308_WIN_WIDTH / 2;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glShadeModel(GL_SMOOTH);
	//glutSolidTeapot(5.0);
	planets[0]->draw();



	glPopMatrix();

}


void mouseMenu(int key){

}

static float UI_WIDTH = 200;
static float UI_HEIGHT = 400;
static float UI_BORDER = 10;
static float UI_X = g_nWinWidth - UI_WIDTH - UI_BORDER;
static float UI_Y = g_nWinHeight - UI_BORDER;

void G308_Draw2D() {

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, g_nWinWidth, 0, g_nWinHeight);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	//2D stuff;
	glPopMatrix();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

// Display function
void G308_Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(1.0f, 0.0f, 0.0f); /* set object color as red */

	// [Assignmet1] : render shaded polygon or wireframe
	//Apply view rotations
	glPushMatrix();
	glLoadIdentity();
	G308_SetCamera();
	G308_Draw3D();
	G308_Draw2D();

	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

void animate(int key){


}

// Reshape function
void G308_Reshape(int w, int h) {
	if (h == 0)
		h = 1;

	g_nWinWidth = w;
	g_nWinHeight = h;

	glViewport(0, 0, g_nWinWidth, g_nWinHeight);
}

// Set Light
void G308_SetLight() {
	float direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	float diffintensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glEnable(GL_LIGHT0);
}

void G308_KeyboardCall(unsigned char key, int x, int y) {
	printf("Key = %c \n", key);

	glutPostRedisplay();
}

void G308_Mouse(int button, int state, int x, int y) {

	glutPostRedisplay();
}

// Set Camera Position
void G308_SetCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(G308_FOVY, (double) g_nWinWidth / (double) g_nWinHeight,
	G308_ZNEAR_3D, G308_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 2.5, 50.0 + zoom, 0.0, 2.5, 0.0, 0.0, 1.0, 0.0);

	glRotatef(xRot,1,0,0);
	glRotatef(yRot,0,1,0);
	glRotatef(zRot,0,0,1);

}

void redisplay(){
	int i = glutGetWindow();
	glutSetWindow(g_mainWnd);
	glutPostRedisplay();
	glutSetWindow(i);


}

