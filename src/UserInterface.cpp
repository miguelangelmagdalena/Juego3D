#include "UserInterface.h"

extern int SCREEN_WIDTH, SCREEN_HEIGHT;

// Global static pointer used to ensure a single instance of the class.
CUserInterface * CUserInterface::mInterface = NULL;

/**
* Creates an instance of the class
*
* @return the instance of this class
*/
CUserInterface * CUserInterface::Instance() 
{
	if (!mInterface)   // Only allow one instance of class to be generated.
		 mInterface = new CUserInterface();
	
   return mInterface;
}

void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString){
  destinationClientString = sourceLibraryString;
}
void TW_CALL run_Button_OpenCB(void * /*clientData*/)	{ 
	char filename[ MAX_PATH ];
	OPENFILENAMEA open;

	ZeroMemory(&filename,	sizeof(filename));
	ZeroMemory(&open,		sizeof( open ) );

	open.lStructSize = sizeof(open);
	open.hwndOwner    = NULL; // If you have a window to center over, put its HANDLE here
	open.lpstrFilter = "Obj\0*.obj\0Off\0*.off\0Any File\0*.*\0";
	open.lpstrFile  = filename;
	open.nMaxFile = MAX_PATH;
	open.lpstrTitle = "Selecciona un archivo...";
	open.Flags = OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&open)) {
		CUserInterface::Instance()->setFileName(filename);
	}
	else{

	}
}
void TW_CALL run_Button_TexturaCB(void * /*clientData*/)	{ 
	char filename[ MAX_PATH ];
	OPENFILENAMEA open;

	ZeroMemory(&filename,	sizeof(filename));
	ZeroMemory(&open,		sizeof( open ) );

	open.lStructSize = sizeof(open);
	open.hwndOwner    = NULL; // If you have a window to center over, put its HANDLE here
	open.lpstrFilter = "Jpg\0*.jpg\0Jpeg\0*.jpeg\0Png\0*.png\0Any File\0*.*\0";
	open.lpstrFile  = filename;
	open.nMaxFile = MAX_PATH;
	open.lpstrTitle = "Selecciona un archivo de textura...";
	open.Flags = OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&open)) {
		CUserInterface::Instance()->setFileNameTexture(filename);
	}
	else{

	}
}
void TW_CALL SetAutoRotateCB(const void *value, void *clientData)
{//  Callback function called when the 'AutoRotate' variable value of the tweak bar has changed
    (void)clientData; // unused
	
    bool g_AutoRotate = *(const int *)value; // copy value to g_AutoRotate
	 
	CUserInterface::Instance()->setAuto_Rotate(g_AutoRotate);

	int g_RotateTime;
	float g_RotateStart[4];
	

    if( g_AutoRotate!=0 ) 
    {
        // init rotation
        g_RotateTime		= CUserInterface::Instance()->GetTimeMs();
		CUserInterface::Instance()->setRotate_Time(g_RotateTime);

		g_RotateStart[0]    = CUserInterface::Instance()->getModelRotation()[0];
		g_RotateStart[1]    = CUserInterface::Instance()->getModelRotation()[1];
		g_RotateStart[2]    = CUserInterface::Instance()->getModelRotation()[2];
		g_RotateStart[3]    = CUserInterface::Instance()->getModelRotation()[3];

		CUserInterface::Instance()->setRotate_Start(g_RotateStart);

        // make Rotation variable read-only
        //TwDefine(" TweakBar/ObjRotation readonly ");
    }
    else{
        // make Rotation variable read-write
        //TwDefine(" TweakBar/ObjRotation readwrite ");
	}
}
void TW_CALL GetAutoRotateCB(void *value, void *clientData)
{//  Callback function called by the tweak bar to get the 'AutoRotate' value
    (void)clientData; // unused
	*(int *)value = CUserInterface::Instance()->getAuto_Rotate(); // copy g_AutoRotate to value
}

void TW_CALL material1CB(void * /*clientData*/){ //Gold
	float ambient	[] = { 0.24725, 0.24725, 0.0745};
	float difuso	[] = { 0.75164, 0.60648, 0.22648};
	float especular	[] = { 0.628281, 0.555802, 0.366065};
	float shininess = 0.4;
	CUserInterface::Instance()->setMaterial_Ambiental(ambient);
	CUserInterface::Instance()->setMaterial_Difuso(difuso);
	CUserInterface::Instance()->setMaterial_Especular(especular);
	CUserInterface::Instance()->setMaterial_Shininess(shininess);
}
void TW_CALL material2CB(void * /*clientData*/){ //Esmerald
	float ambient	[] = { 0.0215, 0.1745, 0.0215};
	float difuso	[] = { 0.07568, 0.61424, 0.07568};
	float especular	[] = { 0.633, 0.727811, 0.633};
	float shininess = 0.6;
	CUserInterface::Instance()->setMaterial_Ambiental(ambient);
	CUserInterface::Instance()->setMaterial_Difuso(difuso);
	CUserInterface::Instance()->setMaterial_Especular(especular);
	CUserInterface::Instance()->setMaterial_Shininess(shininess);
}
void TW_CALL material3CB(void * /*clientData*/){ //Cyan plastic
	float ambient	[] = { 0.0, 0.1, 0.06};
	float difuso	[] = { 0.0, 0.50980392, 0.50980392};
	float especular	[] = { 0.50196078, 0.50196078, 0.50196078};
	float shininess = 0.25;
	CUserInterface::Instance()->setMaterial_Ambiental(ambient);
	CUserInterface::Instance()->setMaterial_Difuso(difuso);
	CUserInterface::Instance()->setMaterial_Especular(especular);
	CUserInterface::Instance()->setMaterial_Shininess(shininess);
}
void TW_CALL material4CB(void * /*clientData*/){ //Brass	 
	float ambient	[] = { 0.329412f, 0.223529f, 0.027451f, 1.0f };
	float difuso	[] = { 0.780392f, 0.568627f, 0.113725f, 1.0f };
	float especular	[] = { 0.992157f, 0.941176f, 0.807843f, 1.0f };
	float shininess = 27.8974f;
	CUserInterface::Instance()->setMaterial_Ambiental(ambient);
	CUserInterface::Instance()->setMaterial_Difuso(difuso);
	CUserInterface::Instance()->setMaterial_Especular(especular);
	CUserInterface::Instance()->setMaterial_Shininess(shininess);
}
void TW_CALL material5CB(void * /*clientData*/){ // Bronze
	float ambient	[] = { 0.2125f, 0.1275f, 0.054f, 1.0f };
	float difuso	[] = { 0.714f, 0.4284f, 0.18144f, 1.0f };
	float especular	[] = { 0.393548f, 0.271906f, 0.166721f, 1.0f };
	float shininess = 25.6f;
	CUserInterface::Instance()->setMaterial_Ambiental(ambient);
	CUserInterface::Instance()->setMaterial_Difuso(difuso);
	CUserInterface::Instance()->setMaterial_Especular(especular);
	CUserInterface::Instance()->setMaterial_Shininess(shininess);
}
void TW_CALL material6CB(void * /*clientData*/){ //Chrome	
	float ambient	[] = {0.25f, 0.25f, 0.25f, 1.0f  };
	float difuso	[] = {0.4f, 0.4f, 0.4f, 1.0f };
	float especular	[] = {0.774597f, 0.774597f, 0.774597f, 1.0f };
	float shininess = 76.8f;
	CUserInterface::Instance()->setMaterial_Ambiental(ambient);
	CUserInterface::Instance()->setMaterial_Difuso(difuso);
	CUserInterface::Instance()->setMaterial_Especular(especular);
	CUserInterface::Instance()->setMaterial_Shininess(shininess);
}

CUserInterface::CUserInterface()
{
	view_option = PERSPECTIVA;
	file_name = "Hola";
	file_name_texture = "Hola";
	visualizacion_alambrica_front			= false;
	visualizacion_alambrica_back			= false;
	visualizacion_alambrica_points_front	= false;
	visualizacion_alambrica_points_back		= false;
	visualizacion_normales_face				= false;
	visualizacion_normales_vertex			= false;
	visualizacion_cull_face_front			= false;
	visualizacion_cull_face_back			= false;
	visualizacion_zbuffer					= true;
	visualizacion_bounding_box				= false;
	activateTexture							= false;

	//has_uvs									= false;

	color_lines[0] = 1;
	color_lines[1] = 1;
	color_lines[2] = 0;

	color_fill[0] = 0.75;
	color_fill[1] = 0; 
	color_fill[2] = 0; 

	color_box[0] = 0;
	color_box[1] = 1; 
	color_box[2] = 0; 

	color_normals[0] = 1; 
	color_normals[1] = 1; 
	color_normals[2] = 1; 

	material_ambiental[0] = 0.24725;
	material_ambiental[1] = 0.1995;
	material_ambiental[2] = 0.0745;

	material_difuso[0] = 0.75164;
	material_difuso[1] = 0.60648;
	material_difuso[2] = 0.22648;
	
	material_especular[0] = 0.628281;
	material_especular[1] = 0.555802;
	material_especular[2] = 0.366065;
	
	material_shininess = 0.4;

	// Store 
	rotateTime = 0;
    rotateTime = GetTimeMs();
	autoRotate = false;

	rotateStart[0] = 0.0f;
	rotateStart[1] = 0.0f;
	rotateStart[2] = 0.0f;
	rotateStart[3] = 1.0f;


	//Set lighting intensity and color
	light_active = true;
	GLfloat ambientLight2[]	= {1, 1, 1, 1};
	GLfloat diffuseLight2[]	= {1, 1, 1, 1};
	GLfloat specularLight2[]= {1.0, 1.0, 1.0, 0.1};
	GLfloat lightPosition2[]= {-1.75, 4.16, -0.57};

	setLight_Ambiental(ambientLight2);
	setLight_Difuso(diffuseLight2);
	setLight_Especular(specularLight2);
	setLight_Posicion(lightPosition2);

	flat_shading = false;
	phong_shading = true;

	mUserInterface = TwNewBar("Propiedades");
	TwDefine("Propiedades refresh = '0.0001f'");
	TwDefine("Propiedades resizable = true");
	TwDefine("Propiedades fontresizable = false");
	TwDefine("Propiedades movable = true");
	TwDefine("Propiedades position = '20 20'");
	TwDefine("Propiedades size = '320 720' ");
	TwDefine("Propiedades visible = false ");


	initialize();
	TwEnumVal figures_types[] = {	{ PERSPECTIVA,	"Perspectiva"}, 
									{ ORTOGONAL,	"Ortogonal" }};
    TwType rotationType = TwDefineEnum( "Modo", figures_types, 2);
	
	TwCopyStdStringToClientFunc(CopyStdStringToClient); 
	TwAddButton(mUserInterface, "Abrir",						run_Button_OpenCB,	NULL,									" ");
	TwAddVarRW(mUserInterface, "Nombre Archivo:",		TW_TYPE_STDSTRING,	&file_name,										" help='Direccón del objeto a renderizar'");

	TwAddVarRW(mUserInterface, "Tipo de proyección",			rotationType,		&view_option,							" group='Vista' help='Elegir proyeccion perspectiva u ortogonal.' ");
	TwAddVarRW(mUserInterface, "Modo Alambrico delante",		TW_TYPE_BOOLCPP,	&visualizacion_alambrica_front,			" group='Vista'");
	TwAddVarRW(mUserInterface, "Modo Alambrico detrás",			TW_TYPE_BOOLCPP,	&visualizacion_alambrica_back,			" group='Vista'");
	TwAddSeparator(mUserInterface, NULL, " group='Vista' ");
	TwAddVarRW(mUserInterface, "Modo Alambrico Points Front",	TW_TYPE_BOOLCPP,	&visualizacion_alambrica_points_front,	" group='Vista'");
	TwAddVarRW(mUserInterface, "Modo Alambrico Points Back",	TW_TYPE_BOOLCPP,	&visualizacion_alambrica_points_back,	" group='Vista'");
	TwAddSeparator(mUserInterface, NULL, " group='Vista' ");
	TwAddVarRW(mUserInterface, "Normales por Cara",				TW_TYPE_BOOLCPP,	&visualizacion_normales_face,			" group='Vista'");
	TwAddVarRW(mUserInterface, "Normales por Vertex",			TW_TYPE_BOOLCPP,	&visualizacion_normales_vertex,			" group='Vista'");
	TwAddSeparator(mUserInterface, NULL, " group='Vista' ");
	TwAddVarRW(mUserInterface, "Cull Face Front",				TW_TYPE_BOOLCPP,	&visualizacion_cull_face_front,			" group='Vista'");
	TwAddVarRW(mUserInterface, "Cull Face Back",				TW_TYPE_BOOLCPP,	&visualizacion_cull_face_back,			" group='Vista'");
	TwAddSeparator(mUserInterface, NULL, " group='Vista' ");
	TwAddVarRW(mUserInterface, "Z - buffer",					TW_TYPE_BOOLCPP,	&visualizacion_zbuffer,					" group='Vista'");
	TwAddVarRW(mUserInterface, "Bounding Box",					TW_TYPE_BOOLCPP,	&visualizacion_bounding_box,			" group='Vista'");
	TwAddSeparator(mUserInterface, NULL, " group='Vista' ");
	TwAddVarRW(mUserInterface, "Tiene coord textura",			TW_TYPE_BOOLCPP,	&has_uvs,								" group='Vista' help='Muestra  si el objeto tiene o no coordenadas de textura'");
	TwAddVarRW(mUserInterface, "Activar textura",				TW_TYPE_BOOLCPP,	&activateTexture,						" group='Vista' help='Si el objeto tiene coor textura se podrá visualizar una textura cargada'");
	TwAddButton(mUserInterface,"Buscar textura",				run_Button_TexturaCB, NULL,									" group='Vista' help='Permite buscar una imagen de textura para colocarsela al objeto'");
	TwAddVarRW(mUserInterface, "Nombre textura:",				TW_TYPE_STDSTRING,	&file_name_texture,						" group='Vista' help='Dirección de la textura'");

	TwAddVarRW(mUserInterface, "Color Lineas",					TW_TYPE_COLOR3F,	&color_lines,							" group='Colores'");
	TwAddVarRW(mUserInterface, "Color Relleno",					TW_TYPE_COLOR3F,	&color_fill,							" group='Colores'");
	TwAddVarRW(mUserInterface, "Color Bounding Box",			TW_TYPE_COLOR3F,	&color_box,								" group='Colores'");
	TwAddVarRW(mUserInterface, "Color Normales",				TW_TYPE_COLOR3F,	&color_normals,							" group='Colors'");

	TwAddVarRW(mUserInterface, "Transladar en X",				TW_TYPE_FLOAT,		&mModelTranslation[0],					" group='Transladar'  step=0.1 ");
	TwAddVarRW(mUserInterface, "Transladar en Y",				TW_TYPE_FLOAT,		&mModelTranslation[1],					" group='Transladar'  step=0.1 ");
	TwAddVarRW(mUserInterface, "Transladar en Z",				TW_TYPE_FLOAT,		&mModelTranslation[2],					" group='Transladar'  step=0.1 ");

	TwAddVarRW(mUserInterface, "Escalar en X",					TW_TYPE_FLOAT,		&mModelScale[0],						" group='Escalar'  min=0.0001 max=10 step=0.001");
	TwAddVarRW(mUserInterface, "Escalar en Y",					TW_TYPE_FLOAT,		&mModelScale[1],						" group='Escalar'  min=0.0001 max=10 step=0.001");
	TwAddVarRW(mUserInterface, "Escalar en Z",					TW_TYPE_FLOAT,		&mModelScale[2],						" group='Escalar'  min=0.0001 max=10 step=0.001");
	TwAddVarCB(mUserInterface, "Escalar todos",					TW_TYPE_FLOAT,		CUserInterface::setModelScaleCB, CUserInterface::getModelScaleCB, &mUserInterface," group='Escalar' min=0.0001 max=10 step=0.001");

	TwAddVarRW(mUserInterface, "Rotar",							TW_TYPE_QUAT4F,		&mModelRotation,						" group='Rotación' opened=true help='Cambia la rotación del objeto.' ");
    TwAddVarCB(mUserInterface, "Auto - Rotación",				TW_TYPE_BOOL32,		SetAutoRotateCB, GetAutoRotateCB, &mUserInterface,	" group='Rotación'  key=space help='Activa o desactiva auto rotación' ");

	mUserInterface2 = TwNewBar("Propiedades_Material");
	TwDefine("Propiedades_Material refresh = '0.0001f'");
	TwDefine("Propiedades_Material resizable = true");
	TwDefine("Propiedades_Material fontresizable = false");
	TwDefine("Propiedades_Material movable = true");
	TwDefine("Propiedades_Material position = '670 20'");
	TwDefine("Propiedades_Material size = '320 320' color='96 216 224'");
	TwDefine("Propiedades_Material visible = false ");

	TwAddVarRW(mUserInterface2, "Material Ambiental",			TW_TYPE_COLOR3F,	&material_ambiental,						" group='Material'");
	TwAddVarRW(mUserInterface2, "Material Difuso",				TW_TYPE_COLOR3F,	&material_difuso,							" group='Material'");
	TwAddVarRW(mUserInterface2, "Material Especular",			TW_TYPE_COLOR3F,	&material_especular,						" group='Material'");
	TwAddVarRW(mUserInterface2, "Material Shininess",			TW_TYPE_FLOAT,		&material_shininess,						" group='Material' min=0 max=256 step=0.1");
	TwAddButton(mUserInterface2, "Gold",						material1CB,		NULL,										" group='Algunos Materiales' ");
	TwAddButton(mUserInterface2, "Emerald",						material2CB,		NULL,										" group='Algunos Materiales' ");
	TwAddButton(mUserInterface2, "Cyan plastic",				material3CB,		NULL,										" group='Algunos Materiales' ");
	TwAddButton(mUserInterface2, "Brass",						material4CB,		NULL,										" group='Algunos Materiales' ");
	TwAddButton(mUserInterface2, "Bronze",						material5CB,		NULL,										" group='Algunos Materiales' ");
	TwAddButton(mUserInterface2, "Chrome",						material6CB,		NULL,										" group='Algunos Materiales' ");


	mUserInterface3 = TwNewBar("Propiedades_Luz");
	TwDefine("Propiedades_Luz refresh = '0.0001f'");
	TwDefine("Propiedades_Luz resizable = true");
	TwDefine("Propiedades_Luz fontresizable = false");
	TwDefine("Propiedades_Luz movable = true");
	TwDefine("Propiedades_Luz position = '670 400'");
	TwDefine("Propiedades_Luz size = '320 320' color='96 96 224' ");
	TwDefine("Propiedades_Luz visible = false ");

	TwAddVarRW(mUserInterface3, "Luz Active",					TW_TYPE_BOOLCPP,	&light_active,								" group='Luz'");
	TwAddVarRW(mUserInterface3, "Luz Ambiental",				TW_TYPE_COLOR3F,	&ambientLight,								" group='Luz'");
	TwAddVarRW(mUserInterface3, "Luz Difuso",					TW_TYPE_COLOR3F,	&diffuseLight,								" group='Luz'");
	TwAddVarRW(mUserInterface3, "Luz Especular",				TW_TYPE_COLOR3F,	&specularLight,								" group='Luz'");
	TwAddVarRW(mUserInterface3, "Luz Direccion",				TW_TYPE_DIR3F,		&lightPosition,								" group='Luz' opened=true help='Cambiar la dirección de la luz.' ");
	TwAddVarRW(mUserInterface3, "Flat Shading",					TW_TYPE_BOOLCPP,	&flat_shading,								" group='Luz'");
	TwAddVarRW(mUserInterface3, "Phong Shading",				TW_TYPE_BOOLCPP,	&phong_shading,								" group='Luz'");
}

CUserInterface::~CUserInterface()
{
}



void CUserInterface::reshape()
{
	TwWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void CUserInterface::show()
{
	TwDefine("Figure visible = true");
}

void CUserInterface::hide()
{
	TwDefine("Figure visible = false");
}

void CUserInterface::initialize(){

	// null rotation
	float mModelTranslation2[3]	= {0.0f, 0.0f, -2.0f}; //WHY??
	float mModelScale2[4]		= {1.0f, 1.0f, 1.0f, 1.0f};
	float mModelRotation2[4]	= {0, 0, 0, 1};

	if(getViewOption() == ORTOGONAL)
		mModelTranslation2[2] = 0.0f;

	setModelTranslation	(mModelTranslation2);
	setModelScale		(mModelScale2);
	setModelRotation	(mModelRotation2);
}

void CUserInterface::setModelTranslation(float *modelTranslation)
{
	mModelTranslation[0] = modelTranslation[0];
	mModelTranslation[1] = modelTranslation[1];
	mModelTranslation[2] = modelTranslation[2];
}

float* CUserInterface::getModelTranslation()
{
	return mModelTranslation;
}

void CUserInterface::setModelScale(float *modelScale){
	mModelScale[0] = modelScale[0];
	mModelScale[1] = modelScale[1];
	mModelScale[2] = modelScale[2];
	mModelScale[3] = modelScale[3];
}
float* CUserInterface::getModelScale(){
	return mModelScale;
}

void CUserInterface::setModelRotation(float *modelRotation){
	mModelRotation[0] = modelRotation[0];
	mModelRotation[1] = modelRotation[1];
	mModelRotation[2] = modelRotation[2];
	mModelRotation[3] = modelRotation[3];
}
float* CUserInterface::getModelRotation(){
	return mModelRotation;
}

figure_type CUserInterface::getViewOption(){
	return view_option;
}
void CUserInterface::setFileName(string name){
	file_name = name;
}
string CUserInterface::getFileName(){
	return file_name;
}
void CUserInterface::setVisualizacion_Alambrica_Front(bool value){
	visualizacion_alambrica_front = value;
}
bool CUserInterface::getVisualizacion_Alambrica_Front(){
	return visualizacion_alambrica_front;
}
void CUserInterface::setVisualizacion_Alambrica_Back(bool value){
	visualizacion_alambrica_back = value;
}
bool CUserInterface::getVisualizacion_Alambrica_Back(){
	return visualizacion_alambrica_back;
}
void CUserInterface::setVisualizacion_Alambrica_Points_Front(bool value){
	visualizacion_alambrica_points_front = value;
}
bool CUserInterface::getVisualizacion_Alambrica_Points_Front(){
	return visualizacion_alambrica_points_front;
}
void CUserInterface::setVisualizacion_Alambrica_Points_Back(bool value){
	visualizacion_alambrica_points_back = value;
}
bool CUserInterface::getVisualizacion_Alambrica_Points_Back(){
	return visualizacion_alambrica_points_back;
}
void CUserInterface::setVisualizacion_Normales_Face(bool value){
	visualizacion_normales_face = value;
}
bool CUserInterface::getVisualizacion_Normales_Face(){
	return visualizacion_normales_face;
}
void CUserInterface::setVisualizacion_Normales_Vertex(bool value){
	visualizacion_normales_vertex = value;
}
bool CUserInterface::getVisualizacion_Normales_Vertex(){
	return visualizacion_normales_vertex;
}
void CUserInterface::setVisualizacion_CullFace_Front(bool value){
	visualizacion_cull_face_front = value;
}
bool CUserInterface::getVisualizacion_CullFace_Front(){
	return visualizacion_cull_face_front;
}
void CUserInterface::setVisualizacion_CullFace_Back(bool value){
	visualizacion_cull_face_back = value;
}
bool CUserInterface::getVisualizacion_CullFace_Back(){
	return visualizacion_cull_face_back;
}
void   CUserInterface::setVisualizacion_Zbuffer(bool value){
	visualizacion_zbuffer = value;
}
bool   CUserInterface::getVisualizacion_Zbuffer(){
	return visualizacion_zbuffer;
}
void   CUserInterface::setVisualizacion_BoundingBox(bool value){
	visualizacion_bounding_box = value;
}
bool   CUserInterface::getVisualizacion_BoundingBox(){
	return visualizacion_bounding_box;
}

void   CUserInterface::setColor_Lines(float *value){
	color_lines[0] = value[0];
	color_lines[1] = value[1];
	color_lines[2] = value[2];
	//color_lines[3] = value[3];
}
float* CUserInterface::getColor_Lines(){
	return color_lines;
}

void   CUserInterface::setColor_Fill(float *value){
	color_fill[0] = value[0];
	color_fill[1] = value[1];
	color_fill[2] = value[2];
	//color_fill[3] = value[3];
}
float* CUserInterface::getColor_Fill(){
	return color_fill;
}

void   CUserInterface::setColor_Box(float *value){
	color_box[0] = value[0];
	color_box[1] = value[1];
	color_box[2] = value[2];
	//color_box[3] = value[3];
}
float* CUserInterface::getColor_Box(){
	return color_box;
}

void   CUserInterface::setColor_Normals(float *value){
	color_normals[0] = value[0];
	color_normals[1] = value[1];
	color_normals[2] = value[2];
	//color_normals[3] = value[3];
}
float* CUserInterface::getColor_Normals(){
	return color_normals;
}

void   CUserInterface::setMaterial_Ambiental(float *value){
	material_ambiental[0] = value[0];
	material_ambiental[1] = value[1];
	material_ambiental[2] = value[2];
	//material_ambiental[3] = value[3];
}
float* CUserInterface::getMaterial_Ambiental(){
	return material_ambiental;
}
void   CUserInterface::setMaterial_Difuso(float *value){
	material_difuso[0] = value[0];
	material_difuso[1] = value[1];
	material_difuso[2] = value[2];
	//material_difuso[3] = value[3];
}
float* CUserInterface::getMaterial_Difuso(){
	return material_difuso;
}
void   CUserInterface::setMaterial_Especular(float *value){
	material_especular[0] = value[0];
	material_especular[1] = value[1];
	material_especular[2] = value[2];
	//material_especular[3] = value[3];
}
float* CUserInterface::getMaterial_Especular(){
	return material_especular;
}
void   CUserInterface::setMaterial_Shininess(float value){
	material_shininess = value;
}
float  CUserInterface::getMaterial_Shininess(){
	return material_shininess;
}
void   CUserInterface::setAuto_Rotate(bool value){
	autoRotate = value;
}
bool   CUserInterface::getAuto_Rotate(){
	return autoRotate;
}
void   CUserInterface::setRotate_Time(int value){
	rotateTime = value;
}
int    CUserInterface::getRotate_Time(){
	return rotateTime;
}

void   CUserInterface::setRotate_Start(float* value){
	rotateStart[0] = value[0];
	rotateStart[1] = value[1];
	rotateStart[2] = value[2];
	rotateStart[3] = value[3];
}
float* CUserInterface::getRotate_Start(){
	return rotateStart;
}
// Return elapsed time in milliseconds
int CUserInterface::GetTimeMs()
{
	#if !defined(_WIN32)
		return glutGet(GLUT_ELAPSED_TIME);
	#else
		// glutGet(GLUT_ELAPSED_TIME) seems buggy on Windows
		return (int)GetTickCount(); 
	#endif
}

void   CUserInterface::setLight_Ambiental(GLfloat *value){
	ambientLight[0] = value[0];
	ambientLight[1] = value[1];
	ambientLight[2] = value[2];
	//ambientLight[3] = value[3];

}
GLfloat* CUserInterface::getLight_Ambiental(){
	return ambientLight;
}
void   CUserInterface::setLight_Difuso(GLfloat *value){
	diffuseLight[0] = value[0];
	diffuseLight[1] = value[1];
	diffuseLight[2] = value[2];
	//diffuseLight[3] = value[3];
}
GLfloat* CUserInterface::getLight_Difuso(){
	return diffuseLight;
}
void   CUserInterface::setLight_Especular(GLfloat *value){
	specularLight[0] = value[0];
	specularLight[1] = value[1];
	specularLight[2] = value[2];
	//specularLight[3] = value[3];
}
GLfloat* CUserInterface::getLight_Especular(){
	return specularLight;
}
void   CUserInterface::setLight_Posicion(GLfloat *value){
	lightPosition[0] = value[0];
	lightPosition[1] = value[1];
	lightPosition[2] = value[2];
}
GLfloat* CUserInterface::getLight_Posicion(){
	return lightPosition;
}
bool   CUserInterface::getFlat_Shading(){
	return flat_shading;
}
bool   CUserInterface::getPhong_Shading(){
	return phong_shading;
}
bool   CUserInterface::getLight_Active(){
	return light_active;
}
bool   CUserInterface::getHas_uvs(){
	return has_uvs;
}
void   CUserInterface::setHas_uvs(bool value){
	has_uvs = value;
}
bool   CUserInterface::getActivateTexture(){
	return activateTexture;
}
void   CUserInterface::setActivateTexture(bool value){
	activateTexture = value;
}
void	CUserInterface::setFileNameTexture(string name){
	file_name_texture = name;
}
string	CUserInterface::getFileNameTexture(){
	return file_name_texture;
}