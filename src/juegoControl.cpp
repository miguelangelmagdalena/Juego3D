#include "juegoControl.h"

juegoControl::juegoControl(int width, int height,FreeTypeFont *text, skyBox *skyBox1,vector<vector<string>> skies, vector <unsigned int> texture){
	this->text		= text;
	this->skyBox1	= skyBox1;
	this->skies		= skies;
	this->texture	= texture;
	SoundEngine 	= createIrrKlangDevice();
	level_actual 	= 0;
	index			= -1;
}
juegoControl::~juegoControl(){
	int i,j;
	for( i= 0; i<max_level; i++){
		nivel[i].clear();
	}
	for (i=0; i<skies.size(); i++){
		for (j=0; j<skies[i].size(); j++){
			skies[i][j].clear();
		}
	}
	texture.clear();

}
bool juegoControl::agregar_objeto(vector <CModel *> models, string file_name, int index_texture, float tx, float ty, float tz, float sx, float sy, float sz, float rx, float ry, float rz, float rw){
	Obj* obj_object = new Obj();
	if(!obj_object->load(file_name))
		return false;
	models.push_back(obj_object);
	index++;

	//Centramos el objeto
	float mModelTranslation[3]	= {models[index]->getTranslation()[0] + tx,models[index]->getTranslation()[1] +  ty,models[index]->getTranslation()[2] +  tz}; 
	float mModelScale[3]		= {sx,sy,sz}; 
	float mModelRotation[4]		= {rx,ry,rz,rw};

	models[index]->setTranslation	(mModelTranslation);
	models[index]->setScale			(mModelScale);
	models[index]->setRotation		(mModelRotation);

	//userInterface -> setActivateTexture(true);
	//userInterface -> setAuto_Rotate(false);

	float ambient	[] = { 0.0215, 0.1745, 0.0215};
	float difuso	[] = { 0.07568, 0.61424, 0.07568};
	float especular	[] = { 0.633, 0.727811, 0.633};
	float shininess = 0.6;

	models[index]->setMaterial_Ambiental					(ambient);
	models[index]->setMaterial_Difuso						(difuso);
	models[index]->setMaterial_Especular					(especular);
	models[index]->setMaterial_Shininess					(shininess);
	models[index]->setAuto_Rotate							(false);
	//models[index]->setRotate_Start						(userInterface->getRotate_Start());
	//models[index]->setTimeMs								(userInterface->GetTimeMs());
	//models[index]->setRotate_Time							(userInterface->getRotate_Time());

	models[index]->setActivateTexture						(true);
	//userInterface->setHas_uvs								(models[index]->getHas_uvs());
	models[index]->setTexture								(index_texture); 
	

	return true;

}

void juegoControl::initScene1(){
	

	//SoundEngine->play2D("../sounds/breakout.mp3", GL_TRUE);
	SoundEngine->play2D("..\\sounds\\Dragon Ball Z - Camino de La Serpiente (Ost).mp3", GL_TRUE);

	printf("\n         --**Juego El Laberinto de Cajas**--\n\n");
	printf(" - Debes encontrar el portal que te llevará al siguiente nivel...\n\n");

	printf(" - Controles : \n");
	printf("    - Desplazamiento: W,A,S,D\n    - Alt: Activar camara estatica\n    - Ctrl: Activar movilidad de camara con mouse\n    - Desplazar camara: flechas de dirección\n    - ESC: Salir del juego\n\n");

	int tam_laberinto = 3;
	int texture_wall = 2;
	index = -1;
	
	//-----------------------------------Suelo
	ground = 0;
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[level_actual], 0,0,0,20,0.0777,20,0,0,0,0);
	float mModelTranslation[3]	= {-nivel[level_actual][index]->init_center.x, -nivel[level_actual][index]->init_center.y - 7, -nivel[level_actual][index]->init_center.z}; 
	nivel[level_actual][ground]->setTranslation(mModelTranslation);
	//agregar_objeto("../models/ground1.obj", level_actual, 0,0,0,10,0.0777,10,3,0,0,0,0);// Fino porque estan dentro del cubo
	//-----------------------------------LLegada
	finish = 1;
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[max_level+1],  -6,-4.1,-6,1,0.108,1,0,0,0,0);
	
	//Laberinto
	tam_laberinto = 3;
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], tam_laberinto,	0,-tam_laberinto ,1,1,1,0,0,0,0); //Esquinas
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], tam_laberinto,	0,+tam_laberinto ,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], -tam_laberinto,	0,-tam_laberinto ,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], -tam_laberinto,	0,+tam_laberinto ,1,1,1,0,0,0,0);
	for(int i = -tam_laberinto+1; i<tam_laberinto; i++){ //Rango index esquinas 2 - 5 ... (5*4) -> 6 - 19
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], i,	0,-tam_laberinto ,1,1,1,0,0,0,0); //Borde Arriba
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], i,	0, tam_laberinto ,1,1,1,0,0,0,0); //Borde Abajo
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall],-tam_laberinto,	0,i	,1,1,1,0,0,0,0);  //Borde Izquierda
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], tam_laberinto,	0,i	,1,1,1,0,0,0,0);  //Borde Derecha
	}
	tam_laberinto = 5;
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], tam_laberinto,	0,-tam_laberinto ,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], tam_laberinto,	0,+tam_laberinto ,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], -tam_laberinto,	0,-tam_laberinto ,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], -tam_laberinto,	0,+tam_laberinto ,1,1,1,0,0,0,0);
	for(int i = -tam_laberinto+1; i<tam_laberinto; i++){
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], i,	0,-tam_laberinto ,1,1,1,0,0,0,0); //Borde Arriba
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], i,	0, tam_laberinto ,1,1,1,0,0,0,0); //Borde Abajo
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall],-tam_laberinto,	0,i	,1,1,1,0,0,0,0);  //Borde Izquierda
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], tam_laberinto,	0,i	,1,1,1,0,0,0,0);  //Borde Derecha
	}
	tam_laberinto = 7;
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], tam_laberinto,	0,-tam_laberinto ,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], tam_laberinto,	0,+tam_laberinto ,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], -tam_laberinto,	0,-tam_laberinto ,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], -tam_laberinto,	0,+tam_laberinto ,1,1,1,0,0,0,0);
	for(int i = -tam_laberinto+1; i<tam_laberinto; i++){
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], i,	0,-tam_laberinto ,1,1,1,0,0,0,0); //Borde Arriba
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], i,	0, tam_laberinto ,1,1,1,0,0,0,0); //Borde Abajo
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall],-tam_laberinto,	0,i	,1,1,1,0,0,0,0);  //Borde Izquierda
		agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], tam_laberinto,	0,i	,1,1,1,0,0,0,0);  //Borde Derecha
	}

	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall],  1,	0	,0	,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], -1,	0	,0	,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], -2,	0	,0	,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall],  0,	0	,-1	,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall],  1,	0	,-1	,1,1,1,0,0,0,0);
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[texture_wall], -1,	0	,-1	,1,1,1,0,0,0,0);

	nivel[level_actual].erase(nivel[level_actual].begin() + 20);
	index--;

	nivel[level_actual].erase(nivel[level_actual].begin() + 40);
	index--;

	//-----------------------------------Jugador
	agregar_objeto(nivel[level_actual],"../models/ground1.obj", texture[5], 0,0,0,1,1,1,0,0,0,0);
	//-----------------------------------Jugador



}

/*void juegoControl::display(){
	switch (level_actual)	{
		case 1:

		break;

		default:
		break;
	}
}*/