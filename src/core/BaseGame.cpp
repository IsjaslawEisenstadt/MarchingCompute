#include "BaseGame.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

void BaseGame::Start()
{
	float lastTime = 0.0f;
	float lastCapTime = 0.0f;

	while (!m_Window->ShouldClose())
	{
		float time = m_Window->GetTime();
		float deltaTime = time - lastTime;
		lastTime = time;

		m_Window->PollInput();
		ImStartFrame();
		Update(deltaTime);
		ImDrawFrame();
		m_Window->PresentFrame();
	}
}

BaseGame::BaseGame(unsigned int width, unsigned int height, const std::string &title)
	: m_Window(std::make_shared<Window>(width, height, title))
{
	m_Window->AddInputListener(*this);
	Renderer::Init();
	ImInit();
}

BaseGame::~BaseGame()
{
	ImCleanup();
}

void BaseGame::ImInit()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetPointer(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void BaseGame::ImStartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void BaseGame::ImDrawFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void BaseGame::ImCleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
