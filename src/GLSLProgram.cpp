#include "GLSLProgram.h"

namespace GShader
{
	struct GLSLShaderFileExtension Extensions[9] = 
	{
		{ ".vs", GShader::VERTEX },
		{ ".cs", GShader::COMPUTE },
		{ ".vert", GShader::VERTEX },
		{ ".gs", GShader::GEOMETRY },
		{ ".fs", GShader::FRAGMENT },
		{ ".geom", GShader::GEOMETRY },
		{ ".frag", GShader::FRAGMENT },
		{ ".tcs", GShader::TESSELATION_CONTROL },
		{ ".tes", GShader::TESSELATION_EVALUATION }
	};
};

CGLSLProgram::CGLSLProgram()
{
	mShaders[GShader::VERTEX_POSITION] = 0;
	mShaders[GShader::COMPUTE_POSITION] = 0;
	mShaders[GShader::FRAGMENT_POSITION] = 0;
	mShaders[GShader::GEOMETRY_POSITION] = 0;
	mShaders[GShader::TESSELATION_CONTROL_POSITION] = 0;
	mShaders[GShader::TESSELATION_EVALUATION_POSITION] = 0;
}

CGLSLProgram::~CGLSLProgram()
{
	mUniformsList.clear();
	mAttributesList.clear();
	glDeleteProgram(mProgram);
}

void CGLSLProgram::enable()
{
	glUseProgram(mProgram);
}

void CGLSLProgram::disable()
{
	glUseProgram(0);
}

void CGLSLProgram::create()
{
	GLint programLinkStatus;

	mProgram = glCreateProgram();

	glAttachShader(mProgram, mShaders[GShader::VERTEX_POSITION]);
	glAttachShader(mProgram, mShaders[GShader::COMPUTE_POSITION]);
	glAttachShader(mProgram, mShaders[GShader::FRAGMENT_POSITION]);
	glAttachShader(mProgram, mShaders[GShader::GEOMETRY_POSITION]);
	glAttachShader(mProgram, mShaders[GShader::TESSELATION_CONTROL_POSITION]);
	glAttachShader(mProgram, mShaders[GShader::TESSELATION_EVALUATION_POSITION]);

	glLinkProgram(mProgram);
	
	
	glUseProgram(mProgram); // Para usar el shader
	//glGetProgramiv(mProgram, GL_LINK_STATUS, &programLinkStatus);

	if (!programLinkStatus)
	{
		GLint infoLogLength;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(mProgram, infoLogLength, NULL, infoLog);
		//printf("Compile Log : %s\n", infoLog);
		delete[] infoLog;
	}
	else
	{
		//printf("The program was successfully created!!\n");
		glDeleteShader(mShaders[GShader::VERTEX_POSITION]);
		glDeleteShader(mShaders[GShader::COMPUTE_POSITION]);
		glDeleteShader(mShaders[GShader::FRAGMENT_POSITION]);
		glDeleteShader(mShaders[GShader::GEOMETRY_POSITION]);
		glDeleteShader(mShaders[GShader::TESSELATION_CONTROL_POSITION]);
		glDeleteShader(mShaders[GShader::TESSELATION_EVALUATION_POSITION]);
	}
}

void CGLSLProgram::loadUniformVariables()
{
	GLenum uniformVariableType;
	GLint uniformMaxLength, numberOfUniforms, uniformVariableSize;

	glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &numberOfUniforms);
	glGetProgramiv(mProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);

	GLchar *uniformName = new GLchar[uniformMaxLength];

	//printf("\nActive Uniforms\n---------------------------------------------\nID  -  Name  -  Type\n\n");

	for (int k = 0; k < numberOfUniforms; ++k){

		glGetActiveUniform(mProgram, k, uniformMaxLength + 1, NULL, &uniformVariableSize, &uniformVariableType, uniformName);
		addUniform(uniformName);
		//printf("%d | %s (%s)\n", mUniformsList[uniformName], uniformName, getTypeString(uniformVariableType).c_str());

	}

	//printf("\n---------------------------------------------\n");
}

void CGLSLProgram::loadAttributeVariables()
{
	GLenum attributeVariableType;
	GLint attributeMaxLength, numberOfAttributes, numberOfWrittenChars, attributeVariableSize;

	glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTES, &numberOfAttributes);
	glGetProgramiv(mProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeMaxLength);

	GLchar * attributeName = new GLchar[attributeMaxLength];

	//printf("\nActive Attributes\n---------------------------------------------\nID  -  Name  -  Type\n\n");

	for (int k = 0; k < numberOfAttributes; ++k){

		glGetActiveAttrib(mProgram, k, attributeMaxLength + 1, &numberOfWrittenChars, &attributeVariableSize, &attributeVariableType, attributeName);
		addAttribute(attributeName);

		//printf("%d | %s (%s)\n", mAttributesList[attributeName], attributeName, getTypeString(attributeVariableType).c_str());

	}

	//printf("\n---------------------------------------------\n");
}

void CGLSLProgram::addUniform(const string name)
{
	mUniformsList[name] = glGetUniformLocation(mProgram, name.c_str());
}

void CGLSLProgram::addAttribute(const string name)
{
	mAttributesList[name] = glGetAttribLocation(mProgram, name.c_str());
}

GLuint CGLSLProgram::getUniform(const string name)
{
	return mUniformsList[name];
}

GLuint CGLSLProgram::getAttribute(const string name)
{
	return mAttributesList[name];
}

void CGLSLProgram::loadFromFile(const string filePath)
{
	fstream shaderFile;
	string shaderLine, shaderCode;

	//printf("Shader: %s \n",filePath.c_str());
	shaderFile.open(filePath, ios::in);

	if (shaderFile)	{

		while (getline(shaderFile, shaderLine))
			shaderCode.append(shaderLine + "\n");

		compileShader(getShaderType(filePath), shaderCode);

	}
	else
		//printf("The shader file doesnt exist !!\n");

	shaderFile.close();
}

string CGLSLProgram::getTypeString(const GLenum nType)
{
	switch (nType)
	{
		case GL_FLOAT:
			return "float";
			break;
		case GL_FLOAT_VEC2:
			return "vec2";
			break;
		case GL_FLOAT_VEC3:
			return "vec3";
			break;
		case GL_FLOAT_VEC4:
			return "vec4";
			break;
		case GL_DOUBLE:
			return "double";
			break;
		case GL_INT:
			return "int";
			break;
		case GL_UNSIGNED_INT:
			return "unsigned int";
			break;
		case GL_BOOL:
			return "bool";
			break;
		case GL_FLOAT_MAT2:
			return "mat2";
			break;
		case GL_FLOAT_MAT3:
			return "mat3";
			break;
		case GL_FLOAT_MAT4:
			return "mat4";
			break;
		case GL_SAMPLER_2D:
			return "sampler2D";
			break;
		case GL_SAMPLER_2D_SHADOW:
			return "sampler2DShadow";
			break;
		case GL_SAMPLER_CUBE:
			return "samplerCube";
			break;
		default:
			return "?";
			break;
	}
}

GLuint CGLSLProgram::getShaderType(const string fileName)
{
	string fileExtension = getFileExtension(fileName);
	GLuint numberOfExtensions, shaderType;

	shaderType = GShader::VERTEX;
	numberOfExtensions = sizeof(GShader::Extensions) / sizeof(GShader::GLSLShaderFileExtension);

	for (int k = 0; k < (GLint)numberOfExtensions; ++k)
		if (fileExtension == GShader::Extensions[k].mExtension)
		{
			shaderType = GShader::Extensions[k].mType;
			break;
		}

	return shaderType;
}

string CGLSLProgram::getFileExtension(const string nFilePath)
{
	size_t dotLoc = nFilePath.find_last_of('.');
	return nFilePath.substr(dotLoc, string::npos);
}

void CGLSLProgram::compileShader(const GLuint nShaderType, const string nShaderCode)
{
	GLint shaderCompilationStatus, shader;
	const GLchar * pCode = nShaderCode.c_str();

	shader = glCreateShader(nShaderType);
	glShaderSource(shader, 1, &pCode, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompilationStatus);

	if (!shaderCompilationStatus)
	{
		GLchar * infoLog;
		GLint infoLogLength;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		infoLog = new GLchar[infoLogLength];

		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		//printf("Compile Log : %s\n", infoLog);
		delete[] infoLog;
	}
	else
	{
		if (nShaderType == GShader::VERTEX)
		{
			//printf("Creating Vertex Shader...\n");
			mShaders[GShader::VERTEX_POSITION] = shader;
		}
		else if (nShaderType == GShader::COMPUTE)
		{
			//printf("Creating Compute Shade...r\n");
			mShaders[GShader::COMPUTE_POSITION] = shader;
		}
		else if (nShaderType == GShader::FRAGMENT)
		{
			//printf("Creating Fragment Shader...\n");
			mShaders[GShader::FRAGMENT_POSITION] = shader;
		}
		else if (nShaderType == GShader::GEOMETRY)
		{
			//printf("Creating Geometry Shader...\n");
			mShaders[GShader::GEOMETRY_POSITION] = shader;
		}
		else if (nShaderType == GShader::TESSELATION_CONTROL)
		{
			//printf("Creating Tesselation Control Shader...\n");
			mShaders[GShader::TESSELATION_CONTROL_POSITION] = shader;
		}
		else if (nShaderType == GShader::TESSELATION_EVALUATION)
		{
			//printf("Creating Tesselation Evaluation Shader...\n");
			mShaders[GShader::TESSELATION_EVALUATION_POSITION] = shader;
		}
	}
}

void CGLSLProgram::loadFromString(const string fileName, const string shaderCode)
{
	compileShader(getShaderType(fileName), shaderCode);
}

void CGLSLProgram::loadUniformSampler(const string samplerName, const GLint &samplerValue)
{
	glUniform1i(getUniform(samplerName), samplerValue);
}

void CGLSLProgram::loadUniformi(const string name, const GLint size, const GLint *pointer)
{
	switch (size)
	{
		case GShader::UNIFORM_SIZE_1D:
			glUniform1i(getUniform(name), pointer[0]);
			break;
		case GShader::UNIFORM_SIZE_2D:
			glUniform2i(getUniform(name), pointer[0], pointer[1]);
			break;
		case GShader::UNIFORM_SIZE_3D:
			glUniform3i(getUniform(name), pointer[0], pointer[1], pointer[2]);
			break;
		case GShader::UNIFORM_SIZE_4D:
			glUniform4i(getUniform(name), pointer[0], pointer[1], pointer[2], pointer[3]);
			break;
	}
}

void CGLSLProgram::loadUniformf(const string name, const GLint size, const GLfloat* pointer)
{
	switch (size)
	{
		case GShader::UNIFORM_SIZE_1D:
			glUniform1f(getUniform(name), pointer[0]);
			break;
		case GShader::UNIFORM_SIZE_2D:
			glUniform2f(getUniform(name), pointer[0], pointer[1]);
			break;
		case GShader::UNIFORM_SIZE_3D:
			glUniform3f(getUniform(name), pointer[0], pointer[1], pointer[2]);
			break;
		case GShader::UNIFORM_SIZE_4D:
			glUniform4f(getUniform(name), pointer[0], pointer[1], pointer[2], pointer[3]);
			break;
	}
}

void CGLSLProgram::loadUniformMatrix(const string name, const GLint size, const GLfloat* pointer)
{
	switch (size)
	{
		case GShader::UNIFORM_SIZE_3X3:
			glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, pointer);
			break;
		case GShader::UNIFORM_SIZE_4X4:
			glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, pointer);
			break;
	}
}