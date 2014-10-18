#version 120
//Based on Ozone3D Phong + Materials

varying vec3 normal, spotLightDir, pointLightDir;
varying vec3 dirLightDir, eyeVec, reflecVec;

uniform float planetWidth;
vec4 generateColor(float dis);
float getRatio(float p1, float p2, float curVal);

void main()
{
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	spotLightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	pointLightDir = vec3(gl_LightSource[1].position.xyz - vVertex);
	dirLightDir = vec3(gl_LightSource[2].position.xyz);// - vVertex);

	eyeVec = -vVertex;

	gl_Position = ftransform();
	//Add Texture
	gl_TexCoord[0] = gl_MultiTexCoord0;

	//Determine vertex color
	vec4 p = gl_Vertex;
	float distFromCenter = sqrt((p.x * p.x) + (p.y * p.y) + (p.z * p.z));
	vec4 vertCol =  generateColor(distFromCenter);
	gl_FrontColor = vertCol;


	//Calculate reflection off vert
	vec3 NN = normalize(normal.xyz);
	reflecVec = reflect(eyeVec, NN);
}


float getRatio(float p1, float p2, float curVal){
	float totdif = p1 - p2;
	float curdif = curVal - p2;
	return (curdif / totdif);
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

	if(dis > 1.03f){
		return vec4(1.0f,1.0f,1.0f,1.0f);
	}else if(dis > 1.02f){
		col1 = vec4(0.5f, 0.5f, 0.5f,1.0f);
		col2 = vec4(0.0f, 0.65f, 0.06f,1.0f);
		ratio = getRatio(1.5, 1.06f, dis);
	}else if(dis > 1.01f){
		col1 = vec4(0.5f, 0.5f, 0.5f,1.0f);
		col2 = vec4(0.0f, 0.65f, 0.06f,1.0f);
		ratio = getRatio(1.125, 1.02f, dis);
	}else if(dis > 1.0f){
		col1 = vec4(0.0f, 0.65f, 0.06f,1.0f);
		col2 = vec4(0.0f, 0.45f, 0.07f,1.0f);
		ratio = getRatio(1.09f, 1.0f, dis);
	}else if(dis > 0.99f){
		col1 = vec4(0.0f, 0.45f, 0.07f,1.0f);
		col2 = vec4(0.76f,0.69f, 0.5f,1.0f);
		ratio = getRatio(1.0f, 0.99f, dis);
	}else if(dis > 0.85f){
		col1 = vec4(0.76f,0.69f, 0.5f,1.0f);
		col2 = vec4(0.1f,0.3f,0.7f,1.0f);
		ratio = getRatio(0.99f, 0.85, dis);
	}else if(dis > 0.5f){
		col1 = vec4(0.1f,0.3f,0.7f,1.0f);
		col2 = vec4(0.0f,0.2f,0.5f,1.0f);
		ratio = getRatio(0.85, 0.5, dis);
	}else{
		return vec4(0.0f,0.2f,0.5f,1.0f);
	}

	vec4 coldif = col1 - col2;
	vec4 steps = coldif / 100;

	return (col2 + (steps * ratio));
	//return vec4(0.0f,0.2f,0.5f,1.0f);
}

