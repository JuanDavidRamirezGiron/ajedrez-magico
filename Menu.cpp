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

void Menu::init(FENFileReader* &fenFileReader, Camera& camera)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("MENU", NULL, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Seleccionar Partida")) {
			string file = "";
			if (ImGui::MenuItem("Partida 1")) {
				file = "Partida1.fen";
				boardChange = true;
			}
			if (ImGui::MenuItem("Partida 2")) {
				file = "Partida2.fen";
				boardChange = true;
			}
			if (ImGui::MenuItem("Partida 3")) {
				file = "Partida3.fen";
				boardChange = true;
			}
			fenFileReader = new FENFileReader(file);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Camara"))
		{
			if (ImGui::MenuItem("1.Camara cenital")) {
				camera.CenitalCamera();
			}
			if (ImGui::MenuItem("2.Camara Jugador Blancas")) {
				camera.WhitePiecesCamera();
			}
			if (ImGui::MenuItem("3.Camara Jugador Negras")) {
				camera.BlackPiecesCamera();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


