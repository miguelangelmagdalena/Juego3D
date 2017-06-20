#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <irrklang/irrKlang.h>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "freeTypeFont.h"
#include "GLSLProgram.h"
#include "Model.h"
#include "Off.h"
#include "Obj.h"
#include "skyBox.h"

#define max_level 4

using namespace std;
using namespace irrklang;

class juegoControl
{
	private:
		int level_actual;
		int index;
		int ground;
		int finish;
		vector	<CModel *>		nivel[max_level];
		vector	<unsigned int>	texture;

		//SkyBox
		skyBox *skyBox1;
		vector<vector<string>> skies;

		//Para control de texto
		FreeTypeFont *text;

		//Para control de sonido
		ISoundEngine *SoundEngine;

		//Para control de la pantalla y luego poder imprimir texto
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
	public:
		juegoControl(int width, int height, FreeTypeFont *text, skyBox *skyBox1,vector<vector<string>> skies,vector <unsigned int>	texture);
		~juegoControl();

		bool agregar_objeto(vector <CModel *> models,string file_name, int index_texture, float tx, float ty, float tz, float sx, float sy, float sz, float rx, float ry, float rz, float rw);
		void initScene1();
		void display();

};