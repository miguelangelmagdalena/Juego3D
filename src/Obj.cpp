#include "Obj.h"

Obj::Obj()
{
}

Obj::~Obj()
{

	uvIndices.clear();
	normalIndices.clear();
}



bool Obj::load(string filename){
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
	
	
	has_uvs = true;
	has_normal = true;

	    

    if(!file.is_open()) {
		printf("Error, no se puede abrir el archivo %s\n",filename.c_str());
        return false;
    }
    
    while(!file.eof())    { //Leemos el archivo
        file.getline(line,256);

		if ( line[0] == 'v' && line[1] == ' ' ){  //Vertex
			glm::vec3 vertex;
			sscanf_s(line, "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z );

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
			
			//uv.y = 1 - uv.y ; //I push back 1-v instead of normal v, because obj file use the upper left corner as 0,0 coorinate







           //but OpenGL use bottom left corner as 0,0, so I convert it

			uvs.push_back(uv);
		}else if(line[0] == 'v' && line[1] == 'n'){ //Normales
			glm::vec3 normal;
			sscanf_s(line, "%vn %f %f %f\n", &normal.x, &normal.y, &normal.z );
			//printf("vn %f %f %f\n",normal.x,normal.y,normal.z);
			normals_vertex.push_back(normal);
		}else if(line[0] == '#'){ //Comentario


		}else if(line[0] == 'f' && line[1] == ' '){ //Indices
			if(uvs.empty()){
				has_uvs = false;
			}
			if(normals_vertex.empty()){
				has_normal = false;

				//printf("\n\n--NO TIENE NORMALES--\n\n");
			}

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
				if(has_uvs){
					uvIndices.push_back(value[1]);
				}
				if(has_normal){
					normalIndices.push_back(value[2]);
					//printf("%d ",value[2]);
				}
				

				matches = sscanf_s(data, " %d/%d/%d%n", &value[0],&value[1],&value[2], &offset);
			}
			//printf("\n");

			while(matches == cantidad && tipo == Vertex_Normals){
				//printf("%d//%d ",value[0],value[1]);
				data += offset;
				cont_inside_face++;

				vertexIndices.push_back(value[0]);
				if(has_normal){
					normalIndices.push_back(value[1]);
				}

				matches = sscanf_s(data, "%d//%d%n", &value[0],&value[1], &offset);
			}
			while(matches == cantidad && tipo == Vertex_Textures){
				//printf("%d/%d ",value[0],value[1]);
				data += offset;
				cont_inside_face++;

				vertexIndices.push_back(value[0]);
				if(has_uvs){
					uvIndices.push_back(value[1]);
				}

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
	

	//------------------->Calculamos las normales
	
	if(!has_normal){
		calculate_normals();
		calculate_normals_vertex();
	}


	//------------------->Pasamos la figura a la gpu
	vector < float > figura;
	int inicial, ultimo;
	cantidad_triangulos = 0;

	if(has_uvs == false){

		for (int i = 0, j=0, n=0; i < faceSizes.size(); i++){
			if(faceSizes[i] == 3){

					for(int cont = 0; cont<faceSizes[i]; j++,cont++){

						figura.push_back(vertices[vertexIndices[j] - 1 ].x);
						figura.push_back(vertices[vertexIndices[j] - 1 ].y);
						figura.push_back(vertices[vertexIndices[j] - 1 ].z);

						if(!has_normal){
							figura.push_back(normals_vertex[vertexIndices[j] - 1].x);
							figura.push_back(normals_vertex[vertexIndices[j] - 1].y);
							figura.push_back(normals_vertex[vertexIndices[j] - 1].z);
						}else{
							figura.push_back(normals_vertex[normalIndices[j] - 1].x);
							figura.push_back(normals_vertex[normalIndices[j] - 1].y);
							figura.push_back(normals_vertex[normalIndices[j] - 1].z);
						}

					}
					cantidad_triangulos++;

				n++;
			}else if(faceSizes[i] >= 4){
					inicial = j;
					ultimo  = inicial + 1;
					j = ultimo + 1;
					for(int cont = 2; cont<faceSizes[i]; cont = cont++){
						figura.push_back(vertices[vertexIndices[inicial] - 1 ].x);
						figura.push_back(vertices[vertexIndices[inicial] - 1 ].y);
						figura.push_back(vertices[vertexIndices[inicial] - 1 ].z);
						if(!has_normal){
							figura.push_back(normals_vertex[vertexIndices[inicial] - 1].x);
							figura.push_back(normals_vertex[vertexIndices[inicial] - 1].y);
							figura.push_back(normals_vertex[vertexIndices[inicial] - 1].z);
						}else{
							figura.push_back(normals_vertex[normalIndices[inicial] - 1].x);
							figura.push_back(normals_vertex[normalIndices[inicial] - 1].y);
							figura.push_back(normals_vertex[normalIndices[inicial] - 1].z);
						}

						figura.push_back(vertices[vertexIndices[ultimo] - 1 ].x);
						figura.push_back(vertices[vertexIndices[ultimo] - 1 ].y);
						figura.push_back(vertices[vertexIndices[ultimo] - 1 ].z);
						if(!has_normal){
							figura.push_back(normals_vertex[vertexIndices[ultimo] - 1].x);
							figura.push_back(normals_vertex[vertexIndices[ultimo] - 1].y);
							figura.push_back(normals_vertex[vertexIndices[ultimo] - 1].z);
						}else{
							figura.push_back(normals_vertex[normalIndices[ultimo] - 1].x);
							figura.push_back(normals_vertex[normalIndices[ultimo] - 1].y);
							figura.push_back(normals_vertex[normalIndices[ultimo] - 1].z);
						}

						figura.push_back(vertices[vertexIndices[j] - 1 ].x);
						figura.push_back(vertices[vertexIndices[j] - 1 ].y);
						figura.push_back(vertices[vertexIndices[j] - 1 ].z);
						if(!has_normal){
							figura.push_back(normals_vertex[vertexIndices[j] - 1].x);
							figura.push_back(normals_vertex[vertexIndices[j] - 1].y);
							figura.push_back(normals_vertex[vertexIndices[j] - 1].z);
						}else{
							figura.push_back(normals_vertex[normalIndices[j] - 1].x);
							figura.push_back(normals_vertex[normalIndices[j] - 1].y);
							figura.push_back(normals_vertex[normalIndices[j] - 1].z);
						}
						n++;
						ultimo = j;
						j++;
						cantidad_triangulos++;

					}
			}
		}

		//Vertex Buffer Objects VBO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO); 
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, figura.size() *sizeof(float), &figura.front(), GL_STATIC_DRAW);
		glBindVertexArray(VAO);

		 // position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    
		// normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	}else if(has_uvs == true){
	
		for (int i = 0, j=0, n=0; i < faceSizes.size(); i++){
			if(faceSizes[i] == 3){

					for(int cont = 0; cont<faceSizes[i]; j++,cont++){

						figura.push_back(vertices[vertexIndices[j] - 1 ].x);
						figura.push_back(vertices[vertexIndices[j] - 1 ].y);
						figura.push_back(vertices[vertexIndices[j] - 1 ].z);

						if(!has_normal){
							figura.push_back(normals_vertex[vertexIndices[j] - 1].x);
							figura.push_back(normals_vertex[vertexIndices[j] - 1].y);
							figura.push_back(normals_vertex[vertexIndices[j] - 1].z);
						}else{
							figura.push_back(normals_vertex[normalIndices[j] - 1].x);
							figura.push_back(normals_vertex[normalIndices[j] - 1].y);
							figura.push_back(normals_vertex[normalIndices[j] - 1].z);
						}

						figura.push_back(uvs[uvIndices[j] - 1].x);
						figura.push_back(uvs[uvIndices[j] - 1].y);
					}
					cantidad_triangulos++;

				n++;
			}else if(faceSizes[i] >= 4){
					inicial = j;
					ultimo  = inicial + 1;
					j = ultimo + 1;
					for(int cont = 2; cont<faceSizes[i]; cont = cont++){
						figura.push_back(vertices[vertexIndices[inicial] - 1 ].x);
						figura.push_back(vertices[vertexIndices[inicial] - 1 ].y);
						figura.push_back(vertices[vertexIndices[inicial] - 1 ].z);
						if(!has_normal){
							figura.push_back(normals_vertex[vertexIndices[inicial] - 1].x);
							figura.push_back(normals_vertex[vertexIndices[inicial] - 1].y);
							figura.push_back(normals_vertex[vertexIndices[inicial] - 1].z);
						}else{
							figura.push_back(normals_vertex[normalIndices[inicial] - 1].x);
							figura.push_back(normals_vertex[normalIndices[inicial] - 1].y);
							figura.push_back(normals_vertex[normalIndices[inicial] - 1].z);
						}
						figura.push_back(uvs[uvIndices[inicial] - 1].x);
						figura.push_back(uvs[uvIndices[inicial] - 1].y);

						figura.push_back(vertices[vertexIndices[ultimo] - 1 ].x);
						figura.push_back(vertices[vertexIndices[ultimo] - 1 ].y);
						figura.push_back(vertices[vertexIndices[ultimo] - 1 ].z);
						if(!has_normal){
							figura.push_back(normals_vertex[vertexIndices[ultimo] - 1].x);
							figura.push_back(normals_vertex[vertexIndices[ultimo] - 1].y);
							figura.push_back(normals_vertex[vertexIndices[ultimo] - 1].z);
						}else{
							figura.push_back(normals_vertex[normalIndices[ultimo] - 1].x);
							figura.push_back(normals_vertex[normalIndices[ultimo] - 1].y);
							figura.push_back(normals_vertex[normalIndices[ultimo] - 1].z);
						}
						figura.push_back(uvs[uvIndices[ultimo] - 1].x);
						figura.push_back(uvs[uvIndices[ultimo] - 1].y);

						figura.push_back(vertices[vertexIndices[j] - 1 ].x);
						figura.push_back(vertices[vertexIndices[j] - 1 ].y);
						figura.push_back(vertices[vertexIndices[j] - 1 ].z);
						if(!has_normal){
							figura.push_back(normals_vertex[vertexIndices[j] - 1].x);
							figura.push_back(normals_vertex[vertexIndices[j] - 1].y);
							figura.push_back(normals_vertex[vertexIndices[j] - 1].z);
						}else{
							figura.push_back(normals_vertex[normalIndices[j] - 1].x);
							figura.push_back(normals_vertex[normalIndices[j] - 1].y);
							figura.push_back(normals_vertex[normalIndices[j] - 1].z);
						}
						figura.push_back(uvs[uvIndices[j] - 1].x);
						figura.push_back(uvs[uvIndices[j] - 1].y);

						n++;
						ultimo = j;
						j++;
						cantidad_triangulos++;

					}

			}
		}

		//Vertex Buffer Objects VBO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO); 
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, figura.size() *sizeof(float), &figura.front(), GL_STATIC_DRAW);
		glBindVertexArray(VAO);

		 // position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    
		// normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		// textures attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));



	}
	//------------------->Pasamos la figura a la gpu


	calculateBoundingBox();


	//Borramos todo :P
	figura.clear();
	vertices.clear();
	normals.clear();
	normals_vertex.clear();
	uvs.clear();
	faceSizes.clear();
	vertexIndices.clear();


	return true;
}


void Obj::display(){
	display_options();

	

     glBindVertexArray(VAO);
	 glDrawArrays(GL_TRIANGLES, 0, cantidad_triangulos*3);


}



void Obj::calculate_normals(){
	int inicial, ultimo;
	glm::vec3 A,B,C,norm;


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
	
}


void Obj::calculate_normals_vertex(){
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



	vector < float > normal1,normal2;
	glm::vec3 unit_normal;
	cantidad_normal1 = 0; cantidad_normal2 = 0;

	for(int i=0; i<normals.size(); i++){
		normal1.push_back(normals[i].x);
		normal1.push_back(normals[i].y);
		normal1.push_back(normals[i].z);

		unit_normal = glm::normalize(normals[i]);
				
		normal1.push_back(unit_normal.x);
		normal1.push_back(unit_normal.y);
		normal1.push_back(unit_normal.z);
		cantidad_normal1= cantidad_normal1 +2;
	}

	for(int i=0; i<normals_vertex.size(); i++){
		normal2.push_back(normals_vertex[i].x);
		normal2.push_back(normals_vertex[i].y);
		normal2.push_back(normals_vertex[i].z);
				
		unit_normal = glm::normalize(normals_vertex[i]);

		normal2.push_back(unit_normal.x);
		normal2.push_back(unit_normal.y);
		normal2.push_back(unit_normal.z);
		cantidad_normal2 = cantidad_normal2 + 2;
	}

	//Vertex Buffer Objects VBO 1
	glGenVertexArrays(1, &VAO_normal1);
    glGenBuffers(1, &VBO_normal1); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal1);
	glBufferData(GL_ARRAY_BUFFER, normal1.size() *sizeof(float), &normal1.front(), GL_STATIC_DRAW);
    glBindVertexArray(VAO_normal1);

	 // position attribute
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Vertex Buffer Objects VBO 2
	glGenVertexArrays(1, &VAO_normal2);
    glGenBuffers(1, &VBO_normal2); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal2);
	glBufferData(GL_ARRAY_BUFFER, normal2.size() *sizeof(float), &normal2.front(), GL_STATIC_DRAW);
    glBindVertexArray(VAO_normal2);

	 // position attribute
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//------------------->Calculamos las normales




}