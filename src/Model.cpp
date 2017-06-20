#include "Model.h"


CModel::CModel()
{
	visualizacion_alambrica_front = false;
	visualizacion_alambrica_back  = false;
	visualizacion_alambrica_points_front	= false;
	visualizacion_alambrica_points_back		= false;
	visualizacion_normales_face				= false;
	visualizacion_normales_vertex			= false;
	visualizacion_cull_face_front			= false;
	visualizacion_cull_face_back			= false;
	visualizacion_zbuffer					= true;
	visualizacion_bounding_box				= false;
	has_uvs									= false;
	activateTexture							= false;

	minVertex.x = minVertex.y = minVertex.z = INT_MAX;
	maxVertex.x = maxVertex.y = maxVertex.z = INT_MIN;

	color_lines[0] = 0;
	color_lines[0] = 1;
	color_lines[0] = 1;
	color_lines[0] = 0.2f;

	color_fill[0] = 0.75;
	color_fill[1] = 1; 
	color_fill[2] = 0; 
	color_fill[3] = 0.2f; 

	color_box[0] = 0;
	color_box[1] = 1; 
	color_box[2] = 0; 
	color_box[3] = 0.2f; 

	color_normals[0] = 1; 
	color_normals[1] = 1; 
	color_normals[2] = 1; 
	color_normals[3] = 0.2f; 

	material_ambiental[0] = 0.85;
	material_ambiental[1] = 0.78;
	material_ambiental[2] = 0;
	material_ambiental[3] = 0;
	material_difuso[0] = 0;
	material_difuso[1] = 0;
	material_difuso[2] = 0.60;
	material_difuso[3] = 0;
	material_especular[0] = 0.68;
	material_especular[1] = 0;
	material_especular[2] = 0;
	material_especular[3] = 0;
	material_shininess = 60;

	mTranslation[0] = mTranslation[1] = mTranslation[2] = 0.0f;


	rotateTime = 0;
  
	autoRotate = false;

	rotateStart[0] = 0.0f;
	rotateStart[0] = 0.0f;
	rotateStart[0] = 0.0f;
	rotateStart[0] = 1.0f;

}

CModel::~CModel()
{
	vertices.clear();
	normals.clear();
	normals_vertex.clear();
	uvs.clear();
	faceSizes.clear();
	vertexIndices.clear();
	
}

void CModel::setTranslation(float *translation)
{
	mTranslation[0] = translation[0];
	mTranslation[1] = translation[1];
	mTranslation[2] = translation[2];
}

float* CModel::getTranslation()
{
	return mTranslation;
}
void CModel::setScale(float *scale)
{
	mScale[0] = scale[0];
	mScale[1] = scale[1];
	mScale[2] = scale[2];
}

float* CModel::getScale()
{
	return mScale;
}
void CModel::setRotation(float *rotation)
{
	mRotation[0] = rotation[0];
	mRotation[1] = rotation[1];
	mRotation[2] = rotation[2];
	mRotation[3] = rotation[3];
}

float* CModel::getRotation()
{
	return mRotation;
}

void CModel::setVisualizacion_Alambrica_Front(bool value){
	visualizacion_alambrica_front = value;
}
bool CModel::getVisualizacion_Alambrica_Front(){
	return visualizacion_alambrica_front;
}
void CModel::setVisualizacion_Alambrica_Back(bool value){
	visualizacion_alambrica_back = value;
}
bool CModel::getVisualizacion_Alambrica_Back(){
	return visualizacion_alambrica_back;
}
void CModel::setVisualizacion_Alambrica_Points_Front(bool value){
	visualizacion_alambrica_points_front = value;
}
bool CModel::getVisualizacion_Alambrica_Points_Front(){
	return visualizacion_alambrica_points_front;
}
void CModel::setVisualizacion_Alambrica_Points_Back(bool value){
	visualizacion_alambrica_points_back = value;
}
bool CModel::getVisualizacion_Alambrica_Points_Back(){
	return visualizacion_alambrica_points_back;
}
void CModel::setVisualizacion_Normales_Face(bool value){
	visualizacion_normales_face = value;
}
bool CModel::getVisualizacion_Normales_Face(){
	return visualizacion_normales_face;
}
void CModel::setVisualizacion_Normales_Vertex(bool value){
	visualizacion_normales_vertex = value;
}
bool CModel::getVisualizacion_Normales_Vertex(){
	return visualizacion_normales_vertex;
}
void CModel::setVisualizacion_CullFace_Front(bool value){
	visualizacion_cull_face_front = value;
}
bool CModel::getVisualizacion_CullFace_Front(){
	return visualizacion_cull_face_front;
}
void CModel::setVisualizacion_CullFace_Back(bool value){
	visualizacion_cull_face_back = value;
}
bool CModel::getVisualizacion_CullFace_Back(){
	return visualizacion_cull_face_back;
}
void CModel::setVisualizacion_Zbuffer(bool value){
	visualizacion_zbuffer = value;
}
bool CModel::getVisualizacion_Zbuffer(){
	return visualizacion_zbuffer;
}
void CModel::setVisualizacion_BoundingBox(bool value){
	visualizacion_bounding_box = value;
}
bool CModel::getVisualizacion_BoundingBox(){
	return visualizacion_bounding_box;
}
void   CModel::display_options(){
	if(visualizacion_alambrica_front)
		glPolygonMode(GL_FRONT, GL_LINE);

	if(visualizacion_alambrica_back)
		glPolygonMode(GL_BACK, GL_LINE);

	if(visualizacion_alambrica_points_front){
		glPolygonMode(GL_FRONT, GL_POINT);
		glPointSize(5);
	}

	if(visualizacion_alambrica_points_back){
		glPolygonMode(GL_BACK, GL_POINT);
		glPointSize(5);
	}

	if(!visualizacion_alambrica_front && !visualizacion_alambrica_points_front)
		glPolygonMode(GL_FRONT, GL_FILL);
	if(!visualizacion_alambrica_back && !visualizacion_alambrica_points_back)
		glPolygonMode(GL_BACK, GL_FILL);
	
	if(visualizacion_cull_face_front){
		glEnable(GL_CULL_FACE);  
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
	
	}else{
		glDisable(GL_CULL_FACE);  
	}
	if(visualizacion_cull_face_back){
		glEnable(GL_CULL_FACE);  
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);	
	}
	if(visualizacion_zbuffer){
		glEnable(GL_DEPTH_TEST);
	}else{
		glDisable(GL_DEPTH_TEST);
	}



}
void CModel::show_normals(){
	if(visualizacion_normales_face){
		glBindVertexArray(VAO_normal1);
		glDrawArrays(GL_LINES, 0, cantidad_normal1);
	}

	if(visualizacion_normales_vertex){
		glBindVertexArray(VAO_normal2);
		glDrawArrays(GL_LINES, 0, cantidad_normal2);
	}
}

void   CModel::setColor_Lines(float *value){
	color_lines[0] = value[0];
	color_lines[1] = value[1];
	color_lines[2] = value[2];
}
float* CModel::getColor_Lines(){
	return color_lines;
}

void   CModel::setColor_Fill(float *value){
	color_fill[0] = value[0];
	color_fill[1] = value[1];
	color_fill[2] = value[2];
}
float* CModel::getColor_Fill(){
	return color_fill;
}

void   CModel::setColor_Box(float *value){
	color_box[0] = value[0];
	color_box[1] = value[1];
	color_box[2] = value[2];
}
float* CModel::getColor_Box(){
	return color_box;
}

void   CModel::setColor_Normals(float *value){
	color_normals[0] = value[0];
	color_normals[1] = value[1];
	color_normals[2] = value[2];
}
float* CModel::getColor_Normals(){
	return color_normals;
}


void   CModel::setMaterial_Ambiental(float *value){
	material_ambiental[0] = value[0];
	material_ambiental[1] = value[1];
	material_ambiental[2] = value[2];
}
GLfloat* CModel::getMaterial_Ambiental(){
	return material_ambiental;
}
void   CModel::setMaterial_Difuso(float *value){
	material_difuso[0] = value[0];
	material_difuso[1] = value[1];
	material_difuso[2] = value[2];
}
GLfloat* CModel::getMaterial_Difuso(){
	return material_difuso;
}
void   CModel::setMaterial_Especular(float *value){
	material_especular[0] = value[0];
	material_especular[1] = value[1];
	material_especular[2] = value[2];
}
GLfloat* CModel::getMaterial_Especular(){
	return material_especular;
}
void   CModel::setMaterial_Shininess(float value){
	material_shininess = value;
}
float  CModel::getMaterial_Shininess(){
	return material_shininess;
}
void   CModel::setAuto_Rotate(bool value){
	autoRotate = value;
}
bool   CModel::getAuto_Rotate(){
	return autoRotate;
}
void   CModel::setRotate_Time(int value){
	rotateTime = value;
}
int    CModel::getRotate_Time(){
	return rotateTime;
}

void   CModel::setRotate_Start(float* value){
	rotateStart[0] = value[0];
	rotateStart[1] = value[1];
	rotateStart[2] = value[2];
	rotateStart[3] = value[3];
}
float* CModel::getRotate_Start(){
	return rotateStart;
}
void   CModel::setTimeMs(int value){
	timeMs = value;
}
int    CModel::getTimeMs(){
	return timeMs;
}
void   CModel::calculateBoundingBox(){
	//------------------->Calculamos el bounding box
	vector < float > box;
	//Front
	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(minVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);

	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(minVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(minVertex.z);

	//Back
	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(maxVertex.z);

	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	
	//Piso
	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);

	box.push_back(minVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);

	//Techo
	box.push_back(maxVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(minVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(minVertex.z);

	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(minVertex.z);

	//Derecha
	box.push_back(maxVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(minVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);


	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(maxVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);

	//Izquierda
	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(minVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);

	box.push_back(minVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(maxVertex.y);
	box.push_back(maxVertex.z);
	box.push_back(minVertex.x); 
	box.push_back(minVertex.y);
	box.push_back(minVertex.z);
	cantidad_vertex_bounding_box = box.size();
	
	//Vertex Buffer Objects VBO
	glGenVertexArrays(1, &VAO_bounding_box);
	glGenBuffers(1, &VBO_bounding_box); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bounding_box);
	glBufferData(GL_ARRAY_BUFFER, box.size() *sizeof(float), &box.front(), GL_STATIC_DRAW);
    glBindVertexArray(VAO_bounding_box);

	 // position attribute
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
void CModel::show_bounding_box(){
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	glBindVertexArray(VAO_bounding_box);
	glDrawArrays(GL_TRIANGLES, 0, cantidad_vertex_bounding_box/3);
	//glDrawArrays(GL_QUADS, 0, cantidad_vertex_bounding_box);
}
bool   CModel::getHas_uvs(){
	return has_uvs;
}
int    CModel::getTexture(){
	return texture;
}
void   CModel::setTexture(unsigned int value){
	texture = value;
}
bool   CModel::getActivateTexture(){
	return activateTexture;
}

void   CModel::setActivateTexture(bool value){
	activateTexture = value;
}