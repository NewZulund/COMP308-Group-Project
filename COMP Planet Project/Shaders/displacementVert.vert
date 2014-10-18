#version 120
//Based on Ozone3D Phong + Materials

varying vec3 normal, spotLightDir, pointLightDir;
varying vec3 dirLightDir, eyeVec, reflecVec;

uniform float planetWidth;

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
	vec4 vertCol = vec4(1.0f,1.0f,1.0f,1);

	if(distFromCenter > 1.09){
		vertCol = vec4(0.7f,0.7f,0.7f,1);
	}else if(distFromCenter > 1){
		vertCol = vec4(0.0f,0.5f,0.0f,1);
	}else if(distFromCenter > 0.99){
		vertCol = vec4(0.0f ,0.0f,0.8f,1);
	}else{
		vertCol = vec4(0.0f ,0.0f,0.5f,1);
	}
	//vertCol = gl_Vertex;

	gl_FrontColor = vertCol;


	//Calculate reflection off vert
	vec3 NN = normalize(normal.xyz);
	reflecVec = reflect(eyeVec, NN);
}