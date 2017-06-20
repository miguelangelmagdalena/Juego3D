#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <glm/gtx/norm.hpp>
#include <string>
#include <vector>



using namespace std;

class CModel
{
	protected:
		vector < glm::vec3 >	vertices;
		vector < glm::vec3 >	normals;
		vector < glm::vec3 >	normals_vertex;
		vector < glm::vec2 >	uvs;
		vector < int >			faceSizes;
		vector < unsigned int > vertexIndices;
		bool has_uvs;

		bool  visualizacion_alambrica_front;
		bool  visualizacion_alambrica_back;
		bool  visualizacion_alambrica_points_front;
		bool  visualizacion_alambrica_points_back;
		bool  visualizacion_normales_face;
		bool  visualizacion_normales_vertex;
		bool  visualizacion_cull_face_front;
		bool  visualizacion_cull_face_back;
		bool  visualizacion_zbuffer;
		bool  visualizacion_bounding_box;
		bool  activateTexture;

		float *mVertices; 
		float mTranslation[3]; 
		float mScale[3]; 
		float mRotation[4];
		
		

		float color_lines[3], color_fill[3], color_box[3], color_normals[3];

		GLfloat material_ambiental[3];
		GLfloat material_difuso[3];
		GLfloat material_especular[3];
		float   material_shininess;
		
		bool  autoRotate;
		int   rotateTime;
		float rotateStart[4];

		int   timeMs;

		

		unsigned int VBO, VAO, VBO_normal1, VAO_normal1, VBO_normal2, VAO_normal2, VBO_bounding_box, VAO_bounding_box;
		//int cantidad_triangulos,cantidad_normal1,cantidad_normal2,cantidad_vertex_bounding_box;
		int cantidad_triangulos,cantidad_normal1,cantidad_normal2;

		unsigned int texture;

	public:
		glm::vec3 minVertex;
		glm::vec3 maxVertex;

		glm::vec3	init_center;
		float		init_scale;

		int cantidad_vertex_bounding_box;

		CModel();
		~CModel();
		virtual bool load(string path) = 0;
		virtual void display() = 0;
		virtual void calculate_normals() = 0;
		void   setTranslation(float *translation);
		float* getTranslation();
		void   setScale(float *scale);
		float* getScale();
		void   setRotation(float *rotation);
		float* getRotation();

		void   setVisualizacion_Alambrica_Front(bool value);
		bool   getVisualizacion_Alambrica_Front();
		void   setVisualizacion_Alambrica_Back(bool value);
		bool   getVisualizacion_Alambrica_Back();

		void   setVisualizacion_Alambrica_Points_Front(bool value);
		bool   getVisualizacion_Alambrica_Points_Front();
		void   setVisualizacion_Alambrica_Points_Back(bool value);
		bool   getVisualizacion_Alambrica_Points_Back();
		void   setVisualizacion_Normales_Face(bool value);
		bool   getVisualizacion_Normales_Face();
		void   setVisualizacion_Normales_Vertex(bool value);
		bool   getVisualizacion_Normales_Vertex();
		void   setVisualizacion_CullFace_Front(bool value);
		bool   getVisualizacion_CullFace_Front();
		void   setVisualizacion_CullFace_Back(bool value);
		bool   getVisualizacion_CullFace_Back();
		void   setVisualizacion_Zbuffer(bool value);
		bool   getVisualizacion_Zbuffer();
		void   setVisualizacion_BoundingBox(bool value);
		bool   getVisualizacion_BoundingBox();

		void   display_options();
		void   show_normals();
		void   show_bounding_box();

		void   setColor_Lines(float *value);
		float* getColor_Lines();
		void   setColor_Fill(float *value);
		float* getColor_Fill();
		void   setColor_Box(float *value);
		float* getColor_Box();
		void   setColor_Normals(float *value);
		float* getColor_Normals();

		void   setMaterial_Ambiental(float *value);
		GLfloat* getMaterial_Ambiental();
		void   setMaterial_Difuso(float *value);
		GLfloat* getMaterial_Difuso();
		void   setMaterial_Especular(float *value);
		GLfloat* getMaterial_Especular();
		void   setMaterial_Shininess(float value);
		float  getMaterial_Shininess();




		void   setAuto_Rotate(bool value);
		bool   getAuto_Rotate();
		void   setRotate_Time(int value);
		int    getRotate_Time();
		void   setRotate_Start(float* value);
		float* getRotate_Start();

		void   setTimeMs(int value);
		int    getTimeMs();

		void   calculateBoundingBox();

		bool   getHas_uvs();

		int    getTexture();
		void   setTexture(unsigned int value);

		bool   getActivateTexture();
		void   setActivateTexture(bool value);
};