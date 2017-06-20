#include "Off.h"

Off::Off()
{
	
}

Off::~Off()
{
}

bool Off::load(string path){
	char line[256];
	int cantidad = 0, aux,cont = 0,cont_line_file = 2;
	int num_Vertices = 0, num_Faces=0  , num_Edges=0;
	int matches = 0;
	glm::vec3 vertex;
	


	ifstream file(path);
	if(!file.is_open()) {
        printf(" Error, no se puede abrir el archivo %s\n",path);
        return false;
    }

	file.getline(line,256);
	if (strcmp(line,"OFF") != 0){
		printf(" Error, el archivo %s no es de tipo OFF\n",path.c_str());
		file.close();
		return false;
	}

	while (!file.eof())	{
		file.getline(line,256);

		if (line[0] == '#'){							//Comment
			printf(" %s\n",line);
			cont--;
		}else if (cont == 0){							//Linea de indices vertices/caras/bordes
				
			matches = sscanf(line, "%d %d %d",&num_Vertices,&num_Faces,&num_Edges);
			if (matches != 3){
				printf("Error en linea %d, leyendo indices: vertices/caras/bordes\n",cont_line_file);
				file.close();
				return false;
			}	

		}else if (cont <= num_Vertices){				//Linea de Vertices
			
			matches = sscanf(line, "%f %f %f",&vertex.x,&vertex.y,&vertex.z);
			if (matches != 3){
				printf("Error en linea %d, leyendo vertices: x y z\n",cont_line_file);
				file.close();
				return false;
			}


			if(vertex.x > maxVertex.x) maxVertex.x = vertex.x;
			if(vertex.y > maxVertex.y) maxVertex.y = vertex.y;
			if(vertex.z > maxVertex.z) maxVertex.z = vertex.z;

			if(vertex.x < minVertex.x) minVertex.x = vertex.x;
			if(vertex.y < minVertex.y) minVertex.y = vertex.y;
			if(vertex.z < minVertex.z) minVertex.z = vertex.z;

			vertices.push_back(vertex);

		}else if (cont <= num_Vertices + num_Faces){	//Linea de caras
			

			char *data = line;
			int offset;
			int cont_inside_face = 0;
			float value;
			bool hasColor = false;

			while (sscanf(data, " %f%n", &value, &offset) == 1)	{
				if(cont_inside_face == 0){				//Cantidad de vertices
					cantidad = value;
					faceSizes.push_back(cantidad);
				}else if(cont_inside_face <= cantidad){	//cada indice de vertice
					vertexIndices.push_back(value);
				}else if(cont_inside_face > cantidad){ //Si tiene colores
					
					if(cont_inside_face == cantidad +1)
						vertex.x = value;

					if(cont_inside_face == cantidad +2)
						vertex.y = value;

					if(cont_inside_face == cantidad +3){
						vertex.z = value;
						hasColor = true;
					}
				}
				data += offset;
				cont_inside_face++;
			}
			if(!hasColor){
				vertex.x = 1.0;
				vertex.y = 1.0;
				vertex.z = 1.0;
			}
			colores.push_back(vertex);
			
		}
		cont++;
		cont_line_file++;


	}
	file.close();

	init_center = (maxVertex + minVertex) / 2.f;
	init_scale = 1.f / glm::max(maxVertex.x - minVertex.x, glm::max(maxVertex.y - minVertex.y, maxVertex.z - minVertex.z));
	
	//Calculamos las normales
	calculate_normals();
	calculate_normals_vertex();


	//Pasamos la figura a la gpu
	vector < float > figura;
	int inicial, ultimo;
	cantidad_triangulos = 0;


	for (int i = 0, j=0, n=0; i < faceSizes.size(); i++){
		if(faceSizes[i] == 3){

				for(int cont = 0; cont<faceSizes[i]; j++,cont++){

					figura.push_back(vertices[vertexIndices[j]].x);
					figura.push_back(vertices[vertexIndices[j]].y);
					figura.push_back(vertices[vertexIndices[j] ].z);

					figura.push_back(normals_vertex[vertexIndices[j]].x);
					figura.push_back(normals_vertex[vertexIndices[j]].y);
					figura.push_back(normals_vertex[vertexIndices[j]].z);

				}
				cantidad_triangulos++;

			n++;
		}else if(faceSizes[i] >= 4){
				inicial = j;
				ultimo  = inicial + 1;
				j = ultimo + 1;
				for(int cont = 2; cont<faceSizes[i]; cont = cont++){
					figura.push_back(vertices[vertexIndices[inicial]].x);
					figura.push_back(vertices[vertexIndices[inicial]].y);
					figura.push_back(vertices[vertexIndices[inicial]].z);
					figura.push_back(normals_vertex[vertexIndices[inicial]].x);
					figura.push_back(normals_vertex[vertexIndices[inicial]].y);
					figura.push_back(normals_vertex[vertexIndices[inicial]].z);

					figura.push_back(vertices[vertexIndices[ultimo]].x);
					figura.push_back(vertices[vertexIndices[ultimo]].y);
					figura.push_back(vertices[vertexIndices[ultimo]].z);
					figura.push_back(normals_vertex[vertexIndices[ultimo]].x);
					figura.push_back(normals_vertex[vertexIndices[ultimo]].y);
					figura.push_back(normals_vertex[vertexIndices[ultimo]].z);

					figura.push_back(vertices[vertexIndices[j]].x);
					figura.push_back(vertices[vertexIndices[j]].y);
					figura.push_back(vertices[vertexIndices[j]].z);
					figura.push_back(normals_vertex[vertexIndices[j]].x);
					figura.push_back(normals_vertex[vertexIndices[j]].y);
					figura.push_back(normals_vertex[vertexIndices[j]].z);

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



void Off::display(){
	display_options();

	 // render the figure
     glBindVertexArray(VAO);
	 glDrawArrays(GL_TRIANGLES, 0, cantidad_triangulos*3);
}

void Off::calculate_normals(){
	int inicial, ultimo;
	glm::vec3 A,B,C,norm;

	normals.clear();
	
	for (int i = 0, j=0; i < faceSizes.size(); i++){

		if(faceSizes[i] == 3){
			A = vertices[vertexIndices[j]];
			B = vertices[vertexIndices[j+1]];
			C = vertices[vertexIndices[j+2]];

			norm = glm::cross(B-A,C-A);

			normals.push_back(norm);

			j = j+3;

		}else if(faceSizes[i] >= 4){

				inicial = j;
				ultimo  = inicial + 1;
				j = ultimo + 1;
				for(int cont = 2; cont<faceSizes[i]; cont++){
					A = vertices[vertexIndices[inicial]];
					B = vertices[vertexIndices[ultimo]];
					C = vertices[vertexIndices[j]];

					norm = glm::cross(B-A,C-A);

					normals.push_back(norm);

					ultimo = j;
					j++;
				}
		}
	}
}

void Off::calculate_normals_vertex(){
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
					
				if( (vertexIndices[j]) == i ){ //El vertice esta contenido en esta cara!
					normals_aux.push_back(normals[n]);
				}else if( (vertexIndices[j+1]) == i ){ //El vertice esta contenido en esta cara!
					normals_aux.push_back(normals[n]);
				}else if( (vertexIndices[j+2]) == i ){ //El vertice esta contenido en esta cara!
					normals_aux.push_back(normals[n]);
				}
				j = j+3;
				n++;
			}else if(faceSizes[k] >= 4){
					inicial = j;
					ultimo  = inicial + 1;
					j = ultimo + 1;

					
					for(int cont = 2; cont<faceSizes[k]; cont = cont++){
						if( (vertexIndices[inicial]) == i ){ //El vertice esta contenido en esta cara!
							normals_aux.push_back(normals[n]);
						}else if( (vertexIndices[ultimo]) == i ){ //El vertice esta contenido en esta cara!
							normals_aux.push_back(normals[n]);
						}else if( (vertexIndices[j]) == i ){ //El vertice esta contenido en esta cara!
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
		cantidad_normal1 = cantidad_normal1 + 2;
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


}
