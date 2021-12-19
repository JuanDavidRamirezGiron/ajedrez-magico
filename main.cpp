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
#include "Skybox.h"


using namespace glm;

void setOpenGLVersion();
void updateBoard(GLFWwindow* window, int& i, int size, bool& released, vector<vec3> controlPoints, glm::vec3 og_lightPos, glm::vec3& lightPos, float& angle, bool& releasedAuto);
void drawPieces(GLFWwindow* window, Shader shaderProgram, Camera camera, Model board, Model pawn_b, Model bishop_b, Model tower_b, Model horse_b, Model queen_b, Model king_b, Model pawn_w, Model bishop_w, Model tower_w, Model horse_w, Model queen_w, Model king_w, std::vector<std::vector<int>> arraygame, vector<vec3> controlPoints);

vector<vec3> draw_Bezier_Curve_VAO(vector<vec3> ctr_points, int nptsCorba, float pas, bool tancat);
vec3 Punt_Bezier_Curve(float t, vec3* ctr);
void incrementPlay();


static std::atomic<bool> s_autoPlay = false;
static int s_globalI = 0;
static thread worker;// = thread(incrementPlay);
static vector<vec3> controlPoints;


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
int translationIndex = 0;
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
	

	Movements* reader1 = new Movements;
	FENFileReader* reader = new FENFileReader("Jugadas.fen");
	StatusToPlaysTransformer* transformer = new StatusToPlaysTransformer();
	
	allBoardStatus = reader->prepareBoard();
	allPlays = transformer->transformStatusToPlays(allBoardStatus);

	//reader->printAllBoardStatus(allBoardStatus);
	
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
	Shader skyboxShader("skyboxShader.vert", "skyboxShader.frag");
	
	//Creamos los modelos y la camara
	Skybox skybox(skyboxShader);
	Camera camera(width, height, vec3(3.6341f, 22.8766f, 1.2473f), vec3(-0.1339f, -0.9960f, -0.0002f), vec3(0.0000f, 1.0000f, 0.0000f));
	Model board("models/board.obj");
	

	Model pawn_b("models/pawn_b.obj");
	Model bishop_b("models/bishop_b.obj");
	Model tower_b("models/tower_b.obj");
	Model horse_b("models/horse_b.obj");
	Model queen_b("models/queen_b.obj");
	Model king_b("models/king_b.obj");
	Model pawn_w("models/pawn_w.obj");
	Model bishop_w("models/bishop_w.obj");
	Model tower_w("models/tower_w.obj");
	Model horse_w("models/horse_w.obj");
	Model queen_w("models/queen_w.obj");
	Model king_w("models/king_w.obj");
	
	
	//Color y posición de la luz para shaders
	//TODO Mirar sombra casilla [0]-[1]
	float near_plane = 1.0f, far_plane = 20.0f;
	vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 og_lightPos =vec3(-5.0f, 5.0f, 0.0f);
	vec3 lightPos = og_lightPos;
	mat4 lightProjection = ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_plane, far_plane);
	
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	
	//variables para cambio de jugada
	bool released = true;
	bool releasedAuto = true;
	int currentPlay = 0;
	float angle = 180;

	//Creamos la intefície de usuario
	Menu menu(window);

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	
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

	shaderProgram.Activate();
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "diffuse0"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 1);

	skyboxShader.Activate();
	camera.updateMatrix(45.0f, 0.1f, 100.0f);
	glm::mat4 projection = camera.getProjection();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
	//Mover piezas
	double previousTime = glfwGetTime();
	//glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	vec3 pieceZoom = vec3(0.2f, 0.2f, 0.2f);
	vec3 pieceRotation = vec3(1.f, 0.f, 0.f);

	

	while (!glfwWindowShouldClose(window)) {

		// Ajustamos color barrido
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Controlamos inputs para la camara
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		controlPoints = reader1->compareBoards(allBoardStatus, s_globalI);
		double currentTime = glfwGetTime();

		if ((currentTime - previousTime) >= 1/30) {
			translationIndex = translationIndex == punts.size() - 1 ? translationIndex : translationIndex + 1;
			previousTime = currentTime;
		}

		mat4 lightView = lookAt(lightPos,vec3(0.0f, 0.0f, 0.0f),vec3(0.0f, 1.0f, 0.0f));
		mat4 lightSpaceMatrix = lightProjection * lightView;
		simpleDepthShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, value_ptr(lightSpaceMatrix));
		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		drawPieces(window, simpleDepthShader, camera, board, pawn_b, bishop_b, tower_b, horse_b, queen_b, king_b, pawn_w, bishop_w, 
			tower_w, horse_w, queen_w, king_w, allPlays[s_globalI], controlPoints);
		

		// Renderizado normal de la escena
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "depthMap"), 1);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightSpaceMatrix"), 1, GL_FALSE, value_ptr(lightSpaceMatrix));
		
		drawPieces(window, shaderProgram, camera, board, pawn_b, bishop_b, tower_b, horse_b, queen_b, king_b, 
			pawn_w, bishop_w, tower_w, horse_w, queen_w, king_w, allPlays[s_globalI], controlPoints);


		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(glm::mat3(camera.getView()));

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		skybox.draw();

		// actualizacion e intercambio de buffers
		//Inicializamos el menú 
		menu.init(camera);
		//Sleep(1000);
		updateBoard(window, currentPlay, allPlays.size(), released, controlPoints, og_lightPos, lightPos, angle, releasedAuto);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Destruimo el menú
	//menu.shutDown();

	glfwDestroyWindow(window);
	glfwTerminate();
	if(worker.joinable())
		worker.detach();
	return 0;
	
}



vec3 Punt_Bezier_Curve(float t, vec3* ctr)
{

	const double AB[4][4] =
	{
		{ -1.0, 3.0, -3.0, 1.0 },
		{ 3.0, -6.0, 3.0, 0.0 },
		{ -3.0, 3.0, 0.0, 0.0 },
		{ 1.0, 0.0, 0.0, 0.0 }
	};

	vec3 p = { 0, 0, 0 };
	float coef[4];
	int i, j;

	// Polinomis que multipliquen els punts de control del patch
	for (i = 0; i < 4; i++)
	{
		coef[i] = 0;
		for (j = 0; j < 4; j++)
			coef[i] = coef[i] * t + AB[i][j];
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


vector<vec3> draw_Bezier_Curve_VAO(vector<vec3> ctr_points, int nptsCorba, float pas, bool tancat)
{
	GLuint vaoId = 0;
	std::vector <double> vertices, colors;		
	vertices.resize(0);		colors.resize(0);	

	vec3 vertexL1, vertexL2;
	vec3 ctr[4];		
	int patch = 0;		
	GLfloat t = 0;

	for (int i = 0; i < 4; i++)
	{
		ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;
	}
	
	vector<vec3> puntsCorba;
	vertexL1 = Punt_Bezier_Curve(t, ctr);
	puntsCorba.push_back(vertexL1);

	t = t + pas;
	while (patch <= nptsCorba - 4) {
		if (t > 1.0 + 2 * pas) {
			t -= 1.0;
			patch = patch + 3;
			if ((patch < nptsCorba - 1) && (patch + 4 > nptsCorba)) patch = nptsCorba - 4;
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
		if (patch <= nptsCorba - 4) {
			if (t > 1.0) vertexL2 = Punt_Bezier_Curve(1.0, ctr);
			else vertexL2 = Punt_Bezier_Curve(t, ctr);
			puntsCorba.push_back(vertexL2);
		
			vertexL1 = vertexL2;
			t = t + pas;
		}
	}

	return puntsCorba;
}

void drawPieces(GLFWwindow* window, Shader shaderProgram, Camera camera, Model board, Model pawn_b, Model bishop_b, Model tower_b, Model horse_b, Model queen_b, Model king_b, Model pawn_w, Model bishop_w, Model tower_w, Model horse_w, Model queen_w, Model king_w, std::vector<std::vector<int>> arraygame, vector<vec3> controlPoints) {

	const int originX = -9;
	const int originZ = 10;
	const int cellSize = 2;
	vec3 pieceZoom = vec3(0.2f, 0.2f, 0.2f);
	vec3 pieceRotation = vec3(1.f, 0.f, 0.f);

	//Dibujamos el tablero
	board.Draw(shaderProgram, camera, vec3(0.0f, -2.7f, 0.0f), 1.5708f, vec3(1.f, 0.f, 0.f), vec3(1.f, 1.f, 1.f));

	//Iteramos en el vector de posiciones de piezas
	for (auto piece : arraygame) {
		//aqui codigo para pasar del vector a las posiciones
		float pieceX = originX + piece[2] * cellSize;
		float pieceZ = originZ - piece[3] * cellSize;

		if (pieceX == controlPoints[0].x && pieceZ == controlPoints[0].z)
		{
			translationPiece = piece;
			continue;
		}
		switch (piece[0])
		{
		case PAWN:
			if (piece[1])
				pawn_b.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			else
				pawn_w.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case BISHOP:
			if (piece[1])
				bishop_b.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			else
				bishop_w.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case TOWER:
			if (piece[1])
				tower_b.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			else
				tower_w.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case HORSE:
			if (piece[1])
				horse_b.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 00.f, pieceRotation, pieceZoom);
			else
				horse_w.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case QUEEN:
			if (piece[1])
				queen_b.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			else
				queen_w.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		case KING:
			if (piece[1])
				king_b.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			else
				king_w.Draw(shaderProgram, camera, vec3(pieceX, 0.0f, pieceZ), 0.0f, pieceRotation, pieceZoom);
			break;
		default:
			break;
		}
	}

	vector<vec3> initial = { {0,0,0}, {0,0,0} };

	float pieceX = punts[translationIndex].x;
	float pieceY = punts[translationIndex].y;
	float pieceZ = punts[translationIndex].z;
	
	if (punts == initial) {
		
		pieceX = originX + translationPiece[2] * cellSize;
		pieceY = 0;
		pieceZ = originZ - translationPiece[3] * cellSize;
	}
	


	switch (translationPiece[0])
	{
	case PAWN:
		if (translationPiece[1])
			pawn_b.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		else
			pawn_w.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		break;
	case BISHOP:
		if (translationPiece[1])
			bishop_b.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		else
			bishop_w.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		break;
	case TOWER:
		if (translationPiece[1])
			tower_b.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		else
			tower_w.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		break;
	case HORSE:
		if (translationPiece[1])
			horse_b.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 00.f, pieceRotation, pieceZoom);
		else
			horse_w.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		break;
	case QUEEN:
		if (translationPiece[1])
			queen_b.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		else
			queen_w.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		break;
	case KING:
		if (translationPiece[1])
			king_b.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		else
			king_w.Draw(shaderProgram, camera, vec3(pieceX, pieceY, pieceZ), 0.0f, pieceRotation, pieceZoom);
		break;
	default:
		break;
	}
}

void incrementPlay() {

	while (s_autoPlay.load()) {

		s_globalI++;
		punts = draw_Bezier_Curve_VAO(controlPoints, 4, 0.05, false);
		translationIndex = 0;
		this_thread::sleep_for(1s);

	}

}


void updateBoard(GLFWwindow* window, int& i, int size, bool& released, vector<vec3> controlPoints, glm::vec3 og_lightPos, glm::vec3& lightPos, float& angle, bool& releasedAuto) {


	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && s_globalI < size - 1 && released)
	{
		released = false;
		s_globalI++;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && s_globalI > 0 && released)
	{
		released = false;
		s_globalI--;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && !released && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
	{
		punts = draw_Bezier_Curve_VAO(controlPoints, 4, 0.05, false);
		translationIndex = 0;
		released = true;
	}
	
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && releasedAuto)
	{
		releasedAuto = false;
		s_autoPlay.store(!s_autoPlay);
		if (s_autoPlay)
			worker = thread(incrementPlay);
		else {
			worker.detach();
		}
		
		
	}
	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
	{
		
		releasedAuto = true;
		
	}
	
	float DEGTORAD = glm::radians(angle);
	float r = sqrt(pow(og_lightPos.x, 2) + pow(og_lightPos.z, 2));
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		angle++;
	}
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		angle--;
	}
	lightPos.x = cos(DEGTORAD) * r;
	lightPos.z = sin(DEGTORAD) * r;
}

void setOpenGLVersion() {

	//Inicializar GLFW y Opengl version 3.3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

