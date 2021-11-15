
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


//Constantes medida ventana
const unsigned int width = 1280;
const unsigned int height = 720;



void drawPieces(Model board, Model pawn, Model bishop, Model tower, Model horse, Model queen, Model king, std::vector<std::vector<std::vector<int>>> arraygame) {
	for (auto arraygameboard : arraygame) {
		for (auto piece : arraygameboard) {
			switch (piece[0])
			{
			case PAWN:
				break;
			case BISHOP:
				break;
			case TOWER:
				break;
			case HORSE:
				break;
			case QUEEN:
				break;
			case KING:
				break;
			default:
				break;
			}
		}
	}
}

int main() {
	std::vector<std::vector<std::vector<int>>> arraygame
	{
	{
		{TOWER, A, 1},{HORSE, B, 1},{BISHOP, C, 1},{QUEEN, D, 1},{KING, E, 1},{BISHOP, F, 1},{HORSE, G, 1},{TOWER, H, 1},
		{PAWN, A, 2},{PAWN, B, 2},{PAWN, C, 2},{PAWN, D, 2},{PAWN, E, 2},{PAWN, F, 2},{PAWN, G, 2},{PAWN, H, 2},
		{TOWER, A, 8},{HORSE, B, 8},{BISHOP, C, 8},{QUEEN, D, 8},{KING, E, 8},{BISHOP, F, 8},{HORSE, G, 8},{TOWER, H, 8},
		{PAWN, A, 7},{PAWN, B, 7},{PAWN, C, 7},{PAWN, D, 7},{PAWN, E, 7},{PAWN, F, 7},{PAWN, G, 7},{PAWN, H, 7}
	},
	{
		{TOWER, A, 1},{HORSE, B, 1},{BISHOP, C, 1},{QUEEN, D, 1},{KING, E, 1},{BISHOP, F, 1},{HORSE, G, 1},{TOWER, H, 1},
		{PAWN, A, 2},{PAWN, B, 2},{PAWN, C, 3}, {PAWN, D, 2},{PAWN, E, 2},{PAWN, F, 2},{PAWN, G, 2},{PAWN, H, 2},
		{TOWER, A, 8},{HORSE, B, 8},{BISHOP, C, 8},{QUEEN, D, 8},{KING, E, 8},{BISHOP, F, 8},{HORSE, G, 8},{TOWER, H, 8},
		{PAWN, A, 7},{PAWN, B, 7},{PAWN, C, 7},{PAWN, D, 7},{PAWN, E, 7},{PAWN, F, 7},{PAWN, G, 7},{PAWN, H, 7}
	} };

	//Inicializar GLFW
	glfwInit();
	//Opengl version 3.3
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

	//Importamos las texturas
	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0),
	};
	Shader shaderProgram("default.vert", "default.frag");

	//Creamos los modelos y la cámara
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	Model board("models/board_2.obj");
	Model pawn("models/pawn.obj");
	Model bishop("models/bishop.obj");
	Model tower("models/tower.obj");
	Model horse("models/horse.obj");
	Model queen("models/queen.obj");
	Model king("models/king.obj");


	while (!glfwWindowShouldClose(window)) {

		// Ajustamos color barrido
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Controlamos inputs para la camara
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		//Dibujamos tablero y piezas
		//Draw(shader, camara, vector translacion, angulo rotacion en radianes, vector ejes rotacion, vector escalado)
		//board.Draw(shaderProgram, camera, glm::vec3(0.0f, -4.6f, 0.0f), 1.5708f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
		//king.Draw(shaderProgram, camera, glm::vec3(0.0f, 0.0f, 0.0f), 0.f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f));
		drawPieces(board, pawn, bishop, tower, horse, queen, king, arraygame);

		//Intercambio de buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

