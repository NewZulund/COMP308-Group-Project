#version 120
//Based on Ozone3D Phong
varying vec3 normal, spotLightDir, pointLightDir;
varying vec3 dirLightDir, eyeVec, reflecVec;
uniform sampler2D curTexture;

uniform vec4 spherePos;
uniform float planetWidth;

void main (void)
{
	vec4 specColor = vec4(0.1f,0.1f,0.1f,1.0f);
	vec4 diffColor = vec4(0.1f,0.1f,0.1f,1.0f);//gl_Color;
	vec4 ambiColor = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
	//vec4 textColor = vec4(texture2D(curTexture, gl_TexCoord[0].st).xyz, 1.0f);

	vec3 N = normalize(normal);
	vec3 L = normalize(spotLightDir);
	vec3 D = normalize(gl_LightSource[0].spotDirection);

	diffColor = gl_Color * max(dot(N,L), 0.0);
	diffColor = clamp(diffColor, 0.0, 1.0);

	//specColor = gl_LightSource[0].specular ;//* pow(max(dot(D,E),0.0f), 0.3f * gl_FrontMaterial.shininess);
	//specColor = clamp(specColor, 0.0, 1.0);

	vec4 final_color = diffColor + ambiColor +specColor;
	gl_FragColor = final_color;

}




