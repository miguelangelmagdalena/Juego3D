#include "player.h"
#include "juegoControl.h"
Player::Player(string nombre2){
	nombre = nombre2;
	titulos.push_back("El Jugador");
	titulos_colores.push_back(glm::vec3(0.0,0.0,0.0));
	titulo_actual = 0;
	for(int i = 0; i< max_level; i++){
		intentos.push_back(0);
		pistas_usadas.push_back(0);
		luz_usada.push_back(0);
	}
	
}
Player::Player(int a,string nombre){
	titulo_actual = 0;
	for(int i = 0; i< max_level; i++){
		intentos.push_back(0);
		pistas_usadas.push_back(0);
		luz_usada.push_back(0);
	}
	load_data(nombre);
	
}
Player::~Player(){
	intentos.clear();
	pistas_usadas.clear();
	luz_usada.clear();
	titulos.clear();
}

bool Player::load_data(string filename){
	int 	matches;
	char 	text[256];
	int 	aux[4];
	char 	line[256];
    ifstream file(filename); 

    if(!file.is_open()) {
		printf("Error, no se puede abrir el archivo de datos del jugador %s\n",filename.c_str());
        return false;
    }

    while(!file.eof())    {
		file.getline(line,256);
    	if ( line[0] == 'n' && line[1] == ' ' ){  //Nombre
			matches = sscanf_s(line, "n \"%255[^\"]\" \n", text);
			//printf("\nmatches: %d\n",matches);
			//printf("n %s\n",text);
			nombre = text;
		}else if( line[0] == 'd' && line[1] == ' ' ){
			matches = sscanf_s(line, "d %i %i %i %i\n", &aux[0],&aux[1],&aux[2],&aux[3]);
			//printf("\nmatches: %d\n",matches);
			//printf("d %i %i %i %i\n", aux[0],aux[1],aux[2],aux[3]);

			intentos[aux[0] - 1] 		= aux[1];
			pistas_usadas[aux[0] - 1] 	= aux[2];
			luz_usada[aux[0] - 1] 		= aux[3];
		}else if ( line[0] == 't' && line[1] == ' ' ){  //Nombre
			glm::vec3 color;
			matches = sscanf_s(line, "t %f %f %f \"%255[^\"]\" \n",&color.x,&color.y,&color.z, text);
			//printf("\nmatches: %d\n",matches);
			//printf("t %f %f %f %s\n",color.x,color.y,color.z,text);
			titulos.push_back(text);
			titulos_colores.push_back(color);
		}
    }
	cambiar_titulo();

	return true;
}
void Player::save_data(){
	FILE * file;
	string save= "..\\players\\"+nombre + "2.txt";


	file = fopen (save.c_str(),"w");

		fprintf (file, "n \"%s\"\n",nombre.c_str());
		for(int i = 0; i<intentos.size(); i++){
			fprintf (file, "d %i %i %i %i\n",(i+1),intentos[i],pistas_usadas[i],luz_usada[i]);
		}
		for(int i = 0; i<titulos_colores.size(); i++){
			fprintf (file, "t %f %f %f \"%s\"\n",titulos_colores[i].x,titulos_colores[i].y,titulos_colores[i].z,titulos[i].c_str());
		}
	
	fclose (file);
}
void Player::cambiar_titulo(){
	int i = titulo_actual;
	if(i == titulos.size() - 1){
		i = 0;
	}else{
		i++;
	}
	titulo_actual = i;
}
void Player::asignar_titulo(string aux){
	titulos.push_back(aux);
}