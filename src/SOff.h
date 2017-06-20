#pragma once

#include "Model.h"
#include <iostream>
#include <fstream>

using std::fstream;

class CSOff : public CModel
{
	protected:
		int   mNumOfVertices, mNumOfFaces, mNumOfEdges;
	public:
		CSOff();
		~CSOff();
		bool load(string path);
		void display();
		void calculate_normals();
};