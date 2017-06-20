#include "skyBox.h"

skyBox::skyBox(vector<string> faces, CGLSLProgram *glslSkyBox){
	this->glslSkyBox = glslSkyBox;
	this->faces = faces;
	init();
}
void skyBox::init(){

	//CubeMap
	float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
	};

	// skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	/*vector<string> faces;

	faces.push_back("../models/textures/skybox/right.jpg");
	faces.push_back("../models/textures/skybox/left.jpg");
    faces.push_back("../models/textures/skybox/top.jpg");
    faces.push_back("../models/textures/skybox/bottom.jpg");
    faces.push_back("../models/textures/skybox/back.jpg");
    faces.push_back("../models/textures/skybox/front.jpg");*/
 
    change_texture(faces);
}
void skyBox::display(glm::mat4 view, glm::mat4 projection){
	glslSkyBox->enable();
		

		view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix

		glslSkyBox->loadUniformMatrix("projection",			GShader::UNIFORM_SIZE_4D, glm::value_ptr(projection));
		glslSkyBox->loadUniformMatrix("view",				GShader::UNIFORM_SIZE_4D, glm::value_ptr(view));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    
		//skyboxShader.use();
	   // view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		//skyboxShader.setMat4("view", view);
	   // skyboxShader.setMat4("projection", projection);
   
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default


	glslSkyBox->disable();	
}
unsigned int skyBox::loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {

        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		stbi_failure_reason();
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            
        }
        else
        {
			printf( "Cubemap texture failed to load at path: %s\n",faces[i].c_str()); 
			return -1;
		}
		stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
void skyBox::change_texture(vector<string> faces){
	
	
	cubemapTexture = loadCubemap(faces);
	faces.clear();
}