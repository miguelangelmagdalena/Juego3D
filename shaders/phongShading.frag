#version 400

in vec3 FragPos; 
in vec3 Normal;  
in vec2 TexCoord;
in vec4 model_vertexPosition; //Para el fog

out vec4 FragColor;

struct Light{
    vec3 lightAmbient;
    vec3 lightDiffuse;
    vec3 lightSpecular;
    vec3 lightPosition; 
    vec3 viewPosition;
    
    //Luz de finish
    vec3 finish_position;
    vec3 finish_lightDiffuse;
    vec3 finish_lightSpecular;

    //Flashlight
    vec3 position;  
    vec3 direction;
    float cutOff;

    //Atenuacion
    float constant;
    float linear;
    float quadratic;

};
struct Material{
    vec3  materialAmbient;
    vec3  materialDiffuse;
    vec3  materialSpecular;
    float materialShininess;
};
struct FogParameters{
    vec4  vFogColor; // Fog color
    float fStart;   // This is only for linear fog
    float fEnd;     // This is only for linear fog
    float fDensity; // For exp and exp2 equation   
    int iEquation;  // 0 = linear, 1 = exp, 2 = exp2
};
struct Propiedades{
    int flashlight;
    int flashlight_fog;
};
uniform Light light;
uniform Material material;
uniform FogParameters fogParams;
uniform Propiedades propiedades;

uniform sampler2D texture1;
uniform int has_texture;
uniform int fog_active;


float getFogFactor(FogParameters params, float fFogCoord);

void main()
{
    vec3 result  ;
    vec3 lightDir;
    float distance;
    float attenuation;

    //----------Luz Ambiental----------
    vec3 ambient        = light.lightAmbient * material.materialAmbient;

    //----------Luz difusa----------
    vec3 norm           = normalize(Normal);
    lightDir            = normalize(light.lightPosition - FragPos);  
    float diff          = max(dot(norm, lightDir), 0.0);
    vec3 diffuse        = light.lightDiffuse * (diff * material.materialDiffuse);

    //----------Luz Especular----------
    vec3 viewDir        = normalize(light.viewPosition - FragPos);
    vec3 reflectDir     = reflect(-lightDir, norm);  
    float spec          = pow(max(dot(viewDir, reflectDir), 0.0), material.materialShininess);
    vec3 specular       = light.lightSpecular * spec * material.materialSpecular;  



/*
    //-----------LLEGADA

    //----------Luz difusa----------
   
        lightDir            = normalize(light.finish_position - FragPos);
        diff                = max(dot(norm, lightDir), 0.0);
        diffuse             = diffuse + light.finish_lightDiffuse * diff * material.materialDiffuse;

        //----------Luz Especular----------
        viewDir             = normalize(light.finish_position - FragPos);
        reflectDir          = reflect(-lightDir, norm);  
        spec                = pow(max(dot(viewDir, reflectDir), 0.0), 0.6);
        specular            = specular + light.finish_lightSpecular * spec * material.materialSpecular;
    //}
*/
/*    
    //----------Luz difusa----------
     norm           = normalize(Normal);
    lightDir            = normalize(light.finish_position - FragPos);  
     diff          = max(dot(norm, lightDir), 0.0);
     diffuse        = diffuse +light.finish_lightDiffuse * (diff * material.materialDiffuse);

    //----------Luz Especular----------
     viewDir        = normalize(light.viewPosition - FragPos);
     reflectDir     = reflect(-lightDir, norm);  
    spec          = pow(max(dot(viewDir, reflectDir), 0.0), material.materialShininess);
     specular       = specular + light.finish_lightSpecular * spec * material.materialSpecular;  

*/



    // check if lighting is inside the spotlight cone
    lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction)); 
    if(theta > light.cutOff && propiedades.flashlight == 1){ 
        //----------Luz difusa----------
        //norm                = normalize(Normal);
        lightDir            = normalize(light.position - FragPos);
        diff                = max(dot(norm, lightDir), 0.0);
        diffuse             = diffuse + light.lightDiffuse * (diff * material.materialDiffuse);

        //----------Luz Especular----------
        viewDir        = normalize(light.position - FragPos);
        reflectDir     = reflect(-lightDir, norm);  
        spec           = pow(max(dot(viewDir, reflectDir), 0.0), material.materialShininess);
        specular       = specular + light.lightSpecular * spec * material.materialSpecular;  


        //--FlashLight
        // attenuation
        distance      = length(light.position - FragPos);
        attenuation   = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse  *= attenuation;
        specular *= attenuation;   

    }else{
        // attenuation

        distance    = length(light.lightPosition - FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;   

    }





    //----------Luz Total----------
    //if((theta > light.cutOff && propiedades.flashlight_fog == 1 && propiedades.flashlight==1))
    result             = (ambient + diffuse + specular);
    //else
        //result             = (ambient);

    //----------Aplicamos el color----------
    if(has_texture == 0){
        FragColor               = vec4(result, 1.0);
    }else{
        FragColor = texture(texture1, TexCoord) * vec4(result, 1.0);  
    }

     //----------Probando el fog-------------
    if(fog_active == 1 && !(theta > light.cutOff && propiedades.flashlight_fog == 1 && propiedades.flashlight==1)){
        float dist = abs(model_vertexPosition.z/1.0);
        FragColor = mix(FragColor, fogParams.vFogColor, getFogFactor(fogParams, dist));
    }
    
}





float getFogFactor(FogParameters params, float dist)
{
    float fResult = 0.0;
    if(params.iEquation == 0){
        fResult = (params.fEnd-dist)/(params.fEnd-params.fStart);
    }
    else if(params.iEquation == 1){
        fResult = exp(-params.fDensity*dist);

        //fResult = 1.0 /exp(dist * params.fDensity);
        //fResult = clamp( fResult, 0.0, 1.0 );
    }
    else if(params.iEquation == 2){
        fResult = exp(-pow(params.fDensity*dist, 2.0));
    }
        
    fResult = 1.0-clamp(fResult, 0.0, 1.0);
    
    return fResult;
}