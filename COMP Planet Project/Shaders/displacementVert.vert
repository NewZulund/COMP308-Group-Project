#version 120
//Based on Ozone3D Phong + Materials	
	
varying vec3 normal, spotLightDir, pointLightDir; 
varying vec3 dirLightDir, eyeVec, reflecVec;

uniform sampler2D curTexture;

void main()
{	
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	spotLightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	pointLightDir = vec3(gl_LightSource[1].position.xyz - vVertex);
	dirLightDir = vec3(gl_LightSource[2].position.xyz);// - vVertex);
	
	eyeVec = -vVertex;
	
	//Displace
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec4 dVect = vec4(texture2D(curTexture, gl_TexCoord[0].st).xyz, 1.0f);
	
	vec4 ModifiedNormal = vec4(dVect.x * normal.x, dVect.y * normal.y, dVect.z * normal.z, 1.0f);
	
	gl_Position = ftransform() + ModifiedNormal;		
	
	//Calculate reflection off vert
	vec3 NN = normalize(normal.xyz);
	reflecVec = reflect(eyeVec, NN);
}