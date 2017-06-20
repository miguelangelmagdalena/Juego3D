#version 400


in vec3 Result;
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform int has_texture;

void main()
{


    //----------Aplicamos el color----------
    if(has_texture == 0){
        FragColor               = vec4(Result, 1.0);
    }else{
        FragColor = texture(texture1, TexCoord) * vec4(Result, 1.0);  
    }
}