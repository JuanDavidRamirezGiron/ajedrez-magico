
#include"Model.h"

//constantes medida ventana
const unsigned int width = 1280;
const unsigned int height = 720;




int main() {
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

	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0),
	};
	Shader shaderProgram("default.vert", "default.frag");

	//creamos los distintos objetos
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	Model king("models/horse.obj");
	Model board("models/board_2.obj");

	while (!glfwWindowShouldClose(window)) {
		//ajustar color barrido
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		//Dibujamos tablero y piezas
		//Draw(shader, camara, vector translacion, angulo rotacion en radianes, vector ejes rotacion, vector escalado)
		king.Draw(shaderProgram, camera, glm::vec3(0.0f, 0.0f, 0.0f), 0.f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f));
		board.Draw(shaderProgram, camera, glm::vec3(0.0f, -4.6f, 0.0f), 1.5708f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));


		//intercambio buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}