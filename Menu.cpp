#include "Menu.h"

Menu::Menu(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	boardChange = false;
}

void Menu::shutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Menu::init(FENFileReader* &fenFileReader, Camera& camera, int globalPlay, int numPlays, bool autoPlay)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("MENU", NULL, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Seleccionar partida")) {
			string file = "";
			if (ImGui::MenuItem("Kasparov vs Topalov (1999)")) {
				file = "Partida1.fen";
				boardChange = true;
			}
			if (ImGui::MenuItem("Morphy vs Duque de Brunswick y Conde Isouard (1858)")) {
				file = "Partida2.fen";
				boardChange = true;
			}
			if (ImGui::MenuItem("Aronian vs. Anand, Wijk aan Zee (2013)")) {
				file = "Partida3.fen";
				boardChange = true;
			}
			fenFileReader = new FENFileReader(file);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Camara"))
		{
			if (ImGui::MenuItem("Camara cenital. Tecla 1")) {
				camera.CenitalCamera();
			}
			if (ImGui::MenuItem("Camara jugador blancas. Tecla 2")) {
				camera.WhitePiecesCamera();
			}
			if (ImGui::MenuItem("Camara jugador negras. Tecla 3")) {
				camera.BlackPiecesCamera();
			}
			ImGui::EndMenu();
		}
		
		ImGui::EndMenuBar();
	}

	
	
	ImGui::Text("Turno %d/%d", globalPlay+1, numPlays-1);
	ImGui::Text("Mover adelante.    \t\t\t\t\t\tTecla >");
	ImGui::Text("Mover atras.   \t\t\t\t\t\t\tTecla <");
	if (autoPlay) {
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 225, 150, 255));
	ImGui::Text("Jugar auto.    \t\t\t\t\t\t\tTecla K");
	ImGui::PopStyleColor();
	}
	else {
		ImGui::Text("Jugar auto.    \t\t\t\t\t\t\tTecla K");
	}

	ImGui::Text("Mover foco de luz sentido horario. \t\tTecla Q");
	ImGui::Text("Mover foco de luz sentido antihorario. \tTecla E");
		
		
		
	

	


	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


