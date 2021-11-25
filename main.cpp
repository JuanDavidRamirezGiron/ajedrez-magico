#include "PGNFileReader.h"
#include "PGNToFENParser.h"
#include "FENFileReader.h"
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
	glm::vec4 color = glm::vec4(0.8f, 0.3f, 0.02f, 1.0f);
	glUniform4fv(glGetUniformLocation(shaderProgram.ID, "color"), 1, glm::value_ptr(color));
	board.Draw(shaderProgram, camera, glm::vec3(0.0f, -1.9f, 0.0f), 1.5708f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));

	//Iteramos en el vector de posiciones de piezas
	for (auto piece : arraygame) {
		//aqui codigo para pasar del vector a las posiciones
		int pieceX = originX + piece[2] * cellSize;
		int pieceZ = originZ - piece[3] * cellSize;
		glm::vec3 pieceZoom = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::vec3 pieceRotation = glm::vec3(1.f, 0.f, 0.f);
		glm::vec4 color;

		(piece[1] == WHITE) ? color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) : color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		glUniform4fv(glGetUniformLocation(shaderProgram.ID, "color"), 1, glm::value_ptr(color));

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
		released = true;
	}
}



int main() {

	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	cout << "Empieza la lectura del programa" << endl;

	/*PGNFileReader* reader = new PGNFileReader("Jugadas.pgn");

	map<int, pair<string, string>> plays;

	plays = reader->gatherInformation();*/

	FENFileReader* reader = new FENFileReader("Jugadas.fen");

	vector<vector<vector<char>>> allBoardStatus;

	allBoardStatus = reader->prepareBoard();


	//int count = 0;
	vector<vector<vector<int>>> testVector;
	vector<vector<int>> outerVector;
	vector<int> auxVector;

	for (vector<vector<char>> boards : allBoardStatus)
	{

		outerVector.clear();
		auxVector.clear();

		for (size_t row = 0; row < boards.size(); row++)
		{

			for (size_t col = 0; col < boards[row].size(); col++)
			{
				char currentPiece = boards[row][col];

				if (!isdigit(currentPiece)) {
					//cout << "Count:" << count << endl;
					//cout << "Pieza es: " << boards[row][col] << endl;
					//count++;

					if (isupper(currentPiece)) {

						switch (currentPiece) {
						case 'R':
							auxVector = { TOWER, WHITE, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);

							break;
						case 'N':
							auxVector = { HORSE, WHITE, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						case 'B':
							auxVector = { BISHOP, WHITE, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						case 'Q':
							auxVector = { QUEEN, WHITE, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						case 'K':
							auxVector = { KING, WHITE, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						case 'P':
							auxVector = { PAWN, WHITE, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						}

					}
					else {

						switch (currentPiece) {
						case 'r':
							auxVector = { TOWER, BLACK, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						case 'n':
							auxVector = { HORSE, BLACK, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						case 'b':
							auxVector = { BISHOP, BLACK, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						case 'q':
							auxVector = { QUEEN, BLACK, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						case 'k':
							auxVector = { KING, BLACK, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						case 'p':
							auxVector = { PAWN, BLACK, (int)row + 1, (int)col + 1 };
							outerVector.push_back(auxVector);
							break;
						}

					}

				}

			}

		}

		testVector.push_back(outerVector);

	}

	//reader->printAllBoardStatus(allBoardStatus);

	//exit(0);

	/*PGNToFENParser* parser = new PGNToFENParser();
	parser->initializeInternalChessBoard();
	parser->parsePGNToFEN(plays);
	exit(0);*/

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
	};

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
	glEnable(GL_DEPTH_TEST);
	//Importamos las texturas
	Texture textures[]
	{
		//Texture("white_wood.png", "diffuse", 0),
		//Texture("black_wood.png", "diffuse", 0),
		Texture("models/textures/board.png", "diffuse", 0),
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
	bool released = true;
	int i = 0;
	while (!glfwWindowShouldClose(window)) {

		// Ajustamos color barrido
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Controlamos inputs para la camara
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		//Dibujamos tablero y piezas
		drawPieces(shaderProgram, camera, board, pawn, bishop, tower, horse, queen, king, testVector[i]);
		updateBoard(window, i, testVector.size(), released);
		//Intercambio de buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}