#include "FileReader.h"
#include "PGNToFENParser.h"
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

//constantes medida ventana
const unsigned int width = 1280;
const unsigned int height = 720;

//Testing after cleaning gitignore

void drawPieces(Shader shaderProgram, Camera camera, Model board, Model pawn, Model bishop, Model tower, Model horse, Model queen, Model king, std::vector<std::vector<int>> arraygame) {

	const int originX = -9;
	const int originZ = 10;
	const int cellSize = 2;

	//Dibujamos el tablero
	glm::vec4 color = glm::vec4(0.8f, 0.3f, 0.02f, 1.0f);
	glUniform4fv(glGetUniformLocation(shaderProgram.ID, "color"), 1, glm::value_ptr(color));
	board.Draw(shaderProgram, camera, glm::vec3(0.0f, -3.2f, 0.0f), 1.5708f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));

	//Iteramos en el vector de posiciones de piezas
	for (auto piece : arraygame) {
		//aqui codigo para pasar del vector a las posiciones
		int pieceX = originX + piece[1] * cellSize;
		int pieceZ = originZ - piece[2] * cellSize;
		glm::vec3 pieceZoom = glm::vec3(0.3f, 0.3f, 0.3f);
		//if pieza blanca??
		glm::vec3 pieceRotation = glm::vec3(1.f, 0.f, 0.f);
		glm::vec4 color;



		switch (piece[0])
		{
		case PAWN:
			color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4fv(glGetUniformLocation(shaderProgram.ID, "color"), 1, glm::value_ptr(color));
			pawn.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case BISHOP:
			color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
			glUniform4fv(glGetUniformLocation(shaderProgram.ID, "color"), 1, glm::value_ptr(color));
			bishop.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case TOWER:
			color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			glUniform4fv(glGetUniformLocation(shaderProgram.ID, "color"), 1, glm::value_ptr(color));
			tower.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case HORSE:
			color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
			glUniform4fv(glGetUniformLocation(shaderProgram.ID, "color"), 1, glm::value_ptr(color));
			horse.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 00.f, pieceRotation, pieceZoom);
			break;
		case QUEEN:
			color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
			glUniform4fv(glGetUniformLocation(shaderProgram.ID, "color"), 1, glm::value_ptr(color));
			queen.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case KING:
			color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			glUniform4fv(glGetUniformLocation(shaderProgram.ID, "color"), 1, glm::value_ptr(color));
			king.Draw(shaderProgram, camera, glm::vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		default:
			break;
		}
	}

}


int main() {

	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	cout << "Empieza la lectura del programa" << endl;

	FileReader* reader = new FileReader("Jugadas.pgn");

	map<int, pair<string, string>> plays;

	plays = reader->gatherInformation();

	PGNToFENParser* parser = new PGNToFENParser();
	parser->initializeInternalChessBoard();
	parser->parsePGNToFEN(plays);

	exit(0);

	std::vector<std::vector<std::vector<int>>> arraygame
	{
		{
			{TOWER, A, 1},{HORSE, B, 1},{BISHOP, C, 1},{QUEEN, D, 1},{KING, E, 1},{BISHOP, F, 1},{HORSE, G, 1},{TOWER, H, 1},
			{PAWN, A, 2},{PAWN, B, 2},{PAWN, C, 2},{PAWN, D, 2},{PAWN, E, 2},{PAWN, F, 2},{PAWN, G, 2},{PAWN, H, 2},
			{TOWER, A, 8},{HORSE, B, 8},{BISHOP, C, 8},{KING, D, 8},{QUEEN, E, 8},{BISHOP, F, 8},{HORSE, G, 8},{TOWER, H, 8},
			{PAWN, A, 7},{PAWN, B, 7},{PAWN, C, 7},{PAWN, D, 7},{PAWN, E, 7},{PAWN, F, 7},{PAWN, G, 7},{PAWN, H, 7}
		},
		{
			{TOWER, A, 1},{HORSE, B, 1},{BISHOP, C, 1},{QUEEN, D, 1},{KING, E, 1},{BISHOP, F, 1},{HORSE, G, 1},{TOWER, H, 1},
			{PAWN, A, 2},{PAWN, B, 2},{PAWN, C, 3}, {PAWN, D, 2},{PAWN, E, 2},{PAWN, F, 2},{PAWN, G, 2},{PAWN, H, 2},
			{TOWER, A, 8},{HORSE, B, 8},{BISHOP, C, 8},{KING, D, 8},{QUEEN, E, 8},{BISHOP, F, 8},{HORSE, G, 8},{TOWER, H, 8},
			{PAWN, A, 7},{PAWN, B, 7},{PAWN, C, 7},{PAWN, D, 7},{PAWN, E, 7},{PAWN, F, 7},{PAWN, G, 7},{PAWN, H, 7}
		},
		{
			{TOWER, A, 1},{HORSE, B, 1},{BISHOP, C, 1},{QUEEN, D, 1},{KING, E, 1},{BISHOP, F, 1},{HORSE, G, 1},{TOWER, H, 1},
			{PAWN, A, 2},{PAWN, B, 2},{PAWN, C, 3}, {PAWN, D, 2},{PAWN, E, 2},{PAWN, F, 2},{PAWN, G, 2},{PAWN, H, 2},
			{TOWER, A, 8},{HORSE, B, 8},{BISHOP, C, 8},{KING, D, 8},{QUEEN, E, 8},{BISHOP, F, 8},{HORSE, G, 8},{TOWER, H, 8},
			{PAWN, A, 7},{PAWN, B, 7},{PAWN, C, 7},{PAWN, D, 7},{PAWN, E, 7},{PAWN, F, 7},{PAWN, G, 7},{PAWN, H, 7}
		}



	};

	//inicializar GLFW
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

	//llamada a GLAD para que configure OpenGL
	gladLoadGL();

	//ajustar el viewport a la ventana
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0),
	};
	Shader shaderProgram("default.vert", "default.frag");

	//Creamos los modelos y la c�mara
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
		drawPieces(shaderProgram, camera, board, pawn, bishop, tower, horse, queen, king, arraygame[1]);

		//Intercambio de buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}