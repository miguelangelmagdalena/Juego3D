#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "stb_image.h"
#include "GLSLProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

class skyBox
{
	private:
		unsigned int skyboxVAO, skyboxVBO;
		unsigned int cubemapTexture ;
		vector<string> faces;
		CGLSLProgram *glslSkyBox;
	public:
		skyBox(vector<string> faces, CGLSLProgram *glslSkyBox);
		void init();
		void display(glm::mat4 view, glm::mat4 projection);
		unsigned int loadCubemap(vector<string> faces);
		void change_texture(vector<string> faces);
	
};