#include "PGNFileReader.h"
#include "PGNToFENParser.h"
#include "FENFileReader.h"
#include "StatusToPlaysTransformer.h"
#include "movements.h"
#include <Windows.h>
#include <cstdio>
#include "Model.h"
#include "Menu.h"
#include <chrono>
#include <thread>


using namespace glm;

void setOpenGLVersion();
void updateBoard(GLFWwindow* window, int& i, int size, bool& released, vector<vec3> controlPoints, bool autoPlay);
void drawPieces(GLFWwindow* window, Shader shaderProgram, Camera camera, Model board, Model pawn, Model bishop, Model tower, Model horse, Model queen, Model king, std::vector<std::vector<int>> arraygame, vector<vec3> controlPoints);
vector<vec3> draw_TFBSpline_Curve(vector<vec3> ctr_points, int nptsCorba, float pas);
vec3 Punt_Corba_BSpline(float t, vec3* ctr);

//piezas
#define PAWN 1
#define BISHOP 2
#define TOWER 3
#define HORSE 4
#define QUEEN 5
#define KING 6

//columnas tablero
#define A 1
#define B 2
#define C 3
#define D 4
#define E 5
#define F 6
#define G 7
#define H 8

vector<int> translationPiece;
vector<vec3> punts = { {0,0,0}, {0,0,0} };


#define WHITE true
#define BLACK false

//Constantes medida ventana
const unsigned int width = 1920; //1280
const unsigned int height = 1080; //720

int main() {

	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);
	
	vector<vector<vector<char>>> allBoardStatus;
	vector<vector<vector<int>>> allPlays;
	vector<vec3> controlPoints;

	Movements* reader1 = new Movements;
	FENFileReader* reader = new FENFileReader("Jugadas.fen");
	StatusToPlaysTransformer* transformer = new StatusToPlaysTransformer();
	
	allBoardStatus = reader->prepareBoard();
	allPlays = transformer->transformStatusToPlays(allBoardStatus);
	
	setOpenGLVersion();

	//creamos ventana
	GLFWwindow* window = glfwCreateWindow(width, height, "Chess Player", NULL, NULL);
	if (window == NULL) {
		std::cout << "Error al crear GLFW window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Llamada a GLAD para que configure OpenGL
	gladLoadGL();

	//Ajustar el viewport a la ventana
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	
	//Cargamos shaders
	Shader shaderProgram("default.vert", "default.frag");
	Shader simpleDepthShader("simpleDepthShader.vert", "simpleDepthShader.frag");
	
	//Creamos los modelos y la camara
	Camera camera(width, height, vec3(3.6341f, 22.8766f, 1.2473f), vec3(-0.1339f, -0.9960f, -0.0002f), vec3(0.0000f, 1.0000f, 0.0000f));
	Model board("models/board.obj");
	Model pawn("models/pawn.obj");
	Model bishop("models/bishop.obj");
	Model tower("models/tower.obj");
	Model horse("models/horse.obj");
	Model queen("models/queen.obj");
	Model king("models/king.obj");
	
	
	//Color y posición de la luz para shaders
	vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 lightPos = vec3(-2.0f, 4.0f, -1.0f);
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	
	float near_plane = 1.0f, far_plane = 150.0f;
	mat4 lightProjection = ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_plane, far_plane);
	mat4 lightView = lookAt(
		lightPos,
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));
	mat4 lightSpaceMatrix = lightProjection * lightView;

	//variables para cambio de jugada
	bool released = true;
	int currentPlay = 0;
	int translationIndex = 0;
	//Creamos la intefície de usuario
	//Menu menu(window);

	// SHADOW MAPPING
	// -----------------------
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	// creamos la depthtexture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// unimos el depth buffer con la textura
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// pasasmos las texturas (normal y shadowmap) al shader 
	shaderProgram.Activate();
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "diffuse0"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 1);


	double previousTime = glfwGetTime();
	controlPoints = reader1->compareBoards(allBoardStatus, currentPlay);

	bool autoPlay = true;

	while (!glfwWindowShouldClose(window)) {

		// Ajustamos color barrido
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Inicializamos el menú 
		//menu.init(camera);


		//Controlamos inputs para la camara
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// renderizado desde el punto de vista de la luz (para sombras)
		simpleDepthShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, value_ptr(lightSpaceMatrix));
		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		
		drawPieces(window, simpleDepthShader, camera, board, pawn, bishop, tower, horse, queen, king, allPlays[currentPlay], controlPoints);

		// Renderizado normal de la escena
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "depthMap"), 1);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightSpaceMatrix"), 1, GL_FALSE, value_ptr(lightSpaceMatrix));
		//Dibujamos tablero y piezas
			
			
		drawPieces(window, shaderProgram, camera, board, pawn, bishop, tower, horse, queen, king, allPlays[currentPlay], controlPoints);

			
		double currentTime = glfwGetTime();
		
		if ((currentTime - previousTime) >= 1) {
				
			translationIndex = translationIndex == punts.size() - 1 ? 0 : translationIndex + 1;

			previousTime = currentTime;
				
		}
			
		int pieceX = punts[translationIndex].x;
		int pieceY = punts[translationIndex].y;
		int pieceZ = punts[translationIndex].z;

		vec3 pieceZoom = vec3(0.2f, 0.2f, 0.2f);
		vec3 pieceRotation = vec3(1.f, 0.f, 0.f);

		pawn.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);

		// actualizacion e intercambio de buffers
		Sleep(1000);
		updateBoard(window, currentPlay, allPlays.size(), released, controlPoints, autoPlay);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Destruimo el menú
	//menu.shutDown();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
	
}


// Punt_Corba_Spline: Calcul del punt del spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre i i els punts de control ctr 
vec3 Punt_Corba_BSpline(float t, vec3* ctr)
{
	// Matriu dels Splines
	const double AS[4][4] =
	{
		{ -1.0 / 6.0, 0.5, -0.5, 1.0 / 6.0 },
		{ 0.5, -1.0, 0.0, 4.0 / 6.0 },
		{ -0.5, 0.5, 0.5, 1.0 / 6.0 },
		{ 1.0 / 6.0, 0.0, 0.0, 0.0 }
	};

	vec3 p = { 0, 0, 0 };
	float coef[4];
	int i, j;

	// Polinomis que multipliquen els punts de control del patch
	for (i = 0; i < 4; i++)
	{
		coef[i] = 0;
		for (j = 0; j < 4; j++)
			coef[i] = coef[i] * t + AS[i][j];
	}

	// Càlcul de la Posició
	for (i = 0; i < 4; i++)
	{
		p.x += coef[i] * ctr[i].x;
		p.y += coef[i] * ctr[i].y;
		p.z += coef[i] * ctr[i].z;
	}

	p.x *= 5;
	p.y *= 5;
	p.z *= 5;


	return p;

}

vector<vec3> draw_TFBSpline_Curve(vector<vec3> ctr_points, int nptsCorba, float pas)
{
	vec3 vertexL1, vertexL2;
	vec3 ctr[4];		// Punts control del patch de la B-Spline.
	int patch = 0;		// Patch actual.
	GLfloat t = 0;
	vector<vec3> puntsCorba;

	//t = t - pas;
// Càrrega primers punts de control.
	for (int i = 0; i < 4; i++)
	{
		float aux = ctr_points[i].y;
		ctr_points[i].y = ctr_points[i].z;
		ctr_points[i].z = -aux;

		ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;

	}



	// Càlcul i dibuix Triedre de Frenet en cada vèrtex de la corba B-Spline
	vertexL1 = Punt_Corba_BSpline(t, ctr);

	t = t + pas;
	while (patch <= nptsCorba - 4) {
		if (t >= 1.0)
		{
			vertexL2 = Punt_Corba_BSpline(1.0, ctr);
			puntsCorba.push_back(vertexL2);
			t = 0.0;
			patch++;
			if (patch <= nptsCorba - 4)
			{
				for (int i = 0; i < 4; i++)
				{
					ctr[i].x = ctr_points[patch + i].x;
					ctr[i].y = ctr_points[patch + i].y;
					ctr[i].z = ctr_points[patch + i].z;
				}
			}
		}
		else if (patch <= nptsCorba - 4) {
			vertexL2 = Punt_Corba_BSpline(t, ctr);
			puntsCorba.push_back(vertexL2);
			vertexL1 = vertexL2;
			t = t + pas;
		}
	}
	return puntsCorba;
}

void drawPieces(GLFWwindow* window, Shader shaderProgram, Camera camera, Model board, Model pawn, Model bishop, Model tower, Model horse, Model queen, Model king, std::vector<std::vector<int>> arraygame, vector<vec3> controlPoints) {

	const int originX = -9;
	const int originZ = 10;
	const int cellSize = 2;
	vec3 pieceZoom = vec3(0.2f, 0.2f, 0.2f);
	vec3 pieceRotation = vec3(1.f, 0.f, 0.f);

	//Dibujamos el tablero
	board.Draw(shaderProgram, camera, vec3(0.0f, -1.9f, 0.0f), 1.5708f, vec3(1.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));

	//Iteramos en el vector de posiciones de piezas
	for (auto piece : arraygame) {
		//aqui codigo para pasar del vector a las posiciones
		int pieceX = originX + piece[2] * cellSize;
		int pieceZ = originZ - piece[3] * cellSize;

		if (pieceX == controlPoints[3].x && pieceZ == controlPoints[3].z)
		{

			translationPiece = piece;
			continue;
		}
		switch (piece[0])
		{
		case PAWN:
			pawn.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case BISHOP:
			bishop.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case TOWER:
			tower.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case HORSE:
			horse.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 00.f, pieceRotation, pieceZoom);
			break;
		case QUEEN:
			queen.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case KING:
			king.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		default:
			break;
		}
	}



}

void updateBoard(GLFWwindow* window, int& i, int size, bool& released, vector<vec3> controlPoints, bool autoPlay) {

	if (autoPlay) {

		i++;

	}else {

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && i < size - 1 && released)
		{
			released = false;
			i++;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && i > 0 && released)
		{
			released = false;
			i--;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && !released && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
		{
			punts = draw_TFBSpline_Curve(controlPoints, 4, 0.005);
			released = true;
		}

	}

	
}

void setOpenGLVersion() {

	//Inicializar GLFW y Opengl version 3.3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

