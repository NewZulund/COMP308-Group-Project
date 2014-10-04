#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "define.h"
#include "G308_Geometry.h"
#include "Mass.h"
#include "Planet.h"
#include "textfile.cpp"
#include <iostream>
#include <math.h>

using namespace std;

// Global Variables
GLuint g_mainWind;
GLuint g_windowHeight = 720;
GLuint g_windowWidth = 1280;

Mass* planets[20];

void display();
void reshape(int w, int h);
void SetCamera();
void SetLight();
void G308_Draw3D();
void G308_Draw2D();
void G308_Mouse(int button, int state, int x, int y);
void G308_KeyboardCall(unsigned char key, int x, int y);
void drawText(char * string, float x, float y);
void keyboardSpecialCall(int key, int x, int y);
void redisplay();
void animate(int);

void initShader(GLuint * v, GLuint * f, char * vertFile, char * fragFile, GLuint * prog);

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
	glutInitWindowSize(g_windowWidth, g_windowHeight);
	g_mainWind = glutCreateWindow("Group Project");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(G308_Mouse);
	glutKeyboardFunc(G308_KeyboardCall);

	SetLight();
	SetCamera();

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
	SetLight();
	SetCamera();
	G308_Draw3D();
	G308_Draw2D();
	glutMainLoop();

	return 0;
}

void G308_Draw3D() {
	glutSetWindow(g_mainWind);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glShadeModel(GL_SMOOTH);
	planets[0]->draw();

	glPopMatrix();

}


void mouseMenu(int key){

}

void G308_Draw2D() {

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, g_windowWidth, 0, g_windowHeight);
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

// display function
void display() {
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
	SetCamera();
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
void reshape(int w, int h) {
	if (h == 0)
		h = 1;

	g_windowWidth = w;
	g_windowHeight = h;

	glViewport(0, 0, g_windowWidth, g_windowHeight);
}

// Set Light
void SetLight() {
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
void SetCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(G308_FOVY, (double) g_windowWidth / (double) g_windowHeight,
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
	glutSetWindow(g_mainWind);
	glutPostRedisplay();
	glutSetWindow(i);

}

void initShader(GLuint * ver, GLuint * fra, char * vertFile, char * fragFile, GLuint * prog) {
	GLuint v = *ver;
	GLuint f = *fra;

	char * vs = NULL;
	char * fs = NULL;
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	vs = textFileRead(vertFile);
	fs = textFileRead(fragFile);
	const char * ff = fs;
	const char * vv = vs;
	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);
	free(vs);
	free(fs);
	glCompileShader(v);
	glCompileShader(f);
	*prog = glCreateProgram();
	glAttachShader(*prog, f);
	glAttachShader(*prog, v);
	glLinkProgram(*prog);
	glUseProgram(*prog);

	*ver = v;
	*fra = f;

}

