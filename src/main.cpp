#include "main.h"

int main(void){
	//picked = 0;

	//Inicialización de bibliotecas para mostrar todo
	if (!initGlfw() || !initUserInterface() || !initGlew() || !initFreeType()){
		printf("Error al inicializar bibliotecas\n");
		MessageBox(NULL,(LPCWSTR)L"Error al inicializar bibliotecas", (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}
	

	//Inicialización de la escena que se va mostrar en el juego
	if(!initScene()){
		printf("Error al inicializar escena \n");
		MessageBox(NULL,(LPCWSTR)L"Error al inicializar escena", (LPCWSTR)L"Error", MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}


	glEnable(GL_DEPTH_TEST);
	reshape(gWindow, SCREEN_WIDTH, SCREEN_HEIGHT);

	while (!glfwWindowShouldClose(gWindow))	{
		display();
		TwDraw();
		//updateUserInterface();
		glfwSwapBuffers(gWindow);
		glfwPollEvents();
	}

	destroy();
	return EXIT_SUCCESS;
}

void borrar_objetos(){

	for (auto it = models.begin(); it != models.end(); ++it){
		delete *it;
	}
	models.clear();
}
void updateUserInterface(){


	if (picked > -1){ //Aplicamos las modificaciones de AntteawBar al ultimo objeto
		models[picked]->setTranslation	(userInterface->getModelTranslation());
		models[picked]->setScale		(userInterface->getModelScale());
		models[picked]->setRotation		(userInterface->getModelRotation());

		models[picked]->setVisualizacion_Alambrica_Front(userInterface->getVisualizacion_Alambrica_Front());
		models[picked]->setVisualizacion_Alambrica_Back(userInterface->getVisualizacion_Alambrica_Back());
		models[picked]->setVisualizacion_Alambrica_Points_Front(userInterface->getVisualizacion_Alambrica_Points_Front());
		models[picked]->setVisualizacion_Alambrica_Points_Back(userInterface->getVisualizacion_Alambrica_Points_Back());
		models[picked]->setVisualizacion_Normales_Vertex(userInterface->getVisualizacion_Normales_Vertex());
		models[picked]->setVisualizacion_Normales_Face(userInterface->getVisualizacion_Normales_Face());
		models[picked]->setVisualizacion_CullFace_Front(userInterface->getVisualizacion_CullFace_Front());
		models[picked]->setVisualizacion_CullFace_Back(userInterface->getVisualizacion_CullFace_Back());
		models[picked]->setVisualizacion_Zbuffer(userInterface->getVisualizacion_Zbuffer());
		models[picked]->setVisualizacion_BoundingBox(userInterface->getVisualizacion_BoundingBox());

		models[picked]->setColor_Lines		(userInterface->getColor_Lines());
		models[picked]->setColor_Fill		(userInterface->getColor_Fill());
		models[picked]->setColor_Box		(userInterface->getColor_Box());
		models[picked]->setColor_Normals	(userInterface->getColor_Normals());

		models[picked]->setMaterial_Ambiental(userInterface->getMaterial_Ambiental());
		models[picked]->setMaterial_Difuso(userInterface->getMaterial_Difuso());
		models[picked]->setMaterial_Especular(userInterface->getMaterial_Especular());
		models[picked]->setMaterial_Shininess(userInterface->getMaterial_Shininess());

		models[picked]->setAuto_Rotate(userInterface->getAuto_Rotate());
		models[picked]->setRotate_Start(userInterface->getRotate_Start());
		models[picked]->setTimeMs(userInterface->GetTimeMs());
		models[picked]->setRotate_Time(userInterface->getRotate_Time());

		models[picked]->setActivateTexture(userInterface->getActivateTexture());
		userInterface->setHas_uvs(models[picked]->getHas_uvs());
	}
}
void SetQuaternionFromAxisAngle(const float *axis, float angle, float *quat)
{// Routine to set a quaternion from a rotation axis and angle
// ( input axis = float[3] angle = float  output: quat = float[4] )
    float sina2, norm;
    sina2 = (float)sin(0.5f * angle);
    norm = (float)sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);
    quat[0] = sina2 * axis[0] / norm;
    quat[1] = sina2 * axis[1] / norm;
    quat[2] = sina2 * axis[2] / norm;
    quat[3] = (float)cos(0.5f * angle);
}
void ConvertQuaternionToMatrix(const float *quat, float *mat)
{// Routine to convert a quaternion to a 4x4 matrix
// ( input: quat = float[4]  output: mat = float[4*4] )
    float yy2 = 2.0f * quat[1] * quat[1];
    float xy2 = 2.0f * quat[0] * quat[1];
    float xz2 = 2.0f * quat[0] * quat[2];
    float yz2 = 2.0f * quat[1] * quat[2];
    float zz2 = 2.0f * quat[2] * quat[2];
    float wz2 = 2.0f * quat[3] * quat[2];
    float wy2 = 2.0f * quat[3] * quat[1];
    float wx2 = 2.0f * quat[3] * quat[0];
    float xx2 = 2.0f * quat[0] * quat[0];
    mat[0*4+0] = - yy2 - zz2 + 1.0f;
    mat[0*4+1] = xy2 + wz2;
    mat[0*4+2] = xz2 - wy2;
    mat[0*4+3] = 0;
    mat[1*4+0] = xy2 - wz2;
    mat[1*4+1] = - xx2 - zz2 + 1.0f;
    mat[1*4+2] = yz2 + wx2;
    mat[1*4+3] = 0;
    mat[2*4+0] = xz2 + wy2;
    mat[2*4+1] = yz2 - wx2;
    mat[2*4+2] = - xx2 - yy2 + 1.0f;
    mat[2*4+3] = 0;
    mat[3*4+0] = mat[3*4+1] = mat[3*4+2] = 0;
    mat[3*4+3] = 1;
}
void MultiplyQuaternions(const float *q1, const float *q2, float *qout)
{// Routine to multiply 2 quaternions (ie, compose rotations)
// ( input q1 = float[4] q2 = float[4]  output: qout = float[4] )
    float qr[4];
	qr[0] = q1[3]*q2[0] + q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1];
	qr[1] = q1[3]*q2[1] + q1[1]*q2[3] + q1[2]*q2[0] - q1[0]*q2[2];
	qr[2] = q1[3]*q2[2] + q1[2]*q2[3] + q1[0]*q2[1] - q1[1]*q2[0];
	qr[3]  = q1[3]*q2[3] - (q1[0]*q2[0] + q1[1]*q2[1] + q1[2]*q2[2]);
    qout[0] = qr[0]; qout[1] = qr[1]; qout[2] = qr[2]; qout[3] = qr[3];
}
void reshape(GLFWwindow *window, int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	userInterface->reshape();

	if(userInterface->getViewOption() == PERSPECTIVA){
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f);
	}else{
		glm::mat4 aux		= glm::mat4(1.0f);
		projection = aux;
		glOrtho(0.0f, (float)SCREEN_WIDTH / 100, 0.0f, (float)SCREEN_HEIGHT / 100, 0.0f, 1000.0f);
	}

}
bool checkCollision(CModel *A, CModel *B){
	bool collision = false;

	//AABB (Axis Aligned Bounding Box)
	//Basically you just check their relative positions and see if all three axes overlap
	if(abs(A->getTranslation()[0] - B->getTranslation()[0]) <  ((A->maxVertex.x - A->minVertex.x)/2 + (B->maxVertex.x - B->minVertex.x)/2) ){
		//if(abs(A->getTranslation()[1] - B->getTranslation()[1]) <  ((A->maxVertex.y - A->minVertex.y)/2 + (B->maxVertex.y - B->minVertex.y)/2) ){
			if(abs(A->getTranslation()[2] - B->getTranslation()[2]) <  ((A->maxVertex.z - A->minVertex.z)/2 + (B->maxVertex.z - B->minVertex.z)/2) ){
				collision = true;
			}
		//}
	}

	return collision;

}
void display()
{
	float mat[4*4]; // rotation matrix
	float *translation;
	float *scale;
	float *rotation;

	//glClearColor(0.7f, 0.7f, 0.7f, 1.0); //Color de fondo gris
	
	glClearColor(FogParameters::vFogColor.x,FogParameters::vFogColor.y,FogParameters::vFogColor.z,FogParameters::vFogColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

	glm::vec3 translation2, scale2;
	glm::mat4 model		= glm::mat4(1.0f);	
	glm::mat4 model2	= glm::mat4(1.0f);
	glm::mat4 rotation2 = glm::make_mat4(mat);
	glm::mat4 view2		= glm::mat4(1.0f);
	
	

	if(userInterface->getViewOption() == PERSPECTIVA){

		view = camera.GetViewMatrix();
	}else{
		view = view2;
	}
	
	

	//------------------- > > Graficamos la light_sphere < < -----------------------
	translation = userInterface->getLight_Posicion();
	light_sphere->setTranslation(translation);

	//------------------- > > Graficamos la light_sphere < < -----------------------

		
	//----------------------> >  Graficamos las figuras en la escena < < -------------------------
	for (unsigned int i = 0; i < models.size(); i++)
	{	
		//translation	= models[i]->getTranslation();
		//models[i]->init_center
		translation	= models[i]->getTranslation();
		scale		= models[i]->getScale();
		rotation	= models[i]->getRotation();
		

		if( models[i]->getAuto_Rotate() ) 	{
			float axis[3] = { 0, 1, 0 };
			float angle = (float)(models[i]->getTimeMs() - models[i]->getRotate_Time())/1000.0f;
			float quat[4];
			SetQuaternionFromAxisAngle(axis, angle, quat);
			MultiplyQuaternions(models[i]->getRotate_Start(), quat, rotation);
		}
		ConvertQuaternionToMatrix(rotation, mat);


		model	= glm::mat4(1.0f);	
		model2	= glm::mat4(1.0f);
		rotation2 = glm::make_mat4(mat);
			
		translation2[0] = translation[0];
		translation2[1] = translation[1];
		translation2[2] = translation[2];
		scale2[0]		= scale[0];
		scale2[1]		= scale[1];
		scale2[2]		= scale[2];

		model	= glm::translate(model, glm::vec3(0,0,-1));
		model	= glm::scale(model,scale2);
		model2	= glm::translate(model2, translation2);
		model =  model * model2 * rotation2 ; //Primero se escala, luego se traslada y por ultimo se rota

		glm::mat4 mvp;

		mvp = projection * view * model;

		if((i != picked && !(i == pista_llegada && pista_llegada_activa == 1)) || (pantalla_menu == true && !(i == pista_llegada && pista_llegada_activa == 1))){

		if(!userInterface->getLight_Active()){ //Sin luz
			glslProgramLight->enable();
				glslProgramLight->loadUniformMatrix("MVP",				GShader::UNIFORM_SIZE_4D, glm::value_ptr(mvp));
				if(models[i]->getVisualizacion_Alambrica_Back() && models[picked]->getVisualizacion_Alambrica_Front())
					glslProgramLight->loadUniformf("colorFill",			GShader::UNIFORM_SIZE_3D, models[i]->getColor_Lines()); //Color de lineas
				else
					glslProgramLight->loadUniformf("colorFill",			GShader::UNIFORM_SIZE_3D, models[i]->getColor_Fill()); //Color de relleno
				
				models[i]->display();
			glslProgramLight->disable();

		}else{ //Con luz
			CGLSLProgram *glslAux;

			//if(userInterface->getFlat_Shading())
				//glslAux = glslProgram2;
			//else if(userInterface->getPhong_Shading())
				glslAux = glslProgram;
			//else //Defecto es  gouround shading
				//glslAux = glslProgram3;

			glslAux->enable();
				glslAux->loadUniformMatrix("projection",			GShader::UNIFORM_SIZE_4D, glm::value_ptr(projection));
				glslAux->loadUniformMatrix("view",					GShader::UNIFORM_SIZE_4D, glm::value_ptr(view));
				glslAux->loadUniformMatrix("model",					GShader::UNIFORM_SIZE_4D, glm::value_ptr(model));

				glslAux->loadUniformf("light.lightAmbient",			GShader::UNIFORM_SIZE_3D, userInterface->getLight_Ambiental());
				glslAux->loadUniformf("light.lightDiffuse",			GShader::UNIFORM_SIZE_3D, userInterface->getLight_Difuso());
				glslAux->loadUniformf("light.lightSpecular",		GShader::UNIFORM_SIZE_3D, userInterface->getLight_Especular());
				glslAux->loadUniformf("light.lightPosition",		GShader::UNIFORM_SIZE_3D, light_sphere->getTranslation());
			

				glslAux->loadUniformf("light.viewPosition",			GShader::UNIFORM_SIZE_3D, glm::value_ptr(view));
			
				glslAux->loadUniformf("material.materialAmbient",	GShader::UNIFORM_SIZE_3D, models[i]->getMaterial_Ambiental());
				glslAux->loadUniformf("material.materialDiffuse",	GShader::UNIFORM_SIZE_3D, models[i]->getMaterial_Difuso());
				glslAux->loadUniformf("material.materialSpecular",	GShader::UNIFORM_SIZE_3D, models[i]->getMaterial_Especular());
				GLfloat aux;
				aux = models[i]->getMaterial_Shininess();
				glslAux->loadUniformf("material.materialShininess",	GShader::UNIFORM_SIZE_1D,  &aux);


				//Luz de Llegada
				float aux_vec [3] = {0.8,0.1,0.1};
				glslAux->loadUniformf("light.finish_position",				GShader::UNIFORM_SIZE_3D, models[finish]->getTranslation());
				glslAux->loadUniformf("light.finish_lightDiffuse",			GShader::UNIFORM_SIZE_3D, aux_vec);
				aux_vec [0] = 0.8;
				aux_vec [1] = 0.1;
				aux_vec [2] = 0.1;
				glslAux->loadUniformf("light.finish_lightSpecular",		GShader::UNIFORM_SIZE_3D, userInterface->getLight_Especular());

				//FlashLight
				glslAux->loadUniformi("propiedades.flashlight",			GShader::UNIFORM_SIZE_1D,  &flashlight);
				glslAux->loadUniformi("propiedades.flashlight_fog",		GShader::UNIFORM_SIZE_1D,  &flashlight_fog);
				if(flashlight == 1){
					aux_vec [0] = camera.Position.x;
					aux_vec [1] = camera.Position.y;
					aux_vec [2] = camera.Position.z;
					glslAux->loadUniformf("light.position",				GShader::UNIFORM_SIZE_3D, aux_vec);
					aux_vec [0] = camera.Front.x;
					aux_vec [1] = camera.Front.y;
					aux_vec [2] = camera.Front.z;
					glslAux->loadUniformf("light.direction",			GShader::UNIFORM_SIZE_3D, aux_vec);
					aux = glm::cos(glm::radians(22.5f));
					glslAux->loadUniformf("light.cutOff",				GShader::UNIFORM_SIZE_1D,  &aux);
				}
				aux = 1.0f;
				glslAux->loadUniformf("light.constant",				GShader::UNIFORM_SIZE_1D,  &aux);
				aux = 0.09f;
				aux = 0.045f;
				glslAux->loadUniformf("light.linear",				GShader::UNIFORM_SIZE_1D,  &aux);
				aux = 0.032f;
				aux = 0.0075f;
				glslAux->loadUniformf("light.quadratic",			GShader::UNIFORM_SIZE_1D,  &aux);
				

				int has_texture[1] ;
				has_texture[0] = 0;

				if(models[i]->getHas_uvs() && models[i]->getActivateTexture()){
					has_texture[0] = 1;

					glslAux->loadUniformSampler("texture1",0);
					// bind Texture
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, models[i]->getTexture());
					
				}
				glslAux->loadUniformi("has_texture",	GShader::UNIFORM_SIZE_1D,  has_texture);


				glslAux->loadUniformi("fog_active",		GShader::UNIFORM_SIZE_1D,  fog_active);
				if(fog_active[0] == 1){
					glslAux->loadUniformi("fogParams.iEquation",	GShader::UNIFORM_SIZE_1D, &FogParameters::iFogEquation);
					float vFogColor [] = {FogParameters::vFogColor.x,FogParameters::vFogColor.y,FogParameters::vFogColor.z,FogParameters::vFogColor.w};
					glslAux->loadUniformf("fogParams.vFogColor",	GShader::UNIFORM_SIZE_4D, vFogColor);
					if(FogParameters::iFogEquation == FOG_EQUATION_LINEAR){
						glslAux->loadUniformf("fogParams.fStart",	GShader::UNIFORM_SIZE_1D, &FogParameters::fStart);
						glslAux->loadUniformf("fogParams.fEnd",		GShader::UNIFORM_SIZE_1D, &FogParameters::fEnd);
					}else
						glslAux->loadUniformf("fogParams.fDensity", GShader::UNIFORM_SIZE_1D, &FogParameters::fDensity);
				}

				models[i]->display();
			glslAux->disable();




			if(models[i]->getVisualizacion_BoundingBox()){ //Bounding Box

				glslNormal->enable();
					glslNormal->loadUniformMatrix("projection",			GShader::UNIFORM_SIZE_4D, glm::value_ptr(projection));
					glslNormal->loadUniformMatrix("view",				GShader::UNIFORM_SIZE_4D, glm::value_ptr(view));
					glslNormal->loadUniformMatrix("model",				GShader::UNIFORM_SIZE_4D, glm::value_ptr(model));
					glslNormal->loadUniformf("colorFill",				GShader::UNIFORM_SIZE_3D, models[i]->getColor_Box());

					models[i]->show_bounding_box();
				glslProgram->disable();

			}

		}

		}



	}
	
	//----------------------> >  Graficamos las figuras en la escena < < -------------------------
	




	
	//----------------------> > Graficamos el Sky Box < < ----------------------
	skyBox1->display(view, projection);
	//----------------------> > Graficamos el Sky Box < < ----------------------


	pantalla_juego();

}

void keyControl(int key,int index[2],int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
	if (key == GLFW_KEY_W){
		//Arriba
		index[0] = x1; index[1] = y1;
	}else if (key ==  GLFW_KEY_S){
		//Abajo
		index[0] = x2; index[1] = y2;
		
	}else if (key == GLFW_KEY_A){
		//Izquierda
		index[0] = x3; index[1] = y3;

	}else if (key == GLFW_KEY_D){
		//Derecha
		index[0] = x4; index[1] = y4;
	}
}
void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	
	//float MovementSpeed = 80.5f;
	//float velocity		= MovementSpeed * deltaTime;
	double velocity = 0.125f;
	float mModelTranslation[3]	= {models[picked]->getTranslation()[0], models[picked]->getTranslation()[1], models[picked]->getTranslation()[2]}; 
	int index[2] = { -1, 0};

	if (TwEventKeyGLFW(key, action))
		return;

	if (action == GLFW_PRESS)
	{
		
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
			break;
			
			case GLFW_KEY_LEFT_CONTROL:
				flying_camera = !flying_camera;
			break;
			/*case GLFW_KEY_LEFT_ALT:
				camera_static = !camera_static;
			break;*/
			/*case GLFW_KEY_LEFT_SHIFT:
				camera_first_person = !camera_first_person;
			break;*/
			case GLFW_KEY_1:
				flashlight = !flashlight;
			break;
			case GLFW_KEY_2:
				flashlight_fog = !flashlight_fog;
			break;
			case GLFW_KEY_3:
				sound(sounds_bonus[0],false);
				activar_pista();
			break;
			case GLFW_KEY_4:
				if(!jugadores.empty() && jugador_actual != -1){
					jugadores[jugador_actual]->cambiar_titulo();
				}
			break;
			case GLFW_KEY_SPACE:
				sound(sounds_fail[1],false);
				//sound_stop(true,"");
				reiniciar();
				models[picked]->setActivateTexture(false);
			break;
			case GLFW_KEY_TAB:
				ayuda = !ayuda;
			break;
			case GLFW_KEY_ENTER:
				if(pantalla_menu == true && !jugadores.empty() && pantallas_activas[0]){
					//sound_stop(true,"");
					level_actual = -1;
					flying_camera = true;
					camera_first_person = true;
					//sound_stop(false,"..\\sounds\\start_1 ILLUMINATI CONFIRMED.mp3");
					
					pantalla_menu = false;
					pantallas_activas[0] = false;

					aumentar_nivel(true);
				}else if(pantalla_menu == true && !jugadores.empty() && pantallas_activas[1]){
					//sound_stop(true,"");
					//level_actual = -1;
					flying_camera = true;
					camera_first_person = true;
					//sound_stop(false,"..\\sounds\\start_1 ILLUMINATI CONFIRMED.mp3");
					//aumentar_nivel(true);
					pantalla_menu = false;
					pantallas_activas[1] = false;
					aumentar_nivel(true);
				}
			break;
			case GLFW_KEY_F5:
				cambiar_jugador();
				models[picked]->setActivateTexture(false);
			break;
			case GLFW_KEY_F12:
				//pantalla_resultados();
				pantalla_menu = false;
				aumentar_nivel(true);
			break;
			case GLFW_KEY_F1:
				pantalla_inicial();
			break;
			case GLFW_KEY_KP_ADD:
				FogParameters::fDensity = FogParameters::fDensity + 0.01f;
				printf("FOG: density = %f\n",FogParameters::fDensity);
			break;
			case GLFW_KEY_KP_SUBTRACT:
				FogParameters::fDensity = FogParameters::fDensity - 0.01f;
				printf("FOG: density = %f\n",FogParameters::fDensity);
			break;




		}
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		
			//Camara
			if(key == GLFW_KEY_UP){
				camera.ProcessKeyboard(FORWARD, deltaTime);
			}else if(key == GLFW_KEY_DOWN){
				camera.ProcessKeyboard(BACKWARD, deltaTime);
			}else if(key == GLFW_KEY_LEFT){
				camera.ProcessKeyboard(LEFT, deltaTime);
			}else if(key == GLFW_KEY_RIGHT){
				camera.ProcessKeyboard(RIGHT, deltaTime);
			}
			

			if(picked>0){

				if(camera.Yaw >= 230 && camera.Yaw <=319)
					keyControl(key, index, 2, 1, 2, 0, 0, 1, 0, 0);//normal
				else if(camera.Yaw >= 141 && camera.Yaw <=230)
					keyControl(key, index, 0, 1, 0, 0, 2, 0, 2, 1);
				else if(camera.Yaw >= 38 && camera.Yaw <=141)
					keyControl(key, index, 2, 0, 2, 1, 0, 0, 0, 1);
				else if((camera.Yaw >= 0 && camera.Yaw <=38) || (camera.Yaw >= 319 && camera.Yaw <=360))
					keyControl(key, index, 0, 0, 0, 1, 2, 1, 2, 0);
				
			}
			
			bool collision = false;
			bool nivel	   = false;
			//Comprobamos si hay collision

			if(picked>0  && index[0] != -1)	{

				if(index[1] == 0)
					mModelTranslation[index[0]] += velocity;
				else
					mModelTranslation[index[0]] -= velocity;

				models[picked] -> setTranslation(mModelTranslation);

				for(int j=0; (j <models.size()) ; j++){
					if( picked!=j && j!= ground && j != pista_llegada && checkCollision(models[picked], models[j])){
						collision = true;
						if(j==finish && !pantalla_menu){

							nivel = true;
							//pantalla_resultados();
							aumentar_nivel(true);
							
						}

					}
			
				}
				if(collision && !nivel){
					if(index[1] == 0)
						mModelTranslation[index[0]] -= velocity;
					else
						mModelTranslation[index[0]] += velocity;

					models[picked] -> setTranslation(mModelTranslation);
					userInterface -> setModelTranslation(mModelTranslation);

					
				
				}else if(!nivel){

					if(!camera_static && !camera_first_person){ //Camara en 3ra persona
						camera.Position.x = mModelTranslation[0];
						//camera.Position.y = mModelTranslation[1];
						camera.Position.z = mModelTranslation[2] + 10;
					}
					else if(!camera_static && camera_first_person){ // Camara en 1era persona
						camera.Position.x = mModelTranslation[0];
						camera.Position.y = mModelTranslation[1];
						camera.Position.z = mModelTranslation[2] - 1.5f;
					}
					userInterface -> setModelTranslation(mModelTranslation);
				}

			}

	}
}
bool type_object(string name){
	int value	= -1;
	int length	= name.length();
	//CSOff* soff = new CSOff();
	Off* off_object = new Off();
	Obj* obj_object = new Obj();

	
	if (length < 5)
		return false;
	
	if(name[length-4]=='.' && name[length-3]=='o' && name[length-2]=='f' && name[length-1]=='f')
		value = 1; // value off
	else if(name[length-4]=='.' && name[length-3]=='o' && name[length-2]=='b' && name[length-1]=='j')
		value = 2; // value obj
	else if(name[length-5]=='.' && name[length-4]=='s' && name[length-3]=='o' && name[length-2]=='f' && name[length-1]=='f')
		value = 0; // value soff
	switch (value)
	{
		case 0:
			
			/*if(!soff->load(name))
				return false;
			models.push_back(soff);
			picked++;*/
		break;
		case 1:
			
			if(!off_object->load(name))
				return false;
			models.push_back(off_object);
			picked++;
		break;
		case 2:
			
			if(!obj_object->load(name))
				return false;
			models.push_back(obj_object);
			picked++;
		break;

		default:
			printf("Error, no se reconoce el tipo de archivo (soff, off, obj) \n");
			return false;
		break;
	}

	//Centramos el objeto
	//float mModelTranslation2[3]	= {-models[picked]->init_center.x, -models[picked]->init_center.y, -models[picked]->init_center.z}; 
	float mModelTranslation2[3]	= {0,0,0}; 
	float mModelScale2[4]		= {models[picked]->init_scale, models[picked]->init_scale, models[picked]->init_scale, models[picked]->init_scale};
	float mModelRotation2[4]	= {0, 0, 0, 1};

	userInterface -> setModelTranslation(mModelTranslation2);
	userInterface -> setModelScale		(mModelScale2);
	userInterface -> setModelRotation	(mModelRotation2);
	models[picked]-> setTranslation		(userInterface->getModelTranslation());
	models[picked]-> setScale			(userInterface->getModelScale());
	models[picked]-> setRotation		(userInterface->getModelRotation());

	if(picked>0){ //Default Options
		userInterface -> setActivateTexture(false);
		userInterface -> setAuto_Rotate(false);
	}

	//printf("- Archivo %s cargado correctamente\n",name.c_str());
	return true;
	
}
bool agregar_objeto(string file_name, int index_texture, float tx, float ty, float tz, float sx, float sy, float sz, float rx, float ry, float rz, float rw){


	if (type_object(file_name) == false){
		printf("Error, al cargar objeto %s\n",file_name.c_str());
		return false;
	}


	//Centramos el objeto
	float mModelTranslation[3]	= {models[picked]->getTranslation()[0] + tx,models[picked]->getTranslation()[1] +  ty,models[picked]->getTranslation()[2] +  tz}; 
	//float mModelTranslation[3]	= {models[picked]->init_center.x + tx,models[picked]->init_center.y +  ty,models[picked]->init_center.z +  tz}; 
	
	float mModelScale[3]		= {sx,sy,sz}; 
	float mModelRotation[4]		= {rx,ry,rz,rw};

	/*float mModelTranslation[3]	= {models[picked]->getTranslation()[0] + tx,models[picked]->getTranslation()[1] +  ty,models[picked]->getTranslation()[2] +  tz}; 
	float mModelScale[3]		= {models[picked]->getScale()[0] + sx,models[picked]->getScale()[1] +  sy,models[picked]->getScale()[2] +  sz}; 
	float mModelRotation[4]		= {models[picked]->getRotation()[0] + rx,models[picked]->getRotation()[1] +  ry,models[picked]->getRotation()[2] +  rz,models[picked]->getRotation()[3] +  rw};*/

	/*userInterface -> setModelTranslation(mModelTranslation);
	userInterface -> setModelScale(mModelScale);
	userInterface -> setModelRotation(mModelRotation);*/
	models[picked]->setTranslation	(mModelTranslation);
	models[picked]->setScale		(mModelScale);
	models[picked]->setRotation		(mModelRotation);

	userInterface -> setActivateTexture(true);
	userInterface -> setAuto_Rotate(false);

	float ambient	[] = { 0.24725, 0.24725, 0.0745};
	float difuso	[] = { 0.75164, 0.60648, 0.22648};
	float especular	[] = { 0.628281, 0.555802, 0.366065};
	float shininess = 0.4;


	models[picked]->setMaterial_Ambiental					(ambient);
	models[picked]->setMaterial_Difuso						(difuso);
	models[picked]->setMaterial_Especular					(especular);
	models[picked]->setMaterial_Shininess					(shininess);

	
	models[picked]->setAuto_Rotate							(userInterface->getAuto_Rotate());
	models[picked]->setRotate_Start							(userInterface->getRotate_Start());
	models[picked]->setTimeMs								(userInterface->GetTimeMs());
	models[picked]->setRotate_Time							(userInterface->getRotate_Time());

	models[picked]->setActivateTexture						(userInterface->getActivateTexture());
	userInterface->setHas_uvs								(models[picked]->getHas_uvs());
	models[picked]->setTexture								(index_texture); 
	

	return true;
}
bool agregar_objeto2(vector	<CModel *>	&models,string file_name, int index_texture, float tx, float ty, float tz, float sx, float sy, float sz, float rx, float ry, float rz, float rw, float mat_ambient1, float mat_ambient2, float mat_ambient3, float mat_difuso1, float mat_difuso2, float mat_difuso3, float mat_especular1, float mat_especular2, float mat_especular3, float mat_shininess){
	if (type_object(file_name) == false){
		printf("Error, al cargar objeto %s\n",file_name.c_str());
		return true;
	}
	//Centramos el objeto
	float mModelTranslation[3]	= {models[picked]->getTranslation()[0] + tx,models[picked]->getTranslation()[1] +  ty,models[picked]->getTranslation()[2] +  tz}; 
	float mModelScale[3]		= {sx,sy,sz}; 
	float mModelRotation[4]		= {rx,ry,rz,rw};

	models[picked]->setTranslation	(mModelTranslation);
	models[picked]->setScale		(mModelScale);
	models[picked]->setRotation		(mModelRotation);

	

	float ambient	[] = { mat_ambient1, mat_ambient2, mat_ambient3};
	float difuso	[] = { mat_difuso1, mat_difuso2, mat_difuso3};
	float especular	[] = { mat_especular1, mat_especular2, mat_especular3};
	float shininess = mat_shininess;


	models[picked]->setMaterial_Ambiental					(ambient);
	models[picked]->setMaterial_Difuso						(difuso);
	models[picked]->setMaterial_Especular					(especular);
	models[picked]->setMaterial_Shininess					(shininess);

	
	//userInterface -> setActivateTexture(true);
	//userInterface -> setAuto_Rotate(false);
	models[picked]->setAuto_Rotate							(false);
	//models[picked]->setRotate_Start							(userInterface->getRotate_Start());
	//models[picked]->setTimeMs								(userInterface->GetTimeMs());
	//models[picked]->setRotate_Time							(userInterface->getRotate_Time());

	models[picked]->setActivateTexture						(true);
	//userInterface->setHas_uvs								(models[picked]->getHas_uvs());
	if(index_texture < 0)
		models[picked]->setActivateTexture						(false);
	else
		models[picked]->setTexture								(texture[index_texture]); 
	

	return false;
}

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	

	if (TwEventMouseButtonGLFW(button, action)){

		if (userInterface->getViewOption() != view_option && picked != -1){ //Cambio de vision, perspectiva o ortogonal
			reshape(gWindow, SCREEN_WIDTH, SCREEN_HEIGHT); 
			view_option = userInterface->getViewOption();

			//Centramos el objeto
			
			float mModelTranslation2[3]	= {-models[picked]->init_center.x, -models[picked]->init_center.y, -models[picked]->init_center.z}; 
			float mModelScale2[4]		= {models[picked]->init_scale, models[picked]->init_scale, models[picked]->init_scale, models[picked]->init_scale};
			float mModelRotation2[4]	= {0, 0, 0, 1};

			userInterface -> setModelTranslation(mModelTranslation2);
			userInterface -> setModelScale(mModelScale2);
			userInterface -> setModelRotation(mModelRotation2);
		}

		if (userInterface->getFileName() != file_name){ // Para cargar nuevos archivos
			//reshape(gWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
			file_name = userInterface -> getFileName();

			//Cargamos otro objeto
			if (type_object(file_name) == false){
				printf("Error, al cargar objeto %s\n",file_name.c_str());
			}

		}

		if (userInterface->getFileNameTexture() != file_name_texture){ // Para cargar nuevos texturas
			reshape(gWindow, SCREEN_WIDTH, SCREEN_HEIGHT);
			
			file_name_texture = userInterface -> getFileNameTexture();

			printf("\nfilename texture: %s\n",file_name_texture.c_str());
		

			//Probando textura


			 // load and create a texture 
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
			models[picked]->setTexture(texture);
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// load image, create texture and generate mipmaps
			int width, height, nrChannels;
			
			//string file_name = "bricks.jpg";

			unsigned char *data2 = stbi_load(file_name_texture.c_str(), &width, &height, &nrChannels, 0);
			stbi_failure_reason();
			if (data2 != NULL)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				//std::cout << "Failed to load texture" << std::endl;
				printf("Failed to load texture \n");
			}
			stbi_image_free(data2);





		}

		return;
	}
}
void cursorPos(GLFWwindow* window, double x, double y)
{

		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}
	
		float xoffset = x - lastX;
		float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top
	

		lastX = x;
		lastY = y;
	if(flying_camera == true &&  !(x == centerX && y == centerY) ){
		camera.ProcessMouseMovement(xoffset, yoffset);

		glfwSetCursorPos (window, centerX, centerY);
	}
	

	if (TwEventMousePosGLFW(int(x), int(y)))
		return;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
void charInput(GLFWwindow* window, unsigned int scanChar)
{
	if (TwEventCharGLFW(scanChar, GLFW_PRESS))
		return;
}
void destroy()
{
	for (unsigned int i = 0; i < models.size(); i++)
		delete models[i];

	//delete userInterface;

	glfwDestroyWindow(gWindow);
	TwTerminate();
	glfwTerminate();
}
bool loadTextures(){

	string file_name_texture[] = {	"../models/textures/wall.jpg",		"../models/textures/bricks.jpg",		"../models/textures/golddiag.jpg", 
									"../models/textures/grass.png",		"../models/textures/metalplate.jpg",	"../models/textures/rust.jpg",		
									"../models/textures/sand.jpg",		"../models/textures/crate_tex.jpg",		"../models/textures/sand_grass.jpg",
									"../models/textures/snow.jpg",		"../models/textures/finish.png",
									"../models/textures/bricks2.jpg",	"../models/textures/bricks3.jpg",		"../models/textures/grass2.png",
									"../models/textures/grass3.png",	"../models/textures/sand_grass2.jpg",	"../models/textures/sand_grass3.jpg",
									"../models/textures/sand2.jpg",		"../models/textures/sand3.jpg",			"../models/textures/snow2.jpg",
									"../models/textures/snow3.jpg",		"../models/textures/wall2.jpg",			"../models/textures/wall3.jpg",};
	unsigned int actual_texture;

	for(int i =0; i<(sizeof(file_name_texture)/sizeof(*file_name_texture)); i++){

		// load and create a texture 


		glGenTextures(1, &actual_texture);
		glBindTexture(GL_TEXTURE_2D, actual_texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		texture.push_back(actual_texture);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
			
		unsigned char *data2 = stbi_load(file_name_texture[i].c_str(), &width, &height, &nrChannels, 0);
		stbi_failure_reason();
		if (data2 != NULL)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
			glGenerateMipmap(GL_TEXTURE_2D);

		}
		else
		{
			//std::cout << "Failed to load texture" << std::endl;
			printf("Failed to load texture \n");
			return false;
		}
		stbi_image_free(data2);
	}

	return true;

}
bool initGlfw(){
	if (!glfwInit())
		return false;

	//Obtenemos el tamaño de la pantalla
	DEVMODE settings = {0}; 
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &settings); // Get current display settings
	SCREEN_WIDTH	= settings.dmPelsWidth;
	SCREEN_HEIGHT	= settings.dmPelsHeight;
	
	//SCREEN_WIDTH	= 1280;
	//SCREEN_HEIGHT	= 648;
	//SCREEN_WIDTH	= 800;
	//SCREEN_HEIGHT	= 648;

	centerX = SCREEN_WIDTH / 2.0; centerY = SCREEN_HEIGHT / 2.0;

	if(MessageBox(NULL,(LPCWSTR)L"Te gustaría ejecutar el programa en pantalla completa?", (LPCWSTR)L"Fullscreen", MB_ICONQUESTION | MB_YESNO) == IDYES)	{ 
		gWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Laberinto de Cajas 3D", glfwGetPrimaryMonitor(), NULL);
	}else{
		gWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Laberinto de Cajas 3D", NULL, NULL);
	}

	if (!gWindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(gWindow);

	const GLFWvidmode * vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(gWindow, (vidMode->width - SCREEN_WIDTH) >> 1, (vidMode->height - SCREEN_HEIGHT) >> 1);

	glfwSetWindowSizeCallback(gWindow, reshape);
	glfwSetKeyCallback(gWindow, keyInput);
	glfwSetMouseButtonCallback(gWindow, mouseButton);
	glfwSetCursorPosCallback(gWindow, cursorPos);
	glfwSetCharCallback(gWindow, charInput);
	glfwSetScrollCallback(gWindow, scroll_callback);
	//glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Desactiva el cursor

	//SetCursorPos(lastX,lastY);
	return true;
}
void initSkies(){
	vector<string> faces;
	faces.push_back("../models/textures/skybox/clouds_right.jpg");
	faces.push_back("../models/textures/skybox/clouds_left.jpg");
	faces.push_back("../models/textures/skybox/clouds_top.jpg");
	faces.push_back("../models/textures/skybox/clouds_top.jpg");
	faces.push_back("../models/textures/skybox/clouds_back.jpg");
	faces.push_back("../models/textures/skybox/clouds_front.jpg");
	skies.push_back(faces);
	faces.clear();

	
	faces.push_back("../models/textures/skybox/1right.jpg");
	faces.push_back("../models/textures/skybox/1left.jpg");
	faces.push_back("../models/textures/skybox/1top.jpg");
	faces.push_back("../models/textures/skybox/1bottom.jpg");
	faces.push_back("../models/textures/skybox/1back.jpg");
	faces.push_back("../models/textures/skybox/1front.jpg");
	skies.push_back(faces);
	faces.clear();

	faces.push_back("../models/textures/skybox/2right.jpg");
	faces.push_back("../models/textures/skybox/2left.jpg");
	faces.push_back("../models/textures/skybox/2top.jpg");
	faces.push_back("../models/textures/skybox/2top.jpg");
	faces.push_back("../models/textures/skybox/2back.jpg");
	faces.push_back("../models/textures/skybox/2front.jpg");
	skies.push_back(faces);
	faces.clear();

	faces.push_back("../models/textures/skybox/jajlands1_rt.jpg");
	faces.push_back("../models/textures/skybox/jajlands1_lf.jpg");
	faces.push_back("../models/textures/skybox/jajlands1_up.jpg");
	faces.push_back("../models/textures/skybox/jajlands1_dn.jpg");
	faces.push_back("../models/textures/skybox/jajlands1_bk.jpg");
	faces.push_back("../models/textures/skybox/jajlands1_ft.jpg");
	skies.push_back(faces);
	faces.clear();	

		
	faces.push_back("../models/textures/skybox/devils-tooth_rt.tga");
	faces.push_back("../models/textures/skybox/devils-tooth_lf.tga");
	faces.push_back("../models/textures/skybox/devils-tooth_up.tga");
	faces.push_back("../models/textures/skybox/devils-tooth_dn.tga");
	faces.push_back("../models/textures/skybox/devils-tooth_bk.tga");
	faces.push_back("../models/textures/skybox/devils-tooth_ft.tga");
	skies.push_back(faces);
	faces.clear();

	faces.push_back("../models/textures/skybox/jajdarkland1_rt.jpg");
	faces.push_back("../models/textures/skybox/jajdarkland1_lf.jpg");
	faces.push_back("../models/textures/skybox/jajdarkland1_up.jpg");
	faces.push_back("../models/textures/skybox/jajdarkland1_dn.jpg");
	faces.push_back("../models/textures/skybox/jajdarkland1_bk.jpg");
	faces.push_back("../models/textures/skybox/jajdarkland1_ft.jpg");
	skies.push_back(faces);
	faces.clear();
	
	

	
	faces.push_back("../models/textures/skybox/elbrus_left.jpg");
	faces.push_back("../models/textures/skybox/elbrus_right.jpg");
	faces.push_back("../models/textures/skybox/elbrus_top.jpg");
	faces.push_back("../models/textures/skybox/elbrus_top.jpg");
	faces.push_back("../models/textures/skybox/elbrus_back.jpg");
	faces.push_back("../models/textures/skybox/elbrus_front.jpg");
	skies.push_back(faces);
	faces.clear();

	
	faces.push_back("../models/textures/skybox/subzero_rt.tga");
	faces.push_back("../models/textures/skybox/subzero_lf.tga");
	faces.push_back("../models/textures/skybox/subzero_up.tga");
	faces.push_back("../models/textures/skybox/subzero_dn.tga");
	faces.push_back("../models/textures/skybox/subzero_bk.tga");
	faces.push_back("../models/textures/skybox/subzero_ft.tga");
	skies.push_back(faces);
	faces.clear();

	faces.push_back("../models/textures/skybox/thefog_rt.tga");
	faces.push_back("../models/textures/skybox/thefog_lf.tga");
	faces.push_back("../models/textures/skybox/thefog_up.tga");
	faces.push_back("../models/textures/skybox/thefog_dn.tga");
	faces.push_back("../models/textures/skybox/thefog_bk.tga");
	faces.push_back("../models/textures/skybox/thefog_ft.tga");
	skies.push_back(faces);
	faces.clear();

	faces.push_back("../models/textures/skybox/st_rt.tga");
	faces.push_back("../models/textures/skybox/st_lf.tga");
	faces.push_back("../models/textures/skybox/st_up.tga");
	faces.push_back("../models/textures/skybox/st_dn.tga");
	faces.push_back("../models/textures/skybox/st_bk.tga");
	faces.push_back("../models/textures/skybox/st_ft.tga");
	skies.push_back(faces);
	faces.clear();
	
}
bool initGlew(){
	if (glewInit() != GLEW_OK)
		return false;
	else
	{
		/*printf("Vendor: %s \n",			glGetString(GL_VENDOR));
		printf("Renderer: %s \n",		glGetString(GL_RENDERER));
		printf("OpenGL Version: %s \n", glGetString(GL_VERSION));
		printf("GLSL Version: %s \n\n",	glGetString(GL_SHADING_LANGUAGE_VERSION));*/

		//printf("\n\nShader: Color \n");
		
		
		glslProgram			= new CGLSLProgram();
		glslProgram		->loadFromFile("../shaders/phongShading.vert");
		glslProgram		->loadFromFile("../shaders/phongShading.frag");
		glslProgram		->create();
		glslProgram		->loadUniformVariables();
		glslProgram		->loadAttributeVariables();

		/*glslProgram2		= new CGLSLProgram();
		glslProgram2	->loadFromFile("../shaders/flatShading.vert");
		glslProgram2	->loadFromFile("../shaders/flatShading.frag");
		glslProgram2	->create();
		glslProgram2	->loadUniformVariables();
		glslProgram2	->loadAttributeVariables();

		glslProgram3		= new CGLSLProgram();
		glslProgram3	->loadFromFile("../shaders/gouraudShading.vert");
		glslProgram3	->loadFromFile("../shaders/gouraudShading.frag");
		glslProgram3	->create();
		glslProgram3	->loadUniformVariables();
		glslProgram3	->loadAttributeVariables();*/
		
		glslProgramLight	= new CGLSLProgram();
		glslProgramLight->loadFromFile("../shaders/colorLamp.vert");
		glslProgramLight->loadFromFile("../shaders/colorLamp.frag");
		glslProgramLight->create();
		glslProgramLight->loadUniformVariables();
		glslProgramLight->loadAttributeVariables();

		/*glslNormal			= new CGLSLProgram();
		glslNormal->loadFromFile("../shaders/colorNormal.vert");
		glslNormal->loadFromFile("../shaders/colorNormal.frag");
		glslNormal->create();
		glslNormal->loadUniformVariables();
		glslNormal->loadAttributeVariables();*/

		glslSkyBox			= new CGLSLProgram();
		glslSkyBox->loadFromFile("../shaders/skyBox.vert");
		glslSkyBox->loadFromFile("../shaders/skyBox.frag");
		glslSkyBox->create();
		glslSkyBox->loadUniformVariables();
		glslSkyBox->loadAttributeVariables();
		
		initSkies();
		skyBox1 = new skyBox(skies[level_actual],glslSkyBox);

		glslFont			= new CGLSLProgram();
		glslFont->loadFromFile("../shaders/text.vert");
		glslFont->loadFromFile("../shaders/text.frag");
		glslFont->create();
		glslFont->loadUniformVariables();
		glslFont->loadAttributeVariables();

		return true;
	}
}
bool initUserInterface()
{
	if (!TwInit(TW_OPENGL, NULL))
		return false;

	userInterface = CUserInterface::Instance();

	return true;
}
bool initFreeType(){
	text = new FreeTypeFont(SCREEN_WIDTH, SCREEN_HEIGHT,glslFont);

	return true;

}
string openfilename(){
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	
	wchar_t input[256] = {};
	ofn.lpstrFile = input;
	//ofn.lpstrFile = '\0';

	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	//ofn.lpstrDefExt = "";

	if (GetOpenFileName(&ofn)){
		wstring ws(input);
		string str(ws.begin(), ws.end());
		return str;
	}
	return "";
}
void sound(string filename,bool repeat){
	if(repeat)
		SoundEngine->play2D(filename.c_str(), GL_TRUE);
	else
		SoundEngine->play2D(filename.c_str(), GL_FALSE);
}
void sound_stop(bool all, string sound){
	if (all)
		SoundEngine->stopAllSounds();
	else
		SoundEngine->removeSoundSource(sound.c_str()); 
}
void pantalla_inicial(){
	fog_active[0] = false;
	sound_stop(true,"");
	borrar_objetos();
	level_actual = 0;
	bool load = false;
	pantallas_activas[0] = true;
	pantalla_menu = true;
	load = readScene("..\\scenes\\scene1.txt", models);
	if(!load){
		printf("Error al cargar el nivel\n");
	}
	camera_first_person = false;

	camera.Position.x = -0.006569;
	camera.Position.y = 10.8357;
	camera.Position.z = 10.888;
	camera.Yaw = 270.399;
	camera.Pitch = -44.899;
	camera.updateCameraVectors();
	flying_camera = false;

	models[picked]->setActivateTexture(false);
	sound("..\\sounds\\start_1 ILLUMINATI CONFIRMED.mp3",true);
}
void pantalla_juego(){
	//Texto
	char texto[255];
	int segundos = difftime( time(0), tiempo);

	sprintf_s(texto, "Segundos %i", segundos);
	text->print(glslFont, texto,20.f,50.f, 0.4f, glm::vec3(0.9f, 0.9f, 0.9f));

	sprintf_s(texto, "Level Actual %d", level_actual + 1);
	text->print(glslFont, texto,20.f,20.f, 0.6f, glm::vec3(0.3f, 0.7f, 0.9f));

	
	if(!jugadores.empty() && jugador_actual != -1){
		int xpos;
		sprintf_s(texto, "%s  ", jugadores[jugador_actual]->nombre.c_str());
		xpos = text->print(glslFont, texto, 20.f,SCREEN_HEIGHT - 40, 0.6f, glm::vec3(0.3f, 0.3f, 0.8f));
		sprintf_s(texto, "%s",jugadores[jugador_actual]->titulos[jugadores[jugador_actual]->titulo_actual].c_str());
		text->print(glslFont, texto, xpos,SCREEN_HEIGHT - 40, 0.6f, jugadores[jugador_actual]->titulos_colores[jugadores[jugador_actual]->titulo_actual] );
	}
	if((pantalla_menu && pantallas_activas[0]) || ayuda){
		sprintf_s(texto, "El Laberinto de Cajas 3D");
		text->print(glslFont, texto,centerX - sizeof(texto)+70,SCREEN_HEIGHT - 100, 0.5f, glm::vec3(8.0f, 8.0f, 0.0f));

		sprintf_s(texto, "Controles Movimiento: W,A,S,D");
		text->print(glslFont, texto,20,SCREEN_HEIGHT - 190, 0.5f, glm::vec3(7.0f, 0.0f, 0.0f));

		sprintf_s(texto, "Linterna: <1>, <2>");
		text->print(glslFont, texto,20,SCREEN_HEIGHT - 230, 0.5f, glm::vec3(7.0f, 0.0f, 0.0f));

		sprintf_s(texto, "Activar Pista: <3>");
		text->print(glslFont, texto,20,SCREEN_HEIGHT - 270, 0.5f, glm::vec3(7.0f, 0.0f, 0.0f));

		sprintf_s(texto, "Cambiar entre titulos disponibles: <4>");
		text->print(glslFont, texto,20,SCREEN_HEIGHT - 310, 0.5f, glm::vec3(7.0f, 0.0f, 0.0f));

		sprintf_s(texto, "Rendirde y repetir nivel: <espacio>");
		text->print(glslFont, texto,20,SCREEN_HEIGHT - 350, 0.5f, glm::vec3(7.0f, 0.0f, 0.0f));

		sprintf_s(texto, "Mostrar comandos: <tab>");
		text->print(glslFont, texto,20,SCREEN_HEIGHT - 390, 0.5f, glm::vec3(7.0f, 0.0f, 0.0f));

		sprintf_s(texto, "Ir a menu principal: <F1>");
		text->print(glslFont, texto,20,SCREEN_HEIGHT - 430, 0.5f, glm::vec3(7.0f, 0.0f, 0.0f));

		sprintf_s(texto, "Cambiar de usuario: <F5>");
		text->print(glslFont, texto,20,SCREEN_HEIGHT - 470, 0.5f, glm::vec3(7.0f, 0.0f, 0.0f));

		sprintf_s(texto, "Salir: <ESC>");
		text->print(glslFont, texto,20,SCREEN_HEIGHT - 510, 0.5f, glm::vec3(7.0f, 0.0f, 0.0f));
	}

	if((pantalla_menu && pantallas_activas[0])){
		if(jugadores.empty()){
			//Debe registrarse para continuar

			sprintf_s(texto, "Presione <F5> para registrarse");
			if(segundos % 2 == 1){
				text->print(glslFont, texto,centerX - sizeof(texto)+65,centerY - sizeof(texto) + 70, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
			}else{
				text->print(glslFont, texto,centerX - sizeof(texto)+65,centerY - sizeof(texto) + 70, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}else{
		
			sprintf_s(texto, "Presione <Enter> para iniciar el juego");
			if(segundos % 2 == 1){
				text->print(glslFont, texto,centerX - sizeof(texto)+20,centerY - sizeof(texto) + 70, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
			}else{
				text->print(glslFont, texto,centerX - sizeof(texto)+20,centerY - sizeof(texto) + 70, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
		

	}else if(pantalla_menu && pantallas_activas[1]){
		sprintf_s(texto, "Presione <Enter> para iniciar el juego");
		if(segundos % 2 == 1){
			text->print(glslFont, texto,centerX - sizeof(texto)+20,centerY - sizeof(texto) + 70, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		}else{
			text->print(glslFont, texto,centerX - sizeof(texto)+20,centerY - sizeof(texto) + 70, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
	}

	control_pista();

}
void pantalla_resultados(){
	sound(sounds_win[1],false); //Paso de nivel
	//bool load = false;
	pantallas_activas[1] = true;
	pantalla_menu = true;
	//load = readScene("..\\scenes\\scene1.txt", models);
	//if(!load){
		//printf("Error al cargar el nivel\n");
	//}
	camera_first_person = false;
	camera.Position.x = -0.006569;
	camera.Position.y = 10.8357;
	camera.Position.z = 10.888;
	flying_camera = true;

	borrar_objetos(); //Limpiamos la escena
}
void cambiar_jugador(){
	
	if(jugador_actual == -1){
		Player *aux;
		if(MessageBox(NULL,(LPCWSTR)L"Eres un nuevo jugador?", (LPCWSTR)L"Inicio", MB_ICONQUESTION | MB_YESNO) == IDYES)	{ 
			printf("Ingresa tu nombre!\n");
			string nombre = "Sin nombre";
			getline (std::cin,nombre);
			aux = new Player(nombre);
		}else{
			
			string filename = openfilename();
			//printf("filename %s",filename.c_str());
			if(filename != ""){
				aux = new Player(0,filename);
			}else{
				aux = new Player("Sin nombre");
			}
		}
		jugadores.push_back(aux);
		jugador_actual = jugadores.size() - 1;

		level_actual = -1;
		aumentar_nivel(true);
	}else{
		jugadores[jugador_actual]->save_data();
		jugador_actual = -1;
		


		Player *aux;
		if(MessageBox(NULL,(LPCWSTR)L"Eres un nuevo jugador?", (LPCWSTR)L"Inicio", MB_ICONQUESTION | MB_YESNO) == IDYES)	{ 
			printf("Ingresa tu nombre!\n");
			string nombre = "Sin nombre";
			getline (std::cin,nombre);
			aux = new Player(nombre);
		}else{
			
			string filename = openfilename();
			printf("filename %s",filename.c_str());
			if(filename != ""){
				aux = new Player(0,filename);
			}else{
				aux = new Player("Sin nombre");
			}
		}
		jugadores.push_back(aux);
		jugador_actual = jugadores.size() - 1;

		level_actual = -1;
		pantalla_inicial();
		//aumentar_nivel(true);
	}
	

}
bool initScene()
{
	//int tam_laberinto = 3;
	picked = -1;
	if(!loadTextures())
		return false;


	file_name = "../models/sphere.obj"; // Va ha ser la luz del modelo
	file_name_texture = "Hola";

	//Esfera de Luz
	userInterface->setFileName(file_name);
	if(!light_sphere->load(file_name)){
		printf("Error, no pudo cargar la sphera de luz %s \n",file_name.c_str());
		return false;
	}

	//juego = new juegoControl(SCREEN_WIDTH,SCREEN_HEIGHT,text,skyBox1,skies,texture);

	
	pantalla_inicial();


	/*
	printf("\n         --**Juego El Laberinto de Cajas**--\n\n");
	printf(" - Debes encontrar el portal que te llevará al siguiente nivel...\n\n");

	printf(" - Controles : \n");
	printf("    - Desplazamiento: W,A,S,D\n    - Alt: Activar camara estatica\n    - Ctrl: Activar movilidad de camara con mouse\n    - Desplazar camara: flechas de dirección\n    - ESC: Salir del juego\n\n");


	cambiar_jugador();
	sound(sounds_nivel[0],true);
	bool load = readScene("..\\scenes\\scene1.txt", models);
	if(camera_first_person){
		camera.Position.x = models[picked]->getTranslation()[0];
		camera.Position.y = models[picked]->getTranslation()[1];
		camera.Position.z = models[picked]->getTranslation()[2] - 1.5f;
	}
	if(!load){
		return false;
	}
	
	*/



	return true;

}
bool readScene(string filename, vector	<CModel *>	&scene){
	picked = -1;
	bool error = false;
	char line[256];
	char text[256];
	float material[10];
	float model[11];
	int matches;
	ifstream file(filename);  

	if(!file.is_open()) {
		printf("Error, no se puede abrir el archivo %s\n",filename.c_str());
        return false;
    }

	while(!file.eof())    { //Leemos el archivo
		file.getline(line,256);
		if ( line[0] == 'p' && line[1] == 'a' && line[2] == 'r' && line[3] == 'e' && line[4] == 'd'){  //Pared de laberinto
			matches = sscanf_s(line, "pared %f %f %f %f %f %f %f %f %f %f %f  %f %f %f %f %f %f %f %f %f %f \"%255[^\"]\" \n",&model[0],&model[1],&model[2],&model[3],&model[4],&model[5],&model[6],&model[7],&model[8],&model[9],&model[10], &material[0], &material[1], &material[2], &material[3], &material[4], &material[5], &material[6], &material[7], &material[8], &material[9],text);
			//printf("\nmatches: %d\n",matches);
			//printf("pared %s %f %f %f %f %f %f %f %f %f %f %f \n",text,model[0],model[1],model[2],model[3],model[4],model[5],model[6],model[7],model[8],model[9],model[10]);
			//printf("pared %f %f %f %f %f %f %f %f %f %f\n",material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
			error = agregar_objeto2(scene, text, model[0], model[1], model[2], model[3], model[4], model[5], model[6], model[7], model[8], model[9], model[10],material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
		}else if ( line[0] == 'p' && line[1] == 'i' && line[2] == 's' && line[3] == 'o' ){  //Pared de laberinto
			matches = sscanf_s(line, "piso %f %f %f %f %f %f %f %f %f %f %f  %f %f %f %f %f %f %f %f %f %f \"%255[^\"]\" \n",&model[0],&model[1],&model[2],&model[3],&model[4],&model[5],&model[6],&model[7],&model[8],&model[9],&model[10], &material[0], &material[1], &material[2], &material[3], &material[4], &material[5], &material[6], &material[7], &material[8], &material[9],text);
			//printf("\nmatches: %d\n",matches);
			//printf("piso %s %f %f %f %f %f %f %f %f %f %f %f \n",text,model[0],model[1],model[2],model[3],model[4],model[5],model[6],model[7],model[8],model[9],model[10]);
			//printf("piso %f %f %f %f %f %f %f %f %f %f\n",material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
			
			error = agregar_objeto2(scene, text, model[0], model[1], model[2], model[3], model[4], model[5], model[6], model[7], model[8], model[9], model[10],material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
			if(!error){
				ground = picked;
				float mModelTranslation[3]	= {-models[ground]->init_center.x, -models[ground]->init_center.y - 7, -models[ground]->init_center.z}; 
				models[ground]->setTranslation(mModelTranslation);
			}
		}else if ( line[0] == 'l' && line[1] == 'l' && line[2] == 'e' && line[3] == 'g' && line[4] == 'a' && line[5] == 'd' && line[6] == 'a'){  //Pared de laberinto
			matches = sscanf_s(line, "llegada %f %f %f %f %f %f %f %f %f %f %f  %f %f %f %f %f %f %f %f %f %f \"%255[^\"]\" \n",&model[0],&model[1],&model[2],&model[3],&model[4],&model[5],&model[6],&model[7],&model[8],&model[9],&model[10], &material[0], &material[1], &material[2], &material[3], &material[4], &material[5], &material[6], &material[7], &material[8], &material[9],text);
			//printf("\nmatches: %d\n",matches);
			//printf("llegada %s %f %f %f %f %f %f %f %f %f %f %f \n",text,model[0],model[1],model[2],model[3],model[4],model[5],model[6],model[7],model[8],model[9],model[10]);
			//printf("llegada %f %f %f %f %f %f %f %f %f %f\n",material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
			
			error = agregar_objeto2(scene, text, model[0], model[1], model[2], model[3], model[4], model[5], model[6], model[7], model[8], model[9], model[10],material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
			if(!error){
				finish = picked;
			}
		}else if ( line[0] == 'p' && line[1] == 'i' && line[2] == 's' && line[3] == 't' && line[4] == 'a'){  //Pared de laberinto
			matches = sscanf_s(line, "pista %f %f %f %f %f %f %f %f %f %f %f  %f %f %f %f %f %f %f %f %f %f \"%255[^\"]\" \n",&model[0],&model[1],&model[2],&model[3],&model[4],&model[5],&model[6],&model[7],&model[8],&model[9],&model[10], &material[0], &material[1], &material[2], &material[3], &material[4], &material[5], &material[6], &material[7], &material[8], &material[9],text);
			//printf("\nmatches: %d\n",matches);
			//printf("llegada %s %f %f %f %f %f %f %f %f %f %f %f \n",text,model[0],model[1],model[2],model[3],model[4],model[5],model[6],model[7],model[8],model[9],model[10]);
			//printf("llegada %f %f %f %f %f %f %f %f %f %f\n",material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
			
			error = agregar_objeto2(scene, text, model[0], model[1], model[2], model[3], model[4], model[5], model[6], model[7], model[8], model[9], model[10],material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
			if(!error){
				pista_llegada = picked;
			}
		}else if ( line[0] == 'j' && line[1] == 'u' && line[2] == 'g' && line[3] == 'a' && line[4] == 'd' && line[5] == 'o' && line[6] == 'r'){  //Pared de laberinto
			matches = sscanf_s(line, "jugador %f %f %f %f %f %f %f %f %f %f %f  %f %f %f %f %f %f %f %f %f %f \"%255[^\"]\" \n",&model[0],&model[1],&model[2],&model[3],&model[4],&model[5],&model[6],&model[7],&model[8],&model[9],&model[10], &material[0], &material[1], &material[2], &material[3], &material[4], &material[5], &material[6], &material[7], &material[8], &material[9],text);
			//printf("\nmatches: %d\n",matches);
			//printf("jugador %s %f %f %f %f %f %f %f %f %f %f %f \n",text,model[0],model[1],model[2],model[3],model[4],model[5],model[6],model[7],model[8],model[9],model[10]);
			//printf("jugador %f %f %f %f %f %f %f %f %f %f\n",material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
			error = agregar_objeto2(scene, text, model[0], model[1], model[2], model[3], model[4], model[5], model[6], model[7], model[8], model[9], model[10],material[0], material[1], material[2], material[3], material[4], material[5], material[6], material[7], material[8], material[9]);
		}
		if(error){
			
			return false;
			
		}
	}

	return true;
}
void reiniciar(){
	//pantalla_resultados();
	aumentar_nivel(false);
}
void aumentar_nivel(bool aux){
	
	tiempo = time(0);

	bool load = true;
	
	pista_llegada_activa = 1;

	if(aux){
		sound(sounds_win[1],false); //Paso de nivel

		//if(!pantallas_activas[1]){
			if(level_actual == max_level){
				printf("\n - Has completado el laberinto\n - !Felecitaciones\n!");
				//level_actual = -1;
				pantalla_inicial();
				level_actual = 0;
			}else{
				level_actual++;
			}
		//}
	}

	//if(pantallas_activas[1]){
		borrar_objetos(); //Limpiamos la escena
	//}

	//if(!pantallas_activas[1]){

		if(level_actual == 0){
			fog_active[0] = 0;
			load = readScene("..\\scenes\\scene1.txt", models);
		}else if(level_actual == 1){
			load = readScene("..\\scenes\\scene2.txt", models);
		}else if(level_actual == 2){
			load = readScene("..\\scenes\\scene3.txt", models);
		}else if(level_actual == 3){
			FogParameters::fDensity = 0.3f;
			FogParameters::iFogEquation = FOG_EQUATION_EXP2;
			FogParameters::vFogColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
			fog_active[0] = 1;
			load = readScene("..\\scenes\\scene4.txt", models);
		}else if(level_actual == 4){
			FogParameters::fDensity = 0.6f;
			FogParameters::iFogEquation = FOG_EQUATION_EXP;
			FogParameters::vFogColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
			fog_active[0] = 1;
			load = readScene("..\\scenes\\scene5.txt", models);
			//initScene5();
		}else if(level_actual == 5){
			load = readScene("..\\scenes\\scene6.txt", models);
			//initScene5();
		}else if(level_actual == 6){
			FogParameters::fDensity = 0.6f;
			FogParameters::iFogEquation = FOG_EQUATION_EXP2;
			FogParameters::vFogColor = glm::vec4(0.6f, 0.6f, 0.9f, 1.0f);
			fog_active[0] = 1;
			load = readScene("..\\scenes\\scene7.txt", models);
			//initScene5();
		}else if(level_actual == 7){
			FogParameters::fDensity = 0.7f;
			FogParameters::iFogEquation = FOG_EQUATION_EXP;
			FogParameters::vFogColor = glm::vec4(0.9f, 0.6f, 0.6f, 1.0f);
			fog_active[0] = 1;
			load = readScene("..\\scenes\\scene8.txt", models);
			//initScene5();
		}else if(level_actual == 8){
			FogParameters::fDensity = 0.9f;
			FogParameters::iFogEquation = FOG_EQUATION_EXP;
			FogParameters::vFogColor = glm::vec4(0.7f, 0.6f, 0.7f, 1.0f);
			fog_active[0] = 1;
			load = readScene("..\\scenes\\scene9.txt", models);
			//initScene5();
		}else if(level_actual == 9){
			FogParameters::fDensity = 0.4f;
			FogParameters::iFogEquation = FOG_EQUATION_EXP2;
			FogParameters::vFogColor = glm::vec4(0.8f, 0.8f, 0.9f, 1.0f);
			fog_active[0] = 1;
			load = readScene("..\\scenes\\scene10.txt", models);
			//initScene5();
		}

		if(!load){
			printf("Error al cargar el nivel\n");
	
		}else{
			sound_stop(true,"");
			if(level_actual != 0){
				//sound_stop(false,sounds_nivel[level_actual-1]);
				sound(sounds_nivel[level_actual],true);
			}else{
				//sound_stop(true,"");
				sound(sounds_nivel[level_actual],true);
			}
			
			//printf(" - Level Actual: %d \n",level_actual + 1);

			//models[ground]->setTexture(texture[level_actual]); 
			skyBox1->change_texture(skies[level_actual]);

			if(camera_first_person){
				camera.Position.x = models[picked]->getTranslation()[0];
				camera.Position.y = models[picked]->getTranslation()[1];
				camera.Position.z = models[picked]->getTranslation()[2] - 1.5f;
			}

		}
	//}
}
void activar_pista(){
	
	if(pista_llegada_activa == 1){
		
		tiempo_pista= time(0);
	}else{
	
	}
	pista_llegada_activa = !pista_llegada_activa;
}
void control_pista(){
	if(pista_llegada_activa == 0){
		int segundos;
		char texto[255];
		int tiempo = difftime( time(0), tiempo_pista);
		if(tiempo <= pista_tiempo_max){
			segundos = pista_tiempo_max - tiempo;

			sprintf_s(texto, "Hay una pista activa, revisa tus alrededores %i", segundos);
			if(segundos % 2 == 1){
				text->print(glslFont, texto,centerX - sizeof(texto),centerY - sizeof(texto) + 10, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
			}else{
				text->print(glslFont, texto,centerX - sizeof(texto),centerY - sizeof(texto) + 10, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
			}


		}else if(tiempo > pista_tiempo_max){
			//tiempo_pista = time(0);
			pista_llegada_activa = !pista_llegada_activa;
		}
	}
}
