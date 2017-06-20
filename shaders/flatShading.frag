#version 400

flat in vec3 FragPos; 
flat in vec3 Normal;  
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 lightPosition; 
uniform vec3 viewPosition;

uniform vec3  materialAmbient;
uniform vec3  materialDiffuse;
uniform vec3  materialSpecular;
uniform float materialShininess;

uniform sampler2D texture1;
uniform int has_texture;

void main()
{
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
	vec3 result  			= (ambient + diffuse + specular);

    //----------Aplicamos el color----------
    if(has_texture == 0){
        FragColor               = vec4(result, 1.0);
    }else{
        FragColor = texture(texture1, TexCoord) * vec4(result, 1.0);  
    }
}