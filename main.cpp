#include "PGNFileReader.h"
#include "PGNToFENParser.h"
#include "FENFileReader.h"
#include "movements.h"
#include <Windows.h>
#include <cstdio>
#include"Model.h"

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


#define WHITE true
#define BLACK false

//Constantes medida ventana
const unsigned int width = 1920; //1280
const unsigned int height = 1080; //720

// Punt_Corba_Spline: Calcul del punt del spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre i i els punts de control ctr 
glm::vec3 Punt_Corba_BSpline(float t, glm::vec3* ctr)
{
	// Matriu dels Splines
	const double AS[4][4] =
	{
		{ -1.0 / 6.0, 0.5, -0.5, 1.0 / 6.0 },
		{ 0.5, -1.0, 0.0, 4.0 / 6.0 },
		{ -0.5, 0.5, 0.5, 1.0 / 6.0 },
		{ 1.0 / 6.0, 0.0, 0.0, 0.0 }
	};

	glm::vec3 p = { 0, 0, 0 };
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
	return p;

}

vector<glm::vec3> draw_TFBSpline_Curve(vector<glm::vec3> ctr_points, int nptsCorba, float pas)
{
	glm::vec3 vertexL1, vertexL2;
	glm::vec3 ctr[4];		// Punts control del patch de la B-Spline.
	int patch = 0;		// Patch actual.
	GLfloat t = 0;
	vector<glm::vec3> puntsCorba;

	//t = t - pas;
// Càrrega primers punts de control.
	for (int i = 0; i < 4; i++)
	{
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
	Movements* reader1 = new Movements;

	vector<vector<vector<char>>> allBoardStatus;

	allBoardStatus = reader->prepareBoard();


	int count = 0;
	vector<vector<vector<int>>> testVector;
	vector<vector<int>> outerVector;
	vector<int> auxVector;
	vector<glm::vec3> controlPoints;
	vector<glm::vec3> punts;

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

	

	//reader->printTransformedBoard(allBoardStatus[0]);

	//reader->printAllBoardStatus(allBoardStatus);
	//reader1->RawBoard(allBoardStatus[0][0]);
	//reader1->RawBoard(allBoardStatus[1][0]);
	
	//exit(0);

	/*PGNToFENParser* parser = new PGNToFENParser();
	parser->initializeInternalChessBoard();
	parser->parsePGNToFEN(plays);
	exit(0);*/

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
	Shader simpleDepthShader("simpleDepthShader.vert", "simpleDepthShader.frag");
	
	//Creamos los modelos y la camara
	Camera camera(width, height, glm::vec3(3.6341f, 22.8766f, 1.2473f), glm::vec3(-0.1339f, -0.9960f, -0.0002f), glm::vec3(0.0000f, 1.0000f, 0.0000f));
	Model board("models/board.obj");
	Model pawn("models/pawn.obj");
	Model bishop("models/bishop.obj");
	Model tower("models/tower.obj");
	Model horse("models/horse.obj");
	Model queen("models/queen.obj");
	Model king("models/king.obj");
	
	
	//Color y posición de la luz para shaders
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	
	float near_plane = 1.0f, far_plane = 150.0f;
	glm::mat4 lightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(
		lightPos,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	//variables para cambio de jugada
	bool released = true;
	int i = 0;

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

	


	while (!glfwWindowShouldClose(window)) {
		
		// Ajustamos color barrido
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// renderizado desde el punto de vista de la luz (para sombras)
		simpleDepthShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		drawPieces(simpleDepthShader, camera, board, pawn, bishop, tower, horse, queen, king, testVector[i]);

		// Renderizado normal de la escena
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "depthMap"), 1);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		//Dibujamos tablero y piezas
		controlPoints = reader1->compareBoards(allBoardStatus, i);
		punts = draw_TFBSpline_Curve(controlPoints, 4, 0.05);
		drawPieces(shaderProgram, camera, board, pawn, bishop, tower, horse, queen, king, testVector[i]);

		// actualizacion e intercambio de buffers
		updateBoard(window, i, testVector.size(), released);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

