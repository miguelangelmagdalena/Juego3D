#pragma once

#include <GL/glew.h>
#include <map>
#include <cstdio>
#include <string>
#include <fstream>

using std::ios;
using std::map;
using std::string;
using std::fstream;

namespace GShader
{
	enum GLSLUniformTypes
	{
		UNIFORM_SIZE_1D = 1,
		UNIFORM_SIZE_2D = 2,
		UNIFORM_SIZE_3D = 3,
		UNIFORM_SIZE_4D = 4,
		UNIFORM_SIZE_2X2 = 2,
		UNIFORM_SIZE_3X3 = 3,
		UNIFORM_SIZE_4X4 = 4
	};

	enum GLSLShaderType
	{
		VERTEX = GL_VERTEX_SHADER,
		COMPUTE = GL_COMPUTE_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER,
		TESSELATION_EVALUATION = GL_TESS_EVALUATION_SHADER
	};

	enum GLSLShaderPositions
	{
		VERTEX_POSITION,
		COMPUTE_POSITION,
		FRAGMENT_POSITION,
		GEOMETRY_POSITION,
		TESSELATION_CONTROL_POSITION,
		TESSELATION_EVALUATION_POSITION
	};

	struct GLSLShaderFileExtension
	{
		const GLchar * mExtension;
		GShader::GLSLShaderType mType;
	};

	extern struct GLSLShaderFileExtension Extensions[9];
};

class CGLSLProgram
{
private:
	GLuint mProgram;
	GLuint mShaders[6];

	map<string, GLuint> mUniformsList;
	map<string, GLuint> mAttributesList;

	string getTypeString(const GLenum nType);
	GLuint getShaderType(const string nFilePath);

	string getFileExtension(const string nFilePath);
	void compileShader(const GLuint nShaderType, const string nShaderCode);

public:
	CGLSLProgram();
	~CGLSLProgram();

	void enable();
	void disable();

	void create();

	void loadUniformVariables();
	void loadAttributeVariables();

	void addUniform(const string nUniformName);
	void addAttribute(const string nAttributeName);

	GLuint getUniform(const string nUniformName);
	GLuint getAttribute(const string nAttributeName);

	void loadFromFile(const string nFilePath);
	void loadFromString(const string nFileExtension, const string nShaderCode);

	void loadUniformSampler(const string nSamplerName, const GLint &nSamplerValue);
	void loadUniformi(const string nUniformName, const GLint nSize, const GLint *npData);
	void loadUniformf(const string nUniformName, const GLint nSize, const GLfloat *npData);
	void loadUniformMatrix(const string nUniformName, const GLint nSize, const GLfloat *npData);
};