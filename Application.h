#pragma once
#include "Entity.h"
#include <string>
#include "SDL_ttf.h""
#include "UIGen.h"

class Camera;

#define WINDOW_W Application::GetInstance()->GetWindowWidth()
#define WINDOW_H Application::GetInstance()->GetWindowHeight()

enum AppState
{
	INITILISING, RUNNING, QUITTING
};

class Application
{
private:
	//private variables
	static Application* m_application;
	SDL_Window *m_window = nullptr;
	SDL_GLContext m_glContext = nullptr;
	int m_windowWidth = 1500;
	int m_windowHeight = 900;
	AppState m_appState = AppState::INITILISING;
	float m_worldDeltaTime = 0.f;
	std::vector<Entity*> m_entities;
	std::vector<Entity*> enemies;
	std::vector<Entity*> missiles;
	Camera* m_mainCamera = nullptr;
	// Game Sounds
	std::vector<std::string> soundList;
	std::vector<soundType> soundTypes;
	std::vector<std::string> soundsToUse;
	bool pause = false;
	bool aim = false;
	bool mouseShown = false;
	bool showUI = true;

	//private functions
	Application();
	void Init();
	void OpenGlInit();
	void GameInit();
	void Loop();
	void Quit();
	void Update(float deltaTime);
	void Render();

	Entity* AC130;
	Entity* dessert;
	Entity* firstCam;
	Entity* thirdCam;


	
	unsigned int score = 0;
	std::string displaySocre;
	SDL_Rect uiREct;

	UIGen* ObjectiveUI = new UIGen();
	SDL_Renderer* ren;

public:
	//public functions
	~Application();
	static Application* GetInstance();
	void Run();

	inline int GetWindowHeight() { return m_windowHeight; }
	inline int GetWindowWidth() { return m_windowWidth; }

	inline Camera* GetCamera() { return m_mainCamera; }
	void SetCamera(Camera* camera);
};

