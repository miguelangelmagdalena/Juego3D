#pragma once

#include <AntTweakBar.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>

using std::string;
enum figure_type {PERSPECTIVA, ORTOGONAL};


class CUserInterface
{
private:
	//Instancia de la clase
	static CUserInterface * mInterface; 

	//Menus de AntteawBar
	TwBar *mUserInterface,*mUserInterface2,*mUserInterface3;

	//Matrices para trasladar, escalar y rotar
	float mModelTranslation[3];
	float mModelScale[4];
	float mModelRotation[4];
	
	//Propiedades de visualización del objeto seleccionado
	string file_name;
	string file_name_texture;

	bool  has_uvs;
	bool  activateTexture;
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
	bool  autoRotate;
	int   rotateTime;
	float rotateStart[4];

	//Color de la figura,   pero cuando no tiene iluminación? :P
	float color_lines[3]; 
	float color_fill[3];
	float color_box[3];
	float color_normals[3];

	//Material del objeto seleccionado
	float material_ambiental[3];
	float material_difuso[3];
	float material_especular[3];
	float material_shininess;
	
	//Luz de la escena
	bool	light_active;
	GLfloat ambientLight[3];
	GLfloat diffuseLight[3];
	GLfloat specularLight[3];	
	GLfloat lightPosition[3];

	//Tipo de shading
	bool flat_shading;
	bool phong_shading;

	//Tipo de visón de la cámara
	figure_type view_option;


public:
	



	///Method to obtain the only instance of the calls
	static CUserInterface * Instance();
	~CUserInterface();
	void reshape();
	void show();
	void hide();
	void initialize();
	void setModelTranslation(float *modelTranslation);
	float* getModelTranslation();

	void setModelScale(float *modelScale);
	float* getModelScale();
	void setModelRotation(float *modelRotation);
	float* getModelRotation();
	figure_type getViewOption();
	void setFileName(string name);
	string getFileName();


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

	void   setColor_Lines(float *value);
	float* getColor_Lines();
	void   setColor_Fill(float *value);
	float* getColor_Fill();
	void   setColor_Box(float *value);
	float* getColor_Box();
	void   setColor_Normals(float *value);
	float* getColor_Normals();

	void   setMaterial_Ambiental(float *value);
	float* getMaterial_Ambiental();
	void   setMaterial_Difuso(float *value);
	float* getMaterial_Difuso();
	void   setMaterial_Especular(float *value);
	float* getMaterial_Especular();
	void   setMaterial_Shininess(float value);
	float  getMaterial_Shininess();

	void   setAuto_Rotate(bool value);
	bool   getAuto_Rotate();

	void   setRotate_Time(int value);
	int    getRotate_Time();

	void   setRotate_Start(float* value);
	float* getRotate_Start();
	int	   GetTimeMs();

	void   setLight_Ambiental(float *value);
	float* getLight_Ambiental();
	void   setLight_Difuso(float *value);
	float* getLight_Difuso();
	void   setLight_Especular(float *value);
	float* getLight_Especular();
	void   setLight_Posicion(float *value);
	float* getLight_Posicion();

	bool   getFlat_Shading();
	bool   getPhong_Shading();
	bool   getLight_Active();

	bool   getHas_uvs();
	void   setHas_uvs(bool value);

	bool   getActivateTexture();
	void   setActivateTexture(bool value);

	void setFileNameTexture(string name);
	string getFileNameTexture();

	static void TW_CALL setModelScaleCB(const void *value, void *clientData)    {
      	(void)clientData; // unused
		float scale = *static_cast<const float *>(value);
		float mModelScale2[4]		= {scale, scale, scale,scale};
		static_cast<CUserInterface*>(clientData)-> setModelScale(mModelScale2);
    }
	static void TW_CALL getModelScaleCB(void *value, void *clientData)   {
		(void)clientData; // unused
		*static_cast<float *>(value) = static_cast< CUserInterface *>(clientData)->getModelScale()[3];
    }


private:
	///Private constructor
	CUserInterface(); 
};

