#include "Light.h"


Light::Light()
{
	visualizacion_alambrica_front = true;
	visualizacion_alambrica_back  = true;

	color_fill[0] = 0.3;
	color_fill[1] = 0.3; 
	color_fill[2] = 0.85; 
	color_fill[3] = 0.2f; 

	float mModelTranslation2[3]	= {-3.76f, -2.43f, -0.77f}; //WHY??
	float mModelScale2[4]		= {.425f, .425f, .425f, .425f};
	float mModelRotation2[4]	= {0, 0, 0, 1};

	/*
	float mModelTranslation2[3]	= {-2.7f, 3.7f, 2.1f}; //WHY??
	float mModelScale2[4]		= {1.0f, 1.0f, 1.0f, 1.0f};
	float mModelRotation2[4]	= {-0.64, -0.36, 0.51, 0.50};
	*/
	/*if(getViewOption() == ORTOGONAL)
		mModelTranslation2[2] = 0.0f;*/

	setTranslation	(mModelTranslation2);
	setScale		(mModelScale2);
	setRotation	    (mModelRotation2);

	
}

Light::~Light()
{
}



bool Light::load(string filename){
	char *data;
	int offset;
	int cont_inside_face = 1;
	int value[3];
	int matches = 0, cantidad = 1;
	int cont_line_file = 1;
	enum faceType {Vertex,Vertex_Textures,Vertex_Textures_Normals,Vertex_Normals};
	faceType tipo = Vertex;
	
	char line[256];
    ifstream file(filename);    
	
	
	//has_uvs = true;
	//has_normal = true;

	    

    if(!file.is_open()) {
		printf("Error, no se puede abrir el archivo %s\n",filename.c_str());
        return false;
    }
    
    while(!file.eof())    { //Leemos el archivo
        file.getline(line,256);

		if ( line[0] == 'v' && line[1] == ' ' ){  //Vertex
			glm::vec3 vertex;
			sscanf_s(line, "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			//printf("v %f %f %f\n",vertex.x,vertex.y,vertex.z);

			if(vertex.x > maxVertex.x) maxVertex.x = vertex.x;
			if(vertex.y > maxVertex.y) maxVertex.y = vertex.y;
			if(vertex.z > maxVertex.z) maxVertex.z = vertex.z;

			if(vertex.x < minVertex.x) minVertex.x = vertex.x;
			if(vertex.y < minVertex.y) minVertex.y = vertex.y;
			if(vertex.z < minVertex.z) minVertex.z = vertex.z;

			vertices.push_back(vertex);
		}else if(line[0] == 'v' && line[1] == 't'){ //Textura
			glm::vec2 uv;
			sscanf_s(line, "vt %f %f\n", &uv.x, &uv.y );
			//printf("vt %f %f\n",uv.x,uv.y);
			uvs.push_back(uv);
		}else if(line[0] == 'v' && line[1] == 'n'){ //Normales
			//glm::vec3 normal;
			//sscanf_s(line, "%vn %f %f %f\n", &normal.x, &normal.y, &normal.z );
			//printf("vn %f %f %f\n",normal.x,normal.y,normal.z);
			//normals.push_back(normal);

		}else if(line[0] == 'f' && line[1] == ' '){ //Indices
			/*if(uvs.empty()){
				has_uvs = false;
			}*/

			/*if(normals.empty()){
				has_normal = false;
			}*/

			//printf("%s\n",line);


			data = line;
			cont_inside_face = 0;

			
			//Format 3: Vertex Normal Indices
			//f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
			cantidad	= 3;
			matches = sscanf_s(data, "f %d/%d/%d%n", &value[0],&value[1],&value[2], &offset);
			if(cantidad == matches){
				tipo		= Vertex_Textures_Normals;	
			}else{
					//Format 4: Vertex Normal Indices Without Texture Coordinate Indices
					//f v1//vn1 v2//vn2 v3//vn3 ...
					cantidad	= 2;
					matches = sscanf_s(data, "f %d//%d %n", &value[0],&value[1], &offset);
				if(cantidad == matches){
					tipo		= Vertex_Normals;
				}else{
					//Format 2: Vertex Texture Coordinate Indices
					//f v1/vt1 v2/vt2 v3/vt3 ...
					cantidad	= 2;
					matches = sscanf_s(data, "f %d/%d %n", &value[0],&value[1], &offset);
					if(cantidad == matches){
						tipo		= Vertex_Textures;
					}else{
						//Format 1: Vertex Indices
						//f v1 v2 v3 ....
						cantidad	= 1;
						matches		= sscanf_s(data, "f %d %n", &value[0], &offset);
						if(cantidad == matches){
							tipo		= Vertex;
						}
					}
				}
			}



			while(matches == cantidad && tipo == Vertex_Textures_Normals){
				//printf("%d/%d/%d ",value[0],value[1],value[2]);
				data += offset;
				cont_inside_face++;

				vertexIndices.push_back(value[0]);
				/*if(has_uvs){
					uvIndices    .push_back(value[1]);
				}*/
				/*if(has_normal){
					normalIndices.push_back(value[2]);
					//printf("%d ",value[2]);
				}*/
				

				matches = sscanf_s(data, " %d/%d/%d%n", &value[0],&value[1],&value[2], &offset);
			}
			//printf("\n");

			while(matches == cantidad && tipo == Vertex_Normals){
				//printf("%d//%d ",value[0],value[1]);
				data += offset;
				cont_inside_face++;

				vertexIndices.push_back(value[0]);
				/*if(has_normal){
					normalIndices.push_back(value[1]);
				}*/

				matches = sscanf_s(data, "%d//%d%n", &value[0],&value[1], &offset);
			}
			while(matches == cantidad && tipo == Vertex_Textures){
				//printf("%d/%d ",value[0],value[1]);
				data += offset;
				cont_inside_face++;

				vertexIndices.push_back(value[0]);
				/*if(has_uvs){
					uvIndices    .push_back(value[1]);
				}*/

				matches = sscanf_s(data, " %d/%d%n", &value[0],&value[1], &offset);
			}
			while(matches == cantidad && tipo == Vertex){
				//printf("%d ",value[0]);
				data += offset;
				cont_inside_face++;

				vertexIndices.push_back(value[0]);

				matches = sscanf_s(data, " %d%n", &value[0], &offset);
			}
			//printf("cont: %d\n",cont_inside_face);
			if(cont_inside_face<3){
				printf("Error en linea %d, debe al menos tener 3 indices de caras\n",cont_line_file);
				file.close();
				return false;
			}

			faceSizes.push_back(cont_inside_face);
			
        }
		cont_line_file++;

    }
	file.close();

	if(vertices.empty()){
		printf("Error, no se leyeron vertices");
		return false;
	}
	if(faceSizes.empty() || vertexIndices.empty()){
		printf("Error, no se leyeron indices de caras");
		return false;
	}
	

	init_center = (maxVertex + minVertex) / 2.f;
	init_scale = 1.f / glm::max(maxVertex.x - minVertex.x, glm::max(maxVertex.y - minVertex.y, maxVertex.z - minVertex.z));
	
	//Calculamos las normales
	
	//calculate_normals();
	//calculate_normals_vertex();

	return true;
	


}


void Light::display(){
	
	int inicial, ultimo;


	/*if(visualizacion_alambrica_front)
		glPolygonMode(GL_FRONT, GL_LINE);

	if(visualizacion_alambrica_back)
		glPolygonMode(GL_BACK, GL_LINE);*/
	display_options();

	//show_normals();

	if(visualizacion_alambrica_front && visualizacion_alambrica_back){
		glColor4fv(color_lines);
	}else{
		glColor4fv(color_fill);
	}


	// Set material properties
	GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);

	for (int i = 0, j=0, n=0; i < faceSizes.size(); i++){
		if(faceSizes[i] == 3){
			glBegin(GL_TRIANGLES);
				for(int cont = 0; cont<faceSizes[i]; j++,cont++){
					//glNormal3f(normals_vertex[vertexIndices[j] - 1].x,normals_vertex[vertexIndices[j] - 1].y,normals_vertex[vertexIndices[j] - 1].z); //Normal by Vertex
					glVertex3f  (vertices[vertexIndices[j] - 1 ].x, vertices[vertexIndices[j] - 1 ].y, vertices[vertexIndices[j] - 1 ].z);

				}
				//glNormal3f(normals[n].x,normals[n].y,normals[n].z); //Normal by face

			glEnd();
			n++;
		}else if(faceSizes[i] >= 4){
			glBegin(GL_TRIANGLES);
				inicial = j;
				ultimo  = inicial + 1;
				j = ultimo + 1;
				for(int cont = 2; cont<faceSizes[i]; cont = cont++){
					//glNormal3f(normals_vertex[vertexIndices[inicial] - 1].x,normals_vertex[vertexIndices[inicial] - 1].y,normals_vertex[vertexIndices[inicial] - 1].z); //Normal by Vertex
					glVertex3f  (vertices[vertexIndices[inicial] - 1 ].x, vertices[vertexIndices[inicial] - 1 ].y, vertices[vertexIndices[inicial] - 1 ].z);
					
					//glNormal3f(normals_vertex[vertexIndices[ultimo] - 1].x,normals_vertex[vertexIndices[ultimo] - 1].y,normals_vertex[vertexIndices[ultimo] - 1].z); //Normal by Vertex
					glVertex3f  (vertices[vertexIndices[ultimo] - 1 ].x, vertices[vertexIndices[ultimo] - 1 ].y, vertices[vertexIndices[ultimo] - 1 ].z);

					//glNormal3f(normals_vertex[vertexIndices[j] - 1].x,normals_vertex[vertexIndices[j] - 1].y,normals_vertex[vertexIndices[j] - 1].z); //Normal by Vertex
					glVertex3f  (vertices[vertexIndices[j] - 1 ].x, vertices[vertexIndices[j] - 1 ].y, vertices[vertexIndices[j] - 1 ].z);

					//glNormal3f(normals[n].x,normals[n].y,normals[n].z); //Normal by face

					n++;
					ultimo = j;
					j++;

				}
			glEnd();
		}
	}
}

void Light::calculate_normals_vertex(){}
void Light::calculate_normals(){}
/*void Obj::calculate_normals_vertex(){
	int i; // Para recorrer todos los vertices
	int k; // Para recorrer todas las caras
	int j; // Para recorrer los indices de vertices por caras
	int n; // Para recorrer las normales por triangulos por cara (1 cara de 3 vertices = 1 triangulo, 1 cara con N vertices = N - 2 triangulos)
	int inicial,ultimo;
	vector < glm::vec3 >	normals_aux;
	glm::vec3 aux;

	

	for (i = 0; i< vertices.size(); i++){
		int cont_apariciones =0;

		

		for(k = 0, j=0 ,n=0; k < faceSizes.size(); k++){
			//printf("%i - cara: %i \n",i,k);

			if(faceSizes[k] == 3){
					
				if( (vertexIndices[j]-1) == i ){ //El vertice esta contenido en esta cara!
					normals_aux.push_back(normals[n]);
				}else if( (vertexIndices[j+1]-1) == i ){ //El vertice esta contenido en esta cara!
					normals_aux.push_back(normals[n]);
				}else if( (vertexIndices[j+2]-1) == i ){ //El vertice esta contenido en esta cara!
					normals_aux.push_back(normals[n]);
				}
				j = j+3;
				n++;
			}else if(faceSizes[k] >= 4){
					inicial = j;
					ultimo  = inicial + 1;
					j = ultimo + 1;

					
					for(int cont = 2; cont<faceSizes[k]; cont = cont++){
						if( (vertexIndices[inicial]-1) == i ){ //El vertice esta contenido en esta cara!
							normals_aux.push_back(normals[n]);
						}else if( (vertexIndices[ultimo]-1) == i ){ //El vertice esta contenido en esta cara!
							normals_aux.push_back(normals[n]);
						}else if( (vertexIndices[j]-1) == i ){ //El vertice esta contenido en esta cara!
							normals_aux.push_back(normals[n]);
						}

						n++;
						ultimo = j;
						j++;

					}
			}

		}

		//Promediamos las normales


		aux = glm::l2Norm(normals_aux[0])*normals_aux[0];
		n = normals_aux.size();

		for(int p = 1; p< n; p++){
			aux += glm::l2Norm(normals_aux[p])*normals_aux[p];
		}
		//aux.x = aux.x * n;
		//aux.y = aux.y * n;
		//aux.y = aux.z * n;
		normals_vertex.push_back(aux);

		normals_aux.clear();
	
	}


}
void Obj::calculate_normals(){
	int inicial, ultimo;
	glm::vec3 A,B,C,norm;

	normalIndices.clear();
	normals.clear();

	for (int i = 0, j=0; i < faceSizes.size(); i++){
		if(faceSizes[i] == 3){
			A = vertices[vertexIndices[j] - 1 ];
			B = vertices[vertexIndices[j+1] - 1 ];
			C = vertices[vertexIndices[j+2] - 1 ];

			norm = glm::cross(B-A,C-A);
			

			normals.push_back(norm );

			j = j+3;

		}else if(faceSizes[i] >= 4){

				inicial = j;
				ultimo  = inicial + 1;
				j = ultimo + 1;
				for(int cont = 2; cont<faceSizes[i]; cont = cont++){
					A = vertices[vertexIndices[inicial] - 1 ];
					B = vertices[vertexIndices[ultimo]  - 1 ];
					C = vertices[vertexIndices[j]       - 1 ];

					norm = glm::cross(B-A,C-A);

					normals.push_back(norm);

					ultimo = j;
					j++;
				}
		}
	}
	
}*/