#include "pch.h"
#include "Application.h"
#include "Common.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"
#include "Physics.h"
#include "BoxShape.h"
#include "RigidBody.h"
#include "cSound.h"
#include "cSoundMgr.h"
#include "SDL_ttf.h"
#include <iostream>
#include <cmath>

Application *Application::m_application = nullptr;
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

Application::Application()
{
}

void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError());
		exit(-1);
	}

	//setting openGL version to 4.2 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//creating window
	m_window = SDL_CreateWindow("AC130 Death Squad", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight,
		SDL_WINDOW_OPENGL);

	ren = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_CaptureMouse(SDL_TRUE);

	OpenGlInit();
	GameInit();
}

void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();

	SDL_GL_SetSwapInterval(1);

	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));

	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));
	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));

	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glCullFace(GL_BACK));

	glDisable(GL_CULL_FACE);
}

void Application::GameInit()
{
	// Initialise the sound manager
	if (theSoundMgr->initMixer())
	{
		// Load game sounds
		soundList = { "theme", "propeller", "explosion" };
		soundTypes = { soundType::music, soundType::music, soundType::sfx };
		soundsToUse = { ASSET_AUDIO_PATH_Theme + "the-descent-by-kevin-macleod-from-filmmusic-io.mp3", ASSET_AUDIO_PATH_Theme + "12812__jilliancallahan__generic-prop-idle.wav", ASSET_AUDIO_PATH_SFX + "235968__tommccann__explosion-01.wav" };
		for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
		{
			theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
		}
		// Play the theme on a constant loop
		theSoundMgr->getSnd("theme")->play(-1);
		theSoundMgr->getSnd("propeller")->play(-1);
	}
	
	ObjectiveUI->uiInit();


	//loading all resources
	Resources::GetInstance()->AddModel("Models/BerievA50.FBX");
	Resources::GetInstance()->AddModel("Models/desert obj.obj");
	Resources::GetInstance()->AddModel("Models/tank_trexhwm.obj");
	Resources::GetInstance()->AddModel("Models/Solder_Character_Mesh_OBJ01.obj");
	Resources::GetInstance()->AddModel("Models/Aim-54_Phoenix.obj");
	Resources::GetInstance()->AddTexture("AIM-54 phoenix texture.jpeg");
	Resources::GetInstance()->AddTexture("LowPoly_Solder_Def.jpg");
	Resources::GetInstance()->AddTexture("Beriev_2048.png");
	Resources::GetInstance()->AddTexture("sandTexture.png");
	Resources::GetInstance()->AddTexture("DefaultMaterial_Diffuse.png");
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "Shaders/simple_VERT.glsl", ASSET_PATH + "Shaders/simple_FRAG.glsl"), "simple");
	//Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "Shader/sandShaderVERT.glsl.txt", ASSET_PATH + "Shaders/sandShaderFRAG.glsl"), "Sand");


	// create entities
	AC130 = new Entity();
	m_entities.push_back(AC130);
	AC130->AddComponent(new MeshRenderer(
		Resources::GetInstance()->GetModel("Models/BerievA50.FBX"),
		Resources::GetInstance()->GetShader("simple"),
		Resources::GetInstance()->GetTexture("Beriev_2048.png"))
	);

	MeshRenderer* acMR = AC130->GetComponent<MeshRenderer>();
	AC130->GetTransform()->SetPosition(glm::vec3(0, 0.f, -10.f));
	AC130->AddComponent<RigidBody>();
	AC130->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(.25f, .25f, .25f)));
	AC130->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	AC130->GetTransform()->SetScale(glm::vec3(.25f, .25f, .25f));
	AC130->GetTransform()->SetRotation(glm::quat(0.f, 0.f, 90.f, 0.f));

	Entity* dessert = new Entity();
	m_entities.push_back(dessert);
	dessert->AddComponent(new MeshRenderer(
		Resources::GetInstance()->GetModel("Models/desert obj.obj"),
		Resources::GetInstance()->GetShader("simple"),
		Resources::GetInstance()->GetTexture("sandTexture.png"))
	);

	MeshRenderer* dMR = dessert->GetComponent<MeshRenderer>();
	dessert->GetTransform()->SetPosition(glm::vec3(0.f, -20.f, 0.f));
	dessert->AddComponent<RigidBody>();
	dessert->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 0.5f, 100.f)));
	dessert->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	dessert->GetTransform()->SetScale(glm::vec3(2.5f, 1.f, 2.5f));
	dessert->GetTransform()->SetRotation(glm::quat(0.0f, 0.0f, 180, 0.0f));

	thirdCam = new Entity();
	m_entities.push_back(thirdCam);
	CameraComp* thirdCC = new CameraComp();
	thirdCam->AddComponent(thirdCC);
	//thirdCam->GetTransform()->SetPosition(glm::vec3(AC130->GetTransform()->GetPosition().x, AC130->GetTransform()->GetPosition().y + 10, AC130->GetTransform()->GetPosition().z + 50));
	thirdCam->GetTransform()->SetPosition(AC130->GetTransform()->GetPosition() + AC130->GetTransform()->GetForward());
	thirdCam->GetTransform()->SetPosition(glm::vec3(thirdCam->GetTransform()->GetPosition().x, thirdCam->GetTransform()->GetPosition().y + 10, thirdCam->GetTransform()->GetPosition().z + 50)); // set to behind the ac130
	thirdCC->Start();

	firstCam = new Entity();
	m_entities.push_back(firstCam);
	CameraComp* firstCC = new CameraComp();
	firstCam->AddComponent(firstCC);
	firstCam->GetTransform()->SetPosition(glm::vec3(AC130->GetTransform()->GetPosition().x, AC130->GetTransform()->GetPosition().y - 10.5, AC130->GetTransform()->GetPosition().z)); // set to below the ac130
	//firstCam->GetTransform()->SetRotation(glm::quat(0.f, 0.f, 90.f, 0.f));

	for (unsigned int i = 0; i < 15; i++)
	{
		Entity* tank = new Entity();
		m_entities.push_back(tank);
		enemies.push_back(tank);
		tank->AddComponent(new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/tank_trexhwm.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("DefaultMaterial_Diffuse.png"))
		);

		MeshRenderer* tMR = tank->GetComponent<MeshRenderer>();
		tank->GetTransform()->SetPosition(glm::vec3(rand() % 150 - 50, -10, rand() % 150 - 50)); // set to random positions
		tank->AddComponent<RigidBody>();
		tank->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1, 1, 1)));
		tank->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		tank->GetTransform()->SetScale(glm::vec3(1, 1, 1));
	}

	for (unsigned int i = 0; i < 15; i++)
	{
		Entity* soilder = new Entity();
		m_entities.push_back(soilder);
		enemies.push_back(soilder);
		soilder->AddComponent(new MeshRenderer(
			Resources::GetInstance()->GetModel("Models/Solder_Character_Mesh_OBJ01.obj"),
			Resources::GetInstance()->GetShader("simple"),
			Resources::GetInstance()->GetTexture("LowPoly_Solder_Def.jpg"))
		);

		MeshRenderer* tMR = soilder->GetComponent<MeshRenderer>();
		soilder->GetTransform()->SetPosition(glm::vec3(rand() % 150 - 50, -10, rand() % 150 - 50)); // set to random positions
		soilder->AddComponent<RigidBody>();
		soilder->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1, 1, 1)));
		soilder->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
		soilder->GetTransform()->SetScale(glm::vec3(.1, .1, .1));
	}


	SDL_ShowCursor(SDL_FALSE);

	Resources::GetInstance()->ReleaseUnusedResources();

}

void Application::Loop()
{
	m_appState = AppState::RUNNING;

	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	while (m_appState != AppState::QUITTING)
	{
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_RETURN: // mute game if enter/return is pressed
					if (!pause)
					{
						theSoundMgr->getSnd("theme")->pause();
						Mix_Volume(-1, 0); // set volume to 0
						pause = true;
					}
					else
					{
						theSoundMgr->getSnd("theme")->resume();
						Mix_Volume(-1, 128); // set volume to max
						pause = false;
					}
					break;

				case SDLK_ESCAPE: // if escape is presseed show or hide the cursor
					if (!mouseShown)
					{
						SDL_ShowCursor(SDL_TRUE);
					}
					else
					{
						SDL_ShowCursor(SDL_FALSE);
					}
					mouseShown = !mouseShown;
					break;

				case SDLK_a: // if a is pressed
					if (!aim)
					{
						AC130->GetTransform()->RotateEulerAxis(-5 * m_worldDeltaTime, glm::vec3(0, 1, 0)); // rotate the ac130
						//thirdCam->rotateAround(AC130, -5 * m_worldDeltaTime);
						//thirdCam->GetComponent<CameraComp>()->relRot(AC130);
						//thirdCam->GetTransform()->SetPosition((AC130->GetTransform()->GetPosition() - AC130->GetTransform()->GetForward()) + glm::vec3(0, 0, 10));
						//thirdCam->GetTransform()->setForward(-AC130->GetTransform()->GetForward());
						//thirdCam->GetTransform()->setRight(-AC130->GetTransform()->GetRight());
						//thirdCam->GetTransform()->setUp(AC130->GetTransform()->GetUp());
						//thirdCam->GetTransform()->SetPosition(AC130->GetTransform()->GetForward()/* + glm::vec3(0, 10.5, 50)*/);
						thirdCam->GetTransform()->RotateEulerAxis(-5 * m_worldDeltaTime, glm::vec3(0, 1, 0)); // rotate camera
						thirdCam->GetTransform()->AddPosition(AC130->GetTransform()->GetRight() * glm::vec3(-1, -1, -1)); // update position relative to the ac130s position
					}
					break;

				case SDLK_d:
					if (!aim)
					{
						AC130->GetTransform()->RotateEulerAxis(5 * m_worldDeltaTime, glm::vec3(0, 1, 0));
						//thirdCam->rotateAround(AC130, 5 * m_worldDeltaTime);
						//thirdCam->GetComponent<CameraComp>()->relRot(AC130);
						//thirdCam->GetTransform()->SetPosition((AC130->GetTransform()->GetPosition() - AC130->GetTransform()->GetForward()) + glm::vec3(0, 0, 10));
						//thirdCam->GetTransform()->setForward(-AC130->GetTransform()->GetForward());
						//thirdCam->GetTransform()->setRight(-AC130->GetTransform()->GetRight());
						//thirdCam->GetTransform()->setUp(AC130->GetTransform()->GetUp());
						//thirdCam->GetTransform()->SetPosition(AC130->GetTransform()->GetForward()/* + glm::vec3(0, 10.5, 50)*/);
						thirdCam->GetTransform()->RotateEulerAxis(5 * m_worldDeltaTime, glm::vec3(0, 1, 0));
						thirdCam->GetTransform()->AddPosition(AC130->GetTransform()->GetRight());
					}
					break;

				case SDLK_SPACE: // if space is pressed alternate cameras
					if (!aim)
					{
						firstCam->GetComponent<CameraComp>()->setCamera();
						aim = true;
					}
					else
					{
						thirdCam->GetComponent<CameraComp>()->setCamera();
						aim = false;
					}
					break;
				case SDLK_LSHIFT: // if left shift is pressed showor hide ui
					showUI = !showUI;
					break;
				}
				break;
				//record when the user releases a key
			case SDL_MOUSEMOTION: // if mouse moves
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				if (aim)
				{
					firstCam->GetTransform()->RotateEulerAxis((m_worldDeltaTime) * event.motion.xrel, firstCam->GetTransform()->GetUp()); // rotate the camera
					firstCam->GetTransform()->RotateEulerAxis((m_worldDeltaTime) * event.motion.yrel, firstCam->GetTransform()->GetRight());
					firstCam->GetTransform()->RotateEulerAxis(0, firstCam->GetTransform()->GetForward());
					//firstCam->GetComponent<CameraComp>()->yaw += 0.01 * event.motion.xrel * m_worldDeltaTime;
					//firstCam->GetComponent<CameraComp>()->pitch += 0.01 * event.motion.yrel * m_worldDeltaTime;
					//if (firstCam->GetComponent<CameraComp>()->pitch > AI_MATH_TWO_PI_F)
					//{
					//	firstCam->GetComponent<CameraComp>()->pitch = AI_MATH_TWO_PI_F - 0.0001f;
					//}
					//else if (firstCam->GetComponent<CameraComp>()->pitch < -AI_MATH_TWO_PI_F)
					//{
					//	firstCam->GetComponent<CameraComp>()->pitch = -AI_MATH_TWO_PI_F + 0.001f;
					//}
					//float forX = -sin(firstCam->GetComponent<CameraComp>()->yaw) * cos(firstCam->GetComponent<CameraComp>()->pitch);
					//float forY = -sin(firstCam->GetComponent<CameraComp>()->pitch);
					//float forZ = -cos(firstCam->GetComponent<CameraComp>()->yaw) * cos(firstCam->GetComponent<CameraComp>()->pitch);
					//firstCam->GetTransform()->setForward(glm::vec3(forX, forY, forZ));

					//float rX = -cos(firstCam->GetComponent<CameraComp>()->yaw);
					//float rY = 0.0f;
					//float rZ = sin(firstCam->GetComponent<CameraComp>()->yaw);
					//firstCam->GetTransform()->setRight(glm::vec3(rX, rY, rZ));

					//firstCam->GetTransform()->setUp(glm::cross(firstCam->GetTransform()->GetForward(), firstCam->GetTransform()->GetRight()));

					//firstCam->GetTransform()->setForward(glm::normalize(firstCam->GetTransform()->GetForward()));
					//firstCam->GetTransform()->setRight(glm::normalize(firstCam->GetTransform()->GetRight()));
					//firstCam->GetTransform()->setUp(glm::normalize(firstCam->GetTransform()->GetUp()));

					////https://stackoverflow.com/questions/34378214/how-to-move-around-camera-using-mouse-in-opengl
				}
				break;
				case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT: // if left mouse is clicked
					if (aim)
					{
						// load missile
						Resources::GetInstance()->AddModel("Models/Aim-54_Phoenix.obj");
						Resources::GetInstance()->AddTexture("AIM-54 phoenix texture.jpeg");
						Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "Shaders/simple_VERT.glsl", ASSET_PATH + "Shaders/simple_FRAG.glsl"), "simple");

						Entity* missile = new Entity();
						m_entities.push_back(missile);
						missiles.push_back(missile);
						missile->AddComponent(new MeshRenderer(
							Resources::GetInstance()->GetModel("Models/Aim-54_Phoenix.obj"),
							Resources::GetInstance()->GetShader("simple"),
							Resources::GetInstance()->GetTexture("AIM-54 phoenix texture.jpeg")
						));

						MeshRenderer* mMR = missile->GetComponent<MeshRenderer>();
						missile->GetTransform()->SetPosition(firstCam->GetTransform()->GetPosition());
						missile->AddComponent<RigidBody>();
						missile->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(10, 10, 10)));
						missile->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
						missile->GetTransform()->setForward(firstCam->GetTransform()->GetForward());
						missile->GetTransform()->setRight(firstCam->GetTransform()->GetRight());
						missile->GetTransform()->SetScale(glm::vec3(.01f, .01f, .01f));
						missile->direction = firstCam->GetTransform()->GetForward();
					}
					break;
				}
			}
		}

		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		// update entities positions
		AC130->GetTransform()->AddPosition(-AC130->GetTransform()->GetForward()/ glm::vec3(3, 3, 3));
		thirdCam->GetTransform()->AddPosition(-AC130->GetTransform()->GetForward()/ glm::vec3(3, 3, 3));
		firstCam->GetTransform()->SetPosition(glm::vec3(AC130->GetTransform()->GetPosition().x, AC130->GetTransform()->GetPosition().y - 2.5, AC130->GetTransform()->GetPosition().z));
		for (unsigned int i = 0; i < missiles.size(); i++)
		{
			if (missiles.at(i) != nullptr)
			{
				missiles.at(i)->GetTransform()->AddPosition(missiles.at(i)->direction/ glm::vec3(3, 3, 3));
			}
		}

		// collisions
		for (unsigned int i = 0; i < missiles.size(); i++)
		{
			for (unsigned int j = 0; j < enemies.size(); j++)
			{
				if (Physics::GetInstance()->collision(missiles.at(i)->GetComponent<RigidBody>()->Get(), enemies.at(j)->GetComponent<RigidBody>()->Get()))
				{
					if (missiles.at(i)->colidable && enemies.at(j)->colidable)
					{
						missiles.at(i)->colidable = false;
						missiles.at(i)->GetTransform()->SetPosition(glm::vec3(-INFINITY, -INFINITY, -INFINITY));
						enemies.at(j)->colidable = false;
						enemies.at(j)->GetTransform()->SetPosition(glm::vec3(-INFINITY, -INFINITY, -INFINITY));
						score += 5;
						theSoundMgr->getSnd("explosion")->play(0);
					}
				}
			}
		}

		Physics::GetInstance()->Update(deltaTime);
		//update and render all entities
		Update(deltaTime);
		Render();

		SDL_GL_SwapWindow(m_window);
	}
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	//SDL_DestroyTexture(text);
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}

void Application::Update(float deltaTime)
{
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}

	
}

void Application::Render()
{
	/* Clear context */
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_PixelFormat* format = new SDL_PixelFormat();
	//SDL_UpdateWindowSurface(ObjectiveUI->renderUI(ObjectiveUI->openFont("..\\firefightbb_reg.ttf", 20), 225, 0, 0, 225, 0, 0, 225, 0, (char*)score, ObjectiveUI->SHADED);)
	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
	if (showUI) { ObjectiveUI->renderText(m_window, ObjectiveUI->openFont("Assets/Fonts/BlackPearl.ttf", 20), 225, 0, 0, 225, 0, 0, 225, 225, std::to_string(score), ren); } // show ui
}


Application::~Application()
{

}

Application * Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}

void Application::Run()
{
	Init();
	Loop();
	Quit();
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}
