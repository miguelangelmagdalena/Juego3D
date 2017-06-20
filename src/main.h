#pragma once
#include "GLSLProgram.h"
#include "UserInterface.h"
#include "freeTypeFont.h";
#include "Model.h"
#include "Off.h"
#include "Obj.h"
#include "Light.h"
#include "skyBox.h"
#include "camera.h"
#include <irrklang/irrKlang.h>
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "juegoControl.h"
#include <time.h>
#include "player.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define max_level 4

int SCREEN_WIDTH  =1280;
int SCREEN_HEIGHT =720;


GLFWwindow		*gWindow;
CUserInterface	*userInterface;

//FreeType
FreeTypeFont	*text ;

//Objetos
vector			<CModel *>	models;

CModel			* light_sphere = new Light();
int picked  = -1;
int ground  = 0;
int finish  = 1;
vector			<unsigned int> texture;
int level_actual	= 0;


//SkyBox
skyBox *skyBox1;
vector<vector<string>> skies;


figure_type		view_option = PERSPECTIVA;
string			file_name, file_name_texture;

//Shaders
CGLSLProgram	*glslProgram, 
				*glslProgram2, 
				*glslProgram3, 
				*glslProgramLight, 
				*glslNormal, 
				*glslSkyBox,
				*glslFont;


// Camara
Camera camera(glm::vec3(0.0f, 10.0f, 10.0f));
float lastX = (float)SCREEN_WIDTH / 2.0;
float lastY = (float)SCREEN_HEIGHT / 2.0;
double centerX = lastX, centerY = lastY;
bool flying_camera			= true;
bool camera_static			= false;
bool camera_first_person	= true;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Matrices
glm::mat4 projection, view;

//IrrKlang para Sonidos
using namespace irrklang;
ISoundEngine *SoundEngine = createIrrKlangDevice();

//Juego
//juegoControl *juego;
vector <Player *> jugadores;
int jugador_actual = -1;
string sounds_nivel[] = {"..\\sounds\\nivel_1 Cinema Sins Background Song.mp3","..\\sounds\\nivel_2 Sneaky Snitch (Kevin MacLeod).mp3","..\\sounds\\nivel_3 Monkeys Spinning Monkeys (Kevin MacLeod).mp3","..\\sounds\\nivel_4 Scheming Weasel Faster.mp3","..\\sounds\\nivel_5 Investigations (Kevin MacLeod).mp3","..\\sounds\\nivel_6 Dragon Ball Z - Freezer Planet.mp3","..\\sounds\\nivel_7 GPS Carupanero Cesar Munoz.mp3","..\\sounds\\nivel_8 breakout.mp3","..\\sounds\\nivel_9 Flyff OST 08 - Mystery (192kbit_AAC).mp3","..\\sounds\\nivel_10 Mozart twinkle twinkle little star.mp3"};
string sounds_win  [] = {"..\\sounds\\win_0.mp3","..\\sounds\\win_1.mp3"};
string sounds_fail [] = {"..\\sounds\\fail_0.mp3","..\\sounds\\fail_1.mp3","..\\sounds\\fail_2.mp3","..\\sounds\\fail_3.mp3"};
string sounds_bonus[] = {"..\\sounds\\pista_llegada.mp3"};
string titulos [] = {"El jugador solitario","El papa de los helados","El perdido en la vida","El que no sabe que hacer","El echon","El inconforme"};
time_t tiempo = time(0);
time_t tiempo_aux = time(0);
int flashlight = 0;
int flashlight_fog = 0;
int pista_llegada_activa = 1;
int pista_llegada = 0;
time_t tiempo_pista= time(0);
int pista_tiempo_max = 5;
bool pantalla_menu = true;
bool pantallas_activas[] = {true,false,false};
bool ayuda = false;

#define FOG_EQUATION_LINEAR		0
#define FOG_EQUATION_EXP		1
#define FOG_EQUATION_EXP2		2

namespace FogParameters
{
	float fDensity = 0.6f;
	float fStart = 10.0f;
	float fEnd = 75.0f;
	glm::vec4 vFogColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	int iFogEquation = FOG_EQUATION_EXP; // 0 = linear, 1 = exp, 2 = exp2
};
int fog_active[1];


void display(); //Muestra todos los objetos de la escena
void borrar_objetos();
void updateUserInterface();
void SetQuaternionFromAxisAngle(const float *axis, float angle, float *quat);
void ConvertQuaternionToMatrix(const float *quat, float *mat);
void MultiplyQuaternions(const float *q1, const float *q2, float *qout);
void reshape(GLFWwindow *window, int width, int height);
bool checkCollision(CModel *A, CModel *B);

void keyInput(GLFWwindow *window, int key, int scancode, int action, int mods);
bool type_object(string name);
bool agregar_objeto(string file_name, int index_texture, float tx, float ty, float tz, float sx, float sy, float sz, float rx, float ry, float rz, float rw);
bool agregar_objeto2(vector	<CModel *>	&models,string file_name, int index_texture, float tx, float ty, float tz, float sx, float sy, float sz, float rx, float ry, float rz, float rw, float mat_ambient1, float mat_ambient2, float mat_ambient3, float mat_difuso1, float mat_difuso2, float mat_difuso3, float mat_especular1, float mat_especular2, float mat_especular3, float mat_shininess);
void mouseButton(GLFWwindow* window, int button, int action, int mods);
void cursorPos(GLFWwindow* window, double x, double y);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void charInput(GLFWwindow* window, unsigned int scanChar);
void destroy();
bool initGlfw();
void initSkies();
bool initGlew();
bool initUserInterface();
bool loadTextures();
bool initFreeType();

string openfilename();
//Para control del juego
void sound(string filename,bool repeat);
void sound_stop(bool all, string sound);

bool readScene(string filename, vector	<CModel *>	&scene);
void pantalla_inicial();
void pantalla_juego();
void pantalla_resultados();
void cambiar_jugador();
bool initScene();

void reiniciar();
void aumentar_nivel(bool aux);
void activar_pista();
void control_pista();
