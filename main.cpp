
#include <Windows.h>
#include <cstdio>
#include"Model.h"
#define PAWN 1
#define BISHOP 2
#define TOWER 3
#define HORSE 4
#define QUEEN 5
#define KING 6
#define A 1
#define B 2
#define C 3
#define D 4
#define E 5
#define F 6
#define G 7
#define H 8
#define WHITE true
#define BLACK false

//Constantes medida ventana
const unsigned int width = 1280;
const unsigned int height = 720;


void drawPieces(Shader shaderProgram, Camera camera, Model board, Model pawn, Model bishop, Model tower, Model horse, Model queen, Model king, std::vector<std::vector<int>> arraygame) {

	const int originX = -9;
	const int originZ = 10;
	const int cellSize = 2;

	//Dibujamos el tablero
	board.Draw(shaderProgram, camera, glm::vec3(0.0f, -1.9f, 0.0f), 1.5708f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	
	//Iteramos en el vector de posiciones de piezas
	for (auto piece : arraygame) {
		//aqui codigo para pasar del vector a las posiciones
		int pieceX = originX + piece[2] * cellSize;
		int pieceZ = originZ - piece[3] * cellSize;
		glm::vec3 pieceZoom = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::vec3 pieceRotation = glm::vec3(1.f, 0.f, 0.f);


		switch (piece[0])
		{
		case PAWN:
			pawn.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case BISHOP:
			bishop.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case TOWER:
			tower.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case HORSE:
			horse.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 00.f, pieceRotation, pieceZoom);
			break;
		case QUEEN:
			queen.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case KING:
			king.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		default:
			break;
		}
	}
	
}


void updateBoard(GLFWwindow* window, int& i, int size, bool& released) {
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && i<size-1 && released)
	{
		released = false;
		i++;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && i>0 && released)
	{
		released = false;
		i--;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && !released && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
	{
		released = true;
	}
}


int main() {
	std::vector<std::vector<std::vector<int>>> arraygame
	{
		{
			{TOWER, WHITE, A, 1},{HORSE, WHITE, B, 1},{BISHOP, WHITE, C, 1},{QUEEN, WHITE, D, 1},{KING, WHITE, E, 1},{BISHOP, WHITE, F, 1},{HORSE, WHITE, G, 1},{TOWER, WHITE, H, 1},
			{PAWN, WHITE, A, 2},{PAWN, WHITE, B, 2},{PAWN, WHITE, C, 2},{PAWN, WHITE, D, 2},{PAWN, WHITE, E, 2},{PAWN, WHITE, F, 2},{PAWN, WHITE, G, 2},{PAWN, WHITE, H, 2},
			{TOWER, BLACK, A, 8},{HORSE, BLACK, B, 8},{BISHOP, BLACK, C, 8},{KING, BLACK, D, 8},{QUEEN, BLACK, E, 8},{BISHOP, BLACK, F, 8},{HORSE, BLACK, G, 8},{TOWER, BLACK, H, 8},
			{PAWN, BLACK, A, 7},{PAWN, BLACK, B, 7},{PAWN, BLACK, C, 7},{PAWN, BLACK, D, 7},{PAWN, BLACK, E, 7},{PAWN, BLACK, F, 7},{PAWN, BLACK, G, 7},{PAWN, BLACK, H, 7}
		},
		{
			{TOWER, WHITE, A, 1},{HORSE, WHITE, B, 1},{BISHOP, WHITE, C, 1},{QUEEN, WHITE, D, 1},{KING, WHITE, E, 1},{BISHOP, WHITE, F, 1},{HORSE, WHITE, G, 1},{TOWER, WHITE, H, 1},
			{PAWN, WHITE, A, 2},{PAWN, WHITE, B, 2},{PAWN, WHITE, C, 3},{PAWN, WHITE, D, 2},{PAWN, WHITE, E, 2},{PAWN, WHITE, F, 2},{PAWN, WHITE, G, 2},{PAWN, WHITE, H, 2},
			{TOWER, BLACK, A, 8},{HORSE, BLACK, B, 8},{BISHOP, BLACK, C, 8},{KING, BLACK, D, 8},{QUEEN, BLACK, E, 8},{BISHOP, BLACK, F, 8},{HORSE, BLACK, G, 8},{TOWER, BLACK, H, 8},
			{PAWN, BLACK, A, 7},{PAWN, BLACK, B, 7},{PAWN, BLACK, C, 7},{PAWN, BLACK, D, 7},{PAWN, BLACK, E, 7},{PAWN, BLACK, F, 7},{PAWN, BLACK, G, 7},{PAWN, BLACK, H, 7}
		},
		{
			{TOWER, WHITE, A, 1},{HORSE, WHITE, B, 1},{BISHOP, WHITE, C, 1},{QUEEN, WHITE, D, 1},{KING, WHITE, E, 1},{BISHOP, WHITE, F, 1},{HORSE, WHITE, G, 1},{TOWER, WHITE, H, 1},
			{PAWN, WHITE, A, 2},{PAWN, WHITE, B, 2},{PAWN, WHITE, C, 3},{PAWN, WHITE, D, 2},{PAWN, WHITE, E, 2},{PAWN, WHITE, F, 2},{PAWN, WHITE, G, 2},{PAWN, WHITE, H, 2},
			{TOWER, BLACK, A, 8},{HORSE, BLACK, B, 8},{BISHOP, BLACK, C, 8},{KING, BLACK, D, 8},{QUEEN, BLACK, E, 8},{BISHOP, BLACK, F, 8},{HORSE, BLACK, G, 8},{TOWER, BLACK, H, 8},
			{PAWN, BLACK, A, 7},{PAWN, BLACK, B, 7},{PAWN, BLACK, C, 7},{PAWN, BLACK, D, 7},{PAWN, BLACK, E, 6},{PAWN, BLACK, F, 7},{PAWN, BLACK, G, 7},{PAWN, BLACK, H, 7}
		},
		{
			{TOWER, WHITE, A, 1},{HORSE, WHITE, B, 1},{BISHOP, WHITE, C, 1},{QUEEN, WHITE, D, 1},{KING, WHITE, E, 1},{BISHOP, WHITE, F, 1},{HORSE, WHITE, G, 1},{TOWER, WHITE, H, 1},
			{PAWN, WHITE, A, 2},{PAWN, WHITE, B, 4},{PAWN, WHITE, C, 3},{PAWN, WHITE, D, 2},{PAWN, WHITE, E, 2},{PAWN, WHITE, F, 2},{PAWN, WHITE, G, 2},{PAWN, WHITE, H, 2},
			{TOWER, BLACK, A, 8},{HORSE, BLACK, B, 8},{BISHOP, BLACK, C, 8},{KING, BLACK, D, 8},{QUEEN, BLACK, E, 8},{BISHOP, BLACK, F, 8},{HORSE, BLACK, G, 8},{TOWER, BLACK, H, 8},
			{PAWN, BLACK, A, 7},{PAWN, BLACK, B, 7},{PAWN, BLACK, C, 7},{PAWN, BLACK, D, 7},{PAWN, BLACK, E, 6},{PAWN, BLACK, F, 7},{PAWN, BLACK, G, 7},{PAWN, BLACK, H, 7}
		},	
		{
			{TOWER, WHITE, A, 1},{HORSE, WHITE, B, 1},{BISHOP, WHITE, C, 1},{QUEEN, WHITE, D, 1},{KING, WHITE, E, 1},{BISHOP, WHITE, F, 1},{HORSE, WHITE, G, 1},{TOWER, WHITE, H, 1},
			{PAWN, WHITE, A, 2},{PAWN, WHITE, B, 4},{PAWN, WHITE, C, 3},{PAWN, WHITE, D, 2},{PAWN, WHITE, E, 2},{PAWN, WHITE, F, 2},{PAWN, WHITE, G, 2},{PAWN, WHITE, H, 2},
			{TOWER, BLACK, A, 8},{HORSE, BLACK, B, 8},{BISHOP, BLACK, C, 8},{KING, BLACK, D, 8},{QUEEN, BLACK, E, 8},{BISHOP, BLACK, F, 8},{HORSE, BLACK, G, 8},{TOWER, BLACK, H, 8},
			{PAWN, BLACK, A, 7},{PAWN, BLACK, B, 7},{PAWN, BLACK, C, 7},{PAWN, BLACK, D, 5},{PAWN, BLACK, E, 6},{PAWN, BLACK, F, 7},{PAWN, BLACK, G, 7},{PAWN, BLACK, H, 7}
		},
	};

	//Inicializar GLFW y Opengl version 3.3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creamos ventana
	GLFWwindow* window = glfwCreateWindow(width, height, "Chess Reproducer", NULL, NULL);
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
	//Creamos los modelos y la cámara
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	Model board("models/board.obj");
	Model pawn("models/pawn.obj");
	Model bishop("models/bishop.obj");
	Model tower("models/tower.obj");
	Model horse("models/horse.obj");
	Model queen("models/queen.obj");
	Model king("models/king.obj");
	//Model lightsource("models/source.obj");
	bool released = true;
	int i = 0;
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, -10.0f, 0.f);
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	while (!glfwWindowShouldClose(window)) {

		// Ajustamos color barrido
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Controlamos inputs para la camara
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		//Dibujamos tablero y piezas
		drawPieces(shaderProgram, camera, board, pawn, bishop, tower, horse, queen, king, arraygame[i]);
		updateBoard(window, i, arraygame.size(), released);

		//Intercambio de buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

