/*
 * Star.h
 *
 *  Created on: 17/09/2014
 *      Author: blomfiisaa
 */

#ifndef STAR_H_
#define STAR_H_

#include "Mass.h"

class Star: public Mass {
public:
	Star(float diam, float weig);
	virtual ~Star();
	virtual void draw();
	virtual float getDiameter();
	virtual float getWeight();
	virtual G308_Point getPosition();
	virtual G308_Point getVelocity();
	void draw(int windowWidth, int windowHeight);

private:
	float angle;//MIGHT NOT KEEP
	GLuint blurTexture;

	void renderToTexture();
	void processStar();
	void drawBlur(int times, float inc);
	GLuint emptyTexture();
	void orthoView();
	void perspectiveView();
	int initialize();
	void deInitialize();
};

#endif /* STAR_H_ */
