#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexture;

out vec3 Result;
out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 lightPosition; 
uniform vec3 viewPosition;

uniform vec3  materialAmbient;
uniform vec3  materialDiffuse;
uniform vec3  materialSpecular;
uniform float materialShininess;

void main()
{
	vec3 FragPos;
	vec3 Normal;

	gl_Position = projection * view * model * vec4(VertexPosition, 1.0);

	FragPos = vec3(model * vec4(VertexPosition, 1.0));
	
	Normal = mat3(transpose(inverse(model))) * VertexNormal;  

	//----------Luz Ambiental----------
    vec3 ambient 			= lightAmbient * materialAmbient;

    //----------Luz difusa----------
    vec3 norm 				= normalize(Normal);
	vec3 lightDir 			= normalize(lightPosition - FragPos);  
    float diff 				= max(dot(norm, lightDir), 0.0);
    vec3 diffuse 			= lightDiffuse * (diff * materialDiffuse);

    //----------Luz Especular----------
    //float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPosition - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
	vec3 specular = lightSpecular * spec * materialSpecular;  

    //----------Luz Total----------
	Result  			= (ambient + diffuse + specular);

	TexCoord = vec2(VertexTexture.x, VertexTexture.y);
}