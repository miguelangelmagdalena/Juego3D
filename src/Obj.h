#pragma once

#include "Model.h"

using std::fstream;

class Obj : public CModel
{
	protected:

		std::vector< unsigned int >uvIndices; 
		std::vector< unsigned int >	normalIndices; //Para guardar indices de f

		bool has_normal;
	public:
		Obj();
		~Obj();
		bool load(string filename);
		void display();
		void calculate_normals();

		void calculate_normals_vertex();
};