#pragma once

#include "Model.h"

using std::fstream;

class Off : public CModel
{
	protected:
		
		vector < glm::vec3 >	colores;

		
	public:
		Off();
		~Off();
		bool load(string path);
		void display();
		void calculate_normals();

		void calculate_normals_vertex();
		
};