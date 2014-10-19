#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "define.h"
#include "G308_Geometry.h"
#include "Mass.h"
#include "Planet.h"
#include "Star.h"
#include "textfile.cpp"
#include <iostream>
#include <math.h>

using namespace std;

// Global Variables
GLuint g_mainWind;
GLuint g_windowHeight = 720;
GLuint g_windowWidth = 1280;

//Planet Variables
Mass* planets[20];
GLuint earthFrag, earthShader, skyVert, skyFrag;;
GLuint dispProg, skyProg;

void display();
void reshape(int w, int h);
void SetCamera();
void setLight();
void draw3D();
void draw2D();
void mouse(int button, int state, int x, int y);
void keyboardCall(unsigned char key, int x, int y);
void drawText(char * string, float x, float y);
void keyboardSpecialCall(int key, int x, int y);
void redisplay();
void animate(int);
void mouseFunc(int button, int state, int x, int y);

void mouseMove(int x, int y);
void panCamera(int, int, int, int);
void zoomCamera(int oldMouseX, int oldMouseY, int x, int y);
void rotateCamera(int oldMouseX, int oldMouseY, int x, int y);

void createCubemap();
void createCubeMapModel();
void drawEnvironmentMap();

void initShader(GLuint * v, GLuint * f, char * vertFile, char * fragFile,
		GLuint * prog);

static int TIMERMSECS = 50;

// angle of rotation for the camera direction
float angle = 0.0, angleY = 0.0f;
float xOffset = 0.0f, yOffset = 0.0f, zOffset = 0.0f;
float yRot = 0.0f, zRot = 0.0f, xRot = 0;

float lx = 0.0f, lz = -1.0f, ly = 0.0f;
float x = 0.0f, z = 5.0f, y = 0.0f;
float zoom = 0.0f;
float deltaAngle = 0.0f;
int xOrigin = -1;

GLuint cubeMapTex;
GLuint * tex_cube = NULL;
GLuint vbo ,vao;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_windowWidth, g_windowHeight);
	g_mainWind = glutCreateWindow("Group Project");
	glutDisplayFunc(display);
	glutMotionFunc(mouseMove);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseFunc);
	glutKeyboardFunc(keyboardCall);
	srand(time(NULL));

	//Initialize GLEW
	glewInit();
	initShader(&earthShader, &earthFrag, "Shaders/displacementVert.vert",
			"Shaders/displacementFrag.frag", &dispProg);

	printf("Shader \n");
	initShader(&skyVert, &skyFrag, "Shaders/skymap.vert", "Shaders/skymap.frag", &skyProg);

	Planet * plan = new Planet(10, 1);
	planets[0] = plan;
	printf("Shader is \n");
	createCubemap();

	printf("Shader is working \n");

	setLight();
	SetCamera();
	printf("Shader is working123123 \n");
	draw3D();
	draw2D();
	glutMainLoop();

	return 0;
}

void draw3D() {
	glutSetWindow(g_mainWind);


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgram(dispProg);

	GLfloat mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	GLfloat transf[4] = { mat[12], mat[13], mat[14], 1.0f };
	GLint spherePos = glGetUniformLocation(dispProg, "spherePos");
	glUniform4fv(spherePos, 1, transf);
	GLuint planetWidth = glGetUniformLocation(dispProg, "planetWidth");
	float diam = planets[0]->diameter;
	glUniform1fv(planetWidth, 1, (&diam));

	//printf("%d disprog, %d myloc, %d planetWidth, %f planet \n",dispProg, spherePos, planetWidth, diam);

	glColor3f(0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glPushMatrix();
	planets[0]->draw();
	glPopMatrix();

	glPopMatrix();

}

void mouseMenu(int key) {

}

void draw2D() {

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	gluOrtho2D(0, g_windowWidth, 0, g_windowHeight);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	//Transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Text Over UI
	drawText("WASD : Spotlight Controls | A/E Cutoff", -0.8f, 0.9f);
	drawText("ARROW KEYS : Camera Controls | t Rotate models", -0.8f, 0.85f);
	drawText("Press , or . to zoom in and out", -0.8f, 0.75f);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

// display function
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	//Apply view rotations
	glPushMatrix();
	glLoadIdentity();
	SetCamera();
	setLight();

	draw3D();
	draw2D();

	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

void animate(int key) {

}

int keyDown = FALSE;
int keyMod = FALSE;
int oldMouseX;
int oldMouseY;

void mouseFunc(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		keyDown = button;
		keyMod = glutGetModifiers();
		//printf("Key down %d\n", keyDown);
	} else {
		keyDown = -1;
		keyMod = -1;
	}

	oldMouseX = x;
	oldMouseY = y;

	glutPostRedisplay();
}

void mouseMove(int x, int y) {

	//printf("%d \n", keyDown);
	if (keyDown == -1) {
		return;
	}
	//printf("Key down and moved \n");
	if (keyDown == GLUT_RIGHT_BUTTON) {

		if (keyMod == GLUT_ACTIVE_SHIFT) {
			panCamera(oldMouseX, oldMouseY, x, y);

		} else if (keyMod == GLUT_ACTIVE_CTRL) {
			zoomCamera(oldMouseX, oldMouseY, x, y);

		} else {

		}
	} else if (keyDown == GLUT_LEFT_BUTTON) {
		//printf("Rotating \n");
		rotateCamera(oldMouseX, oldMouseY, x, y);
	}

	oldMouseX = x;
	oldMouseY = y;
	glutPostRedisplay();
}

void mouseClick(int x, int y) {

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
void setLight() {
	glPushMatrix();
	float pointposition[] = { 0.0f, 0.0f, 100.0f, 0.0f };
	float pointdiffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	float pointambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float pointspecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, pointposition);
	glLightfv(GL_LIGHT0, GL_SPECULAR, pointspecular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, pointdiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, pointambient);
	glEnable(GL_LIGHT0);

	glPopMatrix();
}

void keyboardCall(unsigned char key, int x, int y) {
	int newPlan = FALSE;
	float fraction = 0.1f;

	switch (key) {
	case 27:
		exit(0);
		break;
	case 'd':
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'a':
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'w':
		x += lx * fraction;
		z += lz * fraction;
		break;
	case 's':
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	case ',':
		zoom += 0.5;
		break;
	case '.':
		zoom -= 0.5;
		break;
	case 'r':
		break;
	default:
		break;
	}

	if (newPlan == TRUE) {
		planets[0] = new Planet(10, 1);
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		} else { // state = GLUT_DOWN
			xOrigin = x;
		}
	}

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

	//gluLookAt(0.0, 2.5, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	float camPosX = x;
	float camPosY = 1.0f;
	float camPosZ = 50 + z;

	gluLookAt(camPosX, camPosY, camPosZ, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);

	glRotatef(xRot, 1, 0, 0);
	glRotatef(yRot, 0, 1, 0);
	glRotatef(zRot, 0, 0, 1);


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glTranslatef(camPosX, camPosY, camPosZ);
	//glScalef(10,10,10);
	drawEnvironmentMap();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(xOffset, yOffset, zOffset);
}

void redisplay() {
	int i = glutGetWindow();
	glutSetWindow(g_mainWind);
	glutPostRedisplay();
	glutSetWindow(i);

}

void initShader(GLuint * ver, GLuint * fra, char * vertFile, char * fragFile,
		GLuint * prog) {
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

void drawText(char * words, float x, float y) {
	glUseProgram(0);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_LIGHTING);
	glDisable( GL_DEPTH_TEST);
	glRasterPos2f(x, y);

	char buf[300];
	sprintf(buf, words);

	char c = words[0];
	int i = 0;
	while (c != '\0') {

		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
		i++;
		c = words[i];
	}

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable( GL_DEPTH_TEST);
	glPopMatrix();
}

void panCamera(int oldMouseX, int oldMouseY, int x, int y) {

	float xOff = -(x - oldMouseX) / 100.0f;
	float yOff = -(y - oldMouseY) / 100.0f;
	xOffset += xOff;
	yOffset += yOff;

}

void zoomCamera(int oldMouseX, int oldMouseY, int x, int y) {
	zoom += (oldMouseY - y) / 100.0f;
}

void rotateCamera(int oldMouseX, int oldMouseY, int x, int y) {
	//Flip axis;
	yRot += -(oldMouseX - x) / 10.0f;
	xRot += -(oldMouseY - y) / 10.0f;

	//angle += 0.01f;
	//lx = sin(angle);
	//lz = -cos(angle);

}

void createCubemap() {
	tex_cube = (GLuint*) malloc(sizeof(GLuint) * 8);

	TextureInfo front;
	loadTextureFromJPEG("images/hidef/posz.jpg", &front);
	TextureInfo back;
	loadTextureFromJPEG("images/hidef/posx.jpg", &back);
	TextureInfo left;
	loadTextureFromJPEG("images/hidef/posy.jpg", &left);
	TextureInfo right;
	loadTextureFromJPEG("images/hidef/negx.jpg", &right);
	TextureInfo top;
	loadTextureFromJPEG("images/hidef/negy.jpg", &top);
	TextureInfo bottom;
	loadTextureFromJPEG("images/hidef/negz.jpg", &bottom);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	GLuint height = 2048;
	GLuint width = 2048;
	glGenTextures(1, tex_cube);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *tex_cube);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, height, width, 0,
	GL_RGB,
	GL_UNSIGNED_BYTE, right.textureData);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *tex_cube);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, height, width, 0,
	GL_RGB,
	GL_UNSIGNED_BYTE, left.textureData);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *tex_cube);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, height, width, 0,
	GL_RGB,
	GL_UNSIGNED_BYTE, bottom.textureData);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *tex_cube);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, height, width, 0,
	GL_RGB,
	GL_UNSIGNED_BYTE, top.textureData);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *tex_cube);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, height, width, 0,
	GL_RGB,
	GL_UNSIGNED_BYTE, front.textureData);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *tex_cube);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, height, width, 0,
	GL_RGB,
	GL_UNSIGNED_BYTE, back.textureData);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glDisable(GL_TEXTURE_CUBE_MAP);
	createCubeMapModel();
}

/*
 * Code from antongerdelan.net
 */
void createCubeMapModel() {
	float points[] = { -30.0f, 30.0f, -30.0f, -30.0f, -30.0f, -30.0f, 30.0f,

	-30.0f, -30.0f, 30.0f, -30.0f, -30.0f, 30.0f, 30.0f, -30.0f, -30.0f, 30.0f,
			-30.0f, -30.0f, -30.0f, 30.0f, -30.0f, -30.0f, -30.0f, -30.0f,
			30.0f, -30.0f, -30.0f, 30.0f, -30.0f, -30.0f, 30.0f, 30.0f, -30.0f,
			-30.0f, 30.0f, 30.0f, -30.0f, -30.0f, 30.0f, -30.0f, 30.0f, 30.0f,
			30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, -30.0f, 30.0f,
			-30.0f, -30.0f, -30.0f, -30.0f, 30.0f, -30.0f, 30.0f, 30.0f, 30.0f,
			30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, -30.0f, 30.0f, -30.0f,
			-30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 30.0f, 30.0f, -30.0f, 30.0f,
			30.0f, 30.0f, 30.0f, 30.0f, 30.0f, -30.0f, 30.0f, 30.0f, -30.0f,
			30.0f, -30.0f, -30.0f, -30.0f, -30.0f, -30.0f, -30.0f, 30.0f, 30.0f,
			-30.0f, -30.0f, 30.0f, -30.0f, -30.0f, -30.0f, -30.0f, 30.0f, 30.0f,
			-30.0f, 30.0f };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points,
	GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenVertexArrays(1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void drawEnvironmentMap() {
	glDepthMask(GL_FALSE);
	glUseProgram (skyProg);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, *tex_cube);

	glBindVertexArray (vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

