// STD C++ Libs.
#include <iostream>
#include <cmath>
#include <fstream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"



// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(23.0f, 4.0f, -1.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float turbo = 0.0;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 SpotlightPos(2.1f, 5.0f, 4.0f);
glm::vec3 SpotlightDir(0.0f, -1.0f, 0.0f);

GLfloat PointLinear, PointQuad;
bool active;

//Variables para Animacion
float rotPuertaD, rotPuertaI, PeriodoPendulo;
bool PuertaA = false;
bool PuertaC = false;
bool Tetera = false;

float TeteraX, TeteraZ, TrotX, TrotY, TrotZ = 0.0;	//Variables para la tetera.
float  TeteraY = 1.0f;



bool Pendulo = false;

//KEYFRAMES:
glm::vec3 PosIni(0.0f, 1.0f, 0.0f);
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotX = 0, rotY = 0, rotZ = 0;




#define MAX_FRAMES 16
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotX;
	float rotY;
	float rotZ;
	float rotIncX;
	float rotIncY;
	float rotIncZ;
	
	//Variables para GUARDAR Key Frames
	_frame() {
		
	}

	/*
		Tomamos los elementos relevantes de cada frame: (posición y rotación). 
		Los guardaremos en un archivo de texto con el nombre del parametro filename.
	*/
	void exportar(std::string filename) {
		ofstream archivo;
		archivo.open(filename, ios::out | ios::app );
		archivo << this->posX << " " << this->posY << " " << this->posZ << " "
				<< this->rotX << " " << this->rotY << " " << this->rotZ << std::endl;
		archivo.close();
	}

	
	
}FRAME;


FRAME KeyFrame[MAX_FRAMES];



int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;


// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-3.2968f, 6.30228f,  -0.614378f),
	glm::vec3(-2.87144f, 6.35246f, -1.51074f),
	glm::vec3(-3.30395f, 6.35236f, -2.39931f),
	glm::vec3(-4.13717f, 6.36045f, -2.73863f),
	glm::vec3(-5.05477f, 6.32748f, -2.38543f),
	glm::vec3(-5.39629f, 6.36181f, -1.51754f),
	glm::vec3(-5.06521f, 6.28334f, -0.618727f),
	glm::vec3(-4.16229f, 6.31102f, -0.271806f),
	glm::vec3(-4.18803f, 6.98306f, -0.599339f),
	glm::vec3(-3.34558f, 6.94956f, -1.05675f),
	glm::vec3(-3.35675f, 6.92751f, -1.8845f),
	glm::vec3(-4.19585f, 6.89064f, -2.42919f),
	glm::vec3(-4.96395f, 6.94246f, -1.95142f),
	glm::vec3(-4.94268f, 6.88196f, -1.13767f)
	/*
	X: 0.673034Y: 22.5674Z: 0.998614
	X: -11.7847Y: 37.0226Z: -23.0401
	X: -11.7366Y: 36.4252Z: -27.1644
	X: -14.9185Y: 36.7041Z: -27.5463
	X: -15.1319Y: 36.4794Z: -24.973
	X: -10.6865Y: 30.6277Z: -15.5078

	TO DO:
	*/

};


//Funciones para los keyframes:
void importar_KeyFrames(std::string filename, int& Index, FRAME* KeyFrameArray) {

	FRAME tempframe;
	std::string linea;
	//int num_lin = 0;
	ifstream archivo(filename);

	while (getline(archivo, linea))
	{
		istringstream linestream(linea);
		string valor;

		//Leemos hasta la primera coma
		getline(linestream, valor, ' ');
		tempframe.posX = atof(valor.c_str());

		getline(linestream, valor, ' ');
		tempframe.posY = atof(valor.c_str());

		getline(linestream, valor, ' ');
		tempframe.posZ = atof(valor.c_str());

		getline(linestream, valor, ' ');
		tempframe.rotX = atof(valor.c_str());

		getline(linestream, valor, ' ');
		tempframe.rotY = atof(valor.c_str());

		getline(linestream, valor, ' ');
		tempframe.rotZ = atof(valor.c_str());

		KeyFrameArray[Index] = tempframe;

		Index++;

	}

}


void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotX = rotX;
	KeyFrame[FrameIndex].rotY = rotY;
	KeyFrame[FrameIndex].rotZ = rotZ;

	KeyFrame[FrameIndex].exportar("Anims/temp.keyframe");
	std::cout << KeyFrame[FrameIndex].posX << endl;


	FrameIndex++;
}

void resetElements(void)
{
	TeteraX = KeyFrame[0].posX;
	TeteraY = KeyFrame[0].posY;
	TeteraZ = KeyFrame[0].posZ;

	TrotX = KeyFrame[0].rotX;
	TrotY = KeyFrame[0].rotY;
	TrotZ = KeyFrame[0].rotZ;
	
	//posX = KeyFrame[0].posX;
	//posY = KeyFrame[0].posY;
	//posZ = KeyFrame[0].posZ;

	//rotX = KeyFrame[0].rotX;
	//rotY = KeyFrame[0].rotY;
	//rotZ = KeyFrame[0].rotZ;

}

void clearKeyFrames(void) {

	FRAME empty;

	for (size_t i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i] = empty;
	}

	

		

}

void interpolation(FRAME* KeyFrame, int& Index)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	KeyFrame[playIndex].rotIncX = (KeyFrame[playIndex + 1].rotX - KeyFrame[playIndex].rotX) / i_max_steps;
	KeyFrame[playIndex].rotIncY = (KeyFrame[playIndex + 1].rotY - KeyFrame[playIndex].rotY) / i_max_steps;
	KeyFrame[playIndex].rotIncZ = (KeyFrame[playIndex + 1].rotZ - KeyFrame[playIndex].rotZ) / i_max_steps;
	
}


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animacion();


float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame



int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	

	
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	//Carga de Modelos 3D. 
	Model Piso((char*)"Models/Pisos/Piso.obj");
	
	//Modelos Adicionales.
	Model pasillo((char*)"Models/Extras/Pasillo.obj");
	Model skybox((char*)"Models/Extras/SkyBox.obj");
	Model techo((char*)"Models/Extras/Techo1.obj");
	Model Fachada((char*)"Models/Fachada/FavHQ1.obj");
	Model tapete((char*)"Models/Tapete/Tapete.obj");

	//Modelos para el cuarto.
	Model reloj((char*)"Models/Reloj/Reloj1.obj");
	Model mantelTetera((char*)"Models/Teaset/Mantel.obj");
	Model tazas((char*)"Models/Teaset/Tazas.obj");
	Model escritorio((char*)"Models/Escritorio/Escritorio.obj");
	Model escritorioTe((char*)"Models/EscritorioTe/EscritorioTe.obj");
	Model candelabro((char*)"Models/Candelabro/Candelabro.obj");
	Model escudo((char*)"Models/Escudo/Emblem.obj");
	Model teapot((char*)"Models/Teaset/Tetera.obj");


	//Librero y sus variantes.
	Model librero((char*)"Models/Libreros/Librero.obj");
	Model libreroAncho((char*)"Models/Libreros/LibreroAncho.obj");
	Model librerosVar((char*)"Models/Libreros/LibrerosAnchoVar.obj");

	//Modelos animados.
	Model puertader((char*)"Models/Fachada/PuertaDer.obj");
	Model puertaizq((char*)"Models/Fachada/PuertaIzq.obj");
	Model pendulo((char*)"Models/Reloj/Badajo.obj");




	
	//Modelos de los ambientes.
	//Model cuarto1((char*)"Models/Ambientes/Cuarto1.obj");
	Model cuarto2((char*)"Models/Ambientes/Cuarto2.obj");


	
	//Inicialización de KeyFrames

	importar_KeyFrames("Anims/tetera.keyframe", FrameIndex, KeyFrame);


	for (size_t i = 0; i < FrameIndex; i++)
	{
		std::cout << KeyFrame[i].posX << " " << KeyFrame[i].posY << " " << KeyFrame[i].posZ << " " << endl;
	}

	/*for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotX = 0;
		KeyFrame[i].rotY = 0;
		KeyFrame[i].rotZ = 0;
		KeyFrame[i].rotIncX = 0;
		KeyFrame[i].rotIncY = 0;
		KeyFrame[i].rotIncZ = 0;

	}*/



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 150.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animacion();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		

		

		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);

		//Point Lights

		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = Light1.x; //abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = Light1.y; //abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = Light1.z; //sin(glfwGetTime() * Light1.z);

		PointLinear = 0.7f;
		PointQuad = 1.8f;

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].position"), pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].position"), pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].position"), pointLightPositions[10].x, pointLightPositions[10].y, pointLightPositions[10].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].position"), pointLightPositions[11].x, pointLightPositions[11].y, pointLightPositions[11].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[11].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[11].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[11].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].position"), pointLightPositions[12].x, pointLightPositions[12].y, pointLightPositions[12].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[12].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[12].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[12].quadratic"), PointQuad);

		//Point Light X
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].position"), pointLightPositions[13].x, pointLightPositions[13].y, pointLightPositions[13].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[13].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[13].linear"), PointLinear);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[13].quadratic"), PointQuad);

		
		//TODO: añadir luces al exterior




		

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);
		glm::mat4 modelRoom(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		

		//Carga de más objetos. 
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);

		//Dibujo de Objetos:
		tapete.Draw(lightingShader); //listo!
		
		//maqueta.Draw(lightingShader);

		//Dibujamos la Fachada
		Fachada.Draw(lightingShader);

		//Dibujamos los ambientes.
		modelRoom = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRoom));
		cuarto2.Draw(lightingShader);

		//Dibujamos los objetos adicionales
		pasillo.Draw(lightingShader);
		techo.Draw(lightingShader);
		skybox.Draw(lightingShader);

		

		//Cargamos los 7 objetos requeridos para recrear el ambiente.
		reloj.Draw(lightingShader);
		mantelTetera.Draw(lightingShader);
		tazas.Draw(lightingShader);



		model = glm::mat4(1);
		escritorio.Draw(lightingShader); //Listo!
		escritorioTe.Draw(lightingShader);//Listo!
		candelabro.Draw(lightingShader); //Listo!
		escudo.Draw(lightingShader); //Listo!
		

		//Cargamos los libreros y sus variaciones:
		
		modelRoom = glm::translate(modelRoom, glm::vec3(-10.987f, 0.0f, -1.57f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRoom));
		librero.Draw(lightingShader);

		modelRoom = glm::translate(modelRoom, glm::vec3(0.0f, 0.0f, -1.98f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRoom));
		librero.Draw(lightingShader); //Segundo librero

		modelRoom = glm::translate(modelRoom, glm::vec3(10.0f, 0.5f, 10.83f));
		modelRoom = glm::rotate(modelRoom, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRoom));
		librero.Draw(lightingShader); //Tercer Librero

		modelRoom = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRoom));
		libreroAncho.Draw(lightingShader); //Primer librero ancho
		librerosVar.Draw(lightingShader);


		//Cargamos los modelos que se van a animar

		//Puerta derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.5993f, 1.6369f, -1.3f));
		model = glm::rotate(model, glm::radians(rotPuertaD), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertader.Draw(lightingShader);

		//Puerta izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(9.5993f, 1.6369f, 1.81f));
		model = glm::rotate(model, glm::radians(rotPuertaD), glm::vec3(0.0f, -1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puertaizq.Draw(lightingShader);

		//Pendulo del reloj
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-5.5997f, 6.8775f, 6.77353f));
		model = glm::rotate(model, glm::radians(2.1f * (float)sin(glfwGetTime() * PeriodoPendulo)), glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pendulo.Draw(lightingShader);

		//Tetera
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.94867f + TeteraX, 1.77696f + TeteraY, -3.71513f + TeteraZ ));
		
		model = glm::rotate(model, glm::radians(TrotX), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(TrotY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(TrotZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		teapot.Draw(lightingShader);

		//Reseteamos transformaciones.
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));



		


		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		//Desactiva el canal alfa 
		glBindVertexArray(0);




		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 14; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}


void Animacion() {

	//Animacion de Puerta Frontal
	if (PuertaA) 
	{
		rotPuertaD -= 0.1f;
		if (rotPuertaD < -85.0f) {
			PuertaA = false;
		}
		
	}

	if (PuertaC) 
	{
		
		rotPuertaD += 0.1f;
		if (rotPuertaD > 0.0f) {
			PuertaC = false;
		}

	}

	//Animacion de Reloj de Pendulo.
	if (Pendulo)
	{
		//Basandonos en la ecuación del periodo del pendulo.
					// 2 * PI * sqrt(Longitud * gravedad)
		PeriodoPendulo = 2 * 3.1415 * sqrt(15.0 / 9.81);

	}


	//iniciar animaciones por interpolaciones de la Tetera
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation(KeyFrame, playIndex);
			}
		}
		else
		{
			//Draw animation
			TeteraX += KeyFrame[playIndex].incX;
			TeteraY += KeyFrame[playIndex].incY;
			TeteraZ += KeyFrame[playIndex].incZ;

			TrotX += KeyFrame[playIndex].rotIncX;
			TrotY += KeyFrame[playIndex].rotIncY;
			TrotZ += KeyFrame[playIndex].rotIncZ;

			i_curr_steps++;
		}

	}
	

}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_LEFT_SHIFT])
	{
		camera.UpdateMovementSpeed(20.25f);
	}
	else
		camera.UpdateMovementSpeed(6.0f);
	
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		//pointLightPositions[0].x += 0.01f;
		SpotlightPos.x += 0.01;
	}
	

	if (keys[GLFW_KEY_Y])
	{
		//pointLightPositions[0].y += 0.01f;
		SpotlightPos.y += 0.01;
	}

	if (keys[GLFW_KEY_H])
	{
		//pointLightPositions[0].y -= 0.01f;
		SpotlightPos.y -= 0.01;
	}
	if (keys[GLFW_KEY_U])
	{
		//pointLightPositions[0].z -= 0.01f;
		SpotlightPos.z += 0.01;
	}
	if (keys[GLFW_KEY_J])
	{
		//pointLightPositions[0].z += 0.01f;
		SpotlightPos.z -= 0.01;
	}

	if (keys[GLFW_KEY_1]) {
		posX += 0.001;
	}

	if (keys[GLFW_KEY_2]) {
		posX -= 0.001;
	}

	if (keys[GLFW_KEY_3]) {
		posY += 0.001;
	}

	if (keys[GLFW_KEY_4]) {
		posY -= 0.001;
	}

	if (keys[GLFW_KEY_5]) {
		posZ += 0.001;
	}
	
	if (keys[GLFW_KEY_6]) {
		posZ -= 0.001;
	}

	if (keys[GLFW_KEY_V]) {
		rotX -= 0.05;
	}

	if (keys[GLFW_KEY_B]) {
		rotX += 0.05;
	}

	if (keys[GLFW_KEY_N]) {
		rotY -= 0.05;
	}

	if (keys[GLFW_KEY_M]) {
		rotY += 0.05;
	}

	if (keys[GLFW_KEY_COMMA]) {
		rotZ -= 0.05;
	}

	if (keys[GLFW_KEY_PERIOD]) {
		rotZ += 0.05;
	}

	

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.5f, 0.5f, 0.5f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

	//Correr
	if (keys[GLFW_KEY_P]) {
		//Debug functions
		std::cout << "X: " << camera.GetPosition().x << "Y: " << camera.GetPosition().y << "Z: " << camera.GetPosition().z << std::endl;

	}

	//Botones de animación
	if (keys[GLFW_KEY_G]) {

		PuertaA = true;
	}

	if (keys[GLFW_KEY_T]) {

		PuertaC = true;
	}

	if (keys[GLFW_KEY_H]) {

		Pendulo = true;
	}

	if (keys[GLFW_KEY_Y]) {

		Tetera = true;
	}

	//KEYFRAME CONTROLS
	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
			
		}

	}

	//Animacion Tetera
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();

			//First Interpolation				
			interpolation(KeyFrame, playIndex);

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}



	/*if (keys[GLFW_KEY_R])
	{
		clearKeyFrames();
	}*/

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos ;
	lastY = yPos ;

	camera.ProcessMouseMovement(xOffset, yOffset);
}