#version 120
//Based on Ozone3D Phong + Materials

varying vec3 normal, spotLightDir, pointLightDir;
varying vec3 dirLightDir, eyeVec, reflecVec;

uniform float planetWidth;
vec4 generateColor(float dis);
float getRatio(float p1, float p2, float curVal);
vec4 colr(int r, int g, int b);

void main()
{
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	spotLightDir = vec3(gl_LightSource[0].position.xyz - vVertex);

	eyeVec = -vVertex;

	gl_Position = ftransform();
	//Add Texture
	gl_TexCoord[0] = gl_MultiTexCoord0;

	//Determine vertex color
	vec4 p = gl_Vertex;
	float distFromCenter = sqrt((p.x * p.x) + (p.y * p.y) + (p.z * p.z));
	vec4 vertCol =  generateColor(distFromCenter);
	vertCol.a = 1.0f;
	gl_FrontColor = vertCol;

	//Calculate reflection off vert
	vec3 NN = normalize(normal.xyz);
	reflecVec = reflect(eyeVec, NN);
}

//vec4 DEEPWATER = vec4(0.0f,0.2f,0.5f,1.0f);
//vec4 SHALLOWWATER = vec4(0.1f,0.3f,0.7f,1.0f);
//vec4 SAND = vec4(0.76f,0.69f, 0.5f);
//vec4 GRASS = vec4(0.0f, 0.45f, 0.07f);
//vec4 Hills = vec4(0.0f, 0.65f, 0.06f);
//vec4 SteelHills = vec4(0.5f, 0.5f, 0.5f,1.0f);
//vec4 Mountains = vec4(1.0f, 1.0f, 1.0f);

vec4 generateColor(float dis){
	vec4 vertCol = vec4(1.0f,1.0f,1.0f,1.0f);
	vec4 col1 = vec4(0.0f,0.0f,0.0f,1.0f);
	vec4 col2 = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	float ratio = 0.0f;

	if(dis > 1.07f){
		col1 = colr(8,99,0);
		col2 = colr(36,96,36);
		ratio = getRatio(2.0f, 1.07f, dis);
	}else if(dis > 1.05f){
		col1 = colr(36,96,36);
		col2 = colr(16,50,16);
		ratio = getRatio(1.07f, 1.05f, dis);
	}else if(dis > 1.03f){
		col1 = colr(16,50,16);
		col2 = colr(16,50,16);
		ratio = getRatio(1.05f, 1.03f, dis);
	}else if(dis > 1.01f){
		col1 = colr(16,50,16);
		col2 = colr(8,99,0);
		ratio = getRatio(1.03f, 1.01f, dis);
	}else if(dis > 0.99f){
		col1 = colr(8,99,0);
		col2 = colr(198,191,102);
		ratio = getRatio(1.01f, 0.99f, dis);
	}else if(dis > 0.97f){
		col1 = colr(198,191,102);
		col2 = colr(40,114,110);
		ratio = getRatio(0.99f, 0.97f, dis);
	}else if(dis > 0.95f){
		col1 = colr(40,114,110);
		col2 = colr(198,181,102);
		ratio = getRatio(0.97f, 0.95f, dis);
	}else if(dis > 0.93f){
		col1 = colr(198,181,102);
		col2 = colr(198,181,102);
		ratio = getRatio(0.95f, 0.93f, dis);
	}else if(dis > 0.91f){
		col1 = colr(198,181,102);
		col2 = colr(198,181,102);
		ratio = getRatio(0.93f, 0.91f, dis);
	}else if(dis > 0.88f){
		col1 = colr(198,181,102);
		col2 = colr(198,181,102);
		ratio = getRatio(0.91f, 0.88f, dis);
	}else if(dis > 0.86f){
		col1 = colr(198,181,102);
		col2 = colr(198,181,102);
		ratio = getRatio(0.88f, 0.86f, dis);
	}else{
		col1 = colr(198,181,102);
		col2 = colr(0,0,0);
		ratio = getRatio(0.86f, 0.0f, dis);

	}

	vec4 coldif = col1 - col2;
	vertCol =  (col2 + (coldif * ratio));
	return vertCol;
}

float getRatio(float p1, float p2, float curVal){
	float totdif = p1 - p2;
	float curdif = curVal - p2;
	return (curdif / totdif);
}

vec4 colr(int r, int g, int b){
	vec4 color = vec4(0.0f,0.0f,0.0f,1.0f);
	color.r = (r / 255.0f);
	color.g = (g / 255.0f);
	color.b = (b / 255.0f);
	return color;
}
