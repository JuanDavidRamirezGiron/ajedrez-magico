#include "Menu.h"

Menu::Menu(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void Menu::shutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Menu::init(Camera& camera)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("MENÚ DELOKOS", NULL, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Fichero")) {
			if (ImGui::MenuItem("1.Importar Partida")) {
				camera.CenitalCamera();
			}
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
