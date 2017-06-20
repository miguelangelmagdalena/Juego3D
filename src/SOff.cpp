#include "SOff.h"

CSOff::CSOff()
{
}

CSOff::~CSOff()
{
}

bool CSOff::load(string path)
{
	fstream file;
	string token;

	file.open(path, std::ios::in);

	if(!file.is_open()) {
        printf(" Error, no se puede abrir el archivo %s\n",path);
        return false;
    }

	file >> token;
	if (token != "SOFF"){
		printf(" Error, el archivo %s no es de tipo SOFF\n",path);
		file.close();
		return false;
	}
	else{
		file >> token;
		mNumOfVertices = 3 * atoi(token.c_str());
		mVertices = new float[mNumOfVertices];
		for (int i = 0; i < mNumOfVertices; i++){
			file >> token;
			mVertices[i] = float(atof(token.c_str()));
		}
		file.close();
		return true;
	}
}

void CSOff::display()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (int i = 0; i < mNumOfVertices; i += 9)
	{
		glBegin(GL_TRIANGLES);
			glVertex3f(mVertices[i], mVertices[i + 1], mVertices[i + 2]);
			glVertex3f(mVertices[i + 3], mVertices[i + 4], mVertices[i + 5]);
			glVertex3f(mVertices[i + 6], mVertices[i + 7], mVertices[i + 8]);
		glEnd();
	}
}
void CSOff::calculate_normals(){

}