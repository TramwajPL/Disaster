#include "../pch.h"

#include <glm/glm.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glm\gtx\string_cast.hpp>
#include <stb_image.h>

#include "Model.h"
#include "RenderEngine.h"
#include "TexturedRect.h"

#include "../GUI/GUI.h"
#include "../GUI/Button.h"
#include "../GUI/MenuGUI.h"
#include "../GUI/PokemonGUI.h"
#include "../GUI/StoreGUI.h"
#include "../GUI/WarehouseGUI.h"
#include "../GUI/BetweenLevelsGUI.h"
#include "../GUI/EndGameGUI.h"


#include "../GUI/CreditsGUI.h"
#include "../Game/Enemy.h"
#include "../Game/Player.h"

#include "../Core/Game.h"
#include "../Core/Timestep.h"
#include "../Game/TurnSystem.h"
#include "../Core/GameObject.h"
#include "../Core/MousePicker.h"
#include "../PhysicsEngine/Aabb.h"
#include "../Core/EntityComponent.h"
#include "../Components/RenderModelComponent.h"
#include "ParticleSystem.h"
#include "../GUI/DestructionBar.h"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

//const unsigned int SCR_WIDTH = 1366;
//const unsigned int SCR_HEIGHT = 768;

namespace Clumsy
{
	RenderEngine::RenderEngine(GLFWwindow* window, Window* window2, Camera* camera) :
		m_Window(window2),
		m_GLFWWindow(window),
		m_Camera(camera)
	{
		isRunning = false;

		m_Shader = new Shader("../Clumsy/src/Shaders/shadows_shader_VS.glsl", "../Clumsy/src/Shaders/shadows_shader_FS.glsl");
		m_Postprocessing = new Shader("../Clumsy/src/Shaders/post_VS.glsl", "../Clumsy/src/Shaders/post_FS.glsl");
		simpleDepthShader = new Shader("../Clumsy/src/Shaders/shadow_mapping_depth_VS.glsl", "../Clumsy/src/Shaders/shadow_mapping_depth_FS.glsl");
		debugDepthQuadShader = new Shader("../Clumsy/src/Shaders/debug_depth_quad_VS.glsl", "../Clumsy/src/Shaders/debug_depth_quad_FS.glsl");
		particleShader = new Shader("../Clumsy/src/Shaders/particle_VS.glsl", "../Clumsy/src/Shaders/particle_FS.glsl");
		particleTexture = loadTextureFromFile("../Clumsy/src/models/flame.png", GL_TRUE);
		greenParticleTexture = loadTextureFromFile("../Clumsy/src/models/greenParticle.png", GL_TRUE);
		violetParticleTexture = loadTextureFromFile("../Clumsy/src/models/violet.png", GL_TRUE);
		textShader = new Shader("../Clumsy/src/Shaders/text_VS.glsl", "../Clumsy/src/Shaders/text_FS.glsl");
		buttonShader = new Shader("../Clumsy/src/Shaders/button_VS.glsl", "../Clumsy/src/Shaders/button_FS.glsl");

		shaderCube = new Shader("../Clumsy/src/Shaders/cubeMap_VS.glsl", "../Clumsy/src/Shaders/cubeMap_FS.glsl");

		Effects = new PostProcessor(*m_Postprocessing, SCR_WIDTH, SCR_HEIGHT);
		shaderCube = new Shader("../Clumsy/src/Shaders/cubeMap_VS.glsl", "../Clumsy/src/Shaders/cubeMap_FS.glsl");
		
		mainMenuShader = new Shader("../Clumsy/src/Shaders/main_menu_VS.glsl", "../Clumsy/src/Shaders/main_menu_FS.glsl");

		//models
		playerModel = new Clumsy::Model();
		playerModel->loadModel("../Clumsy/src/models/playerModels/Breathing Idle/Breathing Idle.dae");
		playerWalkModel = new Model();
		playerWalkModel->loadModel("../Clumsy/src/models/playerModels/Dwarf Walk/Dwarf Walk.dae");

		enemyModel = new Clumsy::Model();
		enemyModel->loadModel("../Clumsy/src/models/enemyModels/Idle/Idle.dae"); 
		enemyWalkModel = new Model();
		enemyWalkModel->loadModel("../Clumsy/src/models/enemyModels/Mutant Walking/Mutant Walking.dae");


		m3 = new Model();
		m4 = new Model();
		m5 = new Model();
		m6 = new Model();
		m7 = new Model();
		mBurned = new Model();
		mShop = new Model();

		m3->loadModel("../Clumsy/src/models/hexes/groundEarth_base_color.obj");
		m4->loadModel("../Clumsy/src/models/hexes/Desert_ground_Albedo.obj");
		m5->loadModel("../Clumsy/src/models/hexes/Stone_Albedo.obj");
		m6->loadModel("../Clumsy/src/models/hexes/Water_Albedo.obj");
		m7->loadModel("../Clumsy/src/models/hexes/tree_Oliwiw.obj");
		mBurned->loadModel("../Clumsy/src/models/hexes/Burned_ground.obj");
		mShop->loadModel("../Clumsy/src/models/shop/shop.obj");

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/*debugDepthQuadShader->use();
		debugDepthQuadShader->setInt("depthMap", 0);*/

		glGenFramebuffers(1, &depthMapFBO);
		// create depth texture
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_Shader->use();
		m_Shader->setInt("diffuseTexture", 0);
		m_Shader->setInt("shadowMap", 1);

		particleSystem = new ParticleSystem(particleShader, particleTexture);
		greenParticle = new ParticleSystem(particleShader, violetParticleTexture);
		m_TilesIllumination = new ParticleSystem(particleShader, greenParticleTexture);

		gui = new GUI();
		m_ButtonCameraOnPlayer = new Button(glm::vec2(-0.88f, 0.65f), "Find Player", glm::vec3(0.16f, 0.03f, 0.29f), glm::vec2(0.2f, 0.08f));
		m_ButtonEndTurn = new Button(glm::vec2(-0.88f, 0.55f), "End Turn", glm::vec3(0.16f, 0.03f, 0.29f), glm::vec2(0.2f, 0.08f));
		m_ButtonRestart = new Button(glm::vec2(-0.88f, 0.45f), " Restart", glm::vec3(0.16f, 0.03f, 0.29f), glm::vec2(0.2f, 0.08f));
		m_StoreGUI = new StoreGUI();
		m_WarehouseGUI = new WarehouseGUI();
		m_MenuGUI = new MenuGUI();
		m_CreditsGUI = new CreditsGUI();
		m_BetweenLevelsGUI = new BetweenLevelsGUI();
		m_EndGUI = new EndGameGUI();

	    m_TexturedRect = new TexturedRect("../Clumsy/src/models/tutek1.jpg", glm::vec3(0.9f, 0.9f, 0.0f), glm::vec3(0.9, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.9f, 0.0f));
		
		m_Instruction2 = new TexturedRect("../Clumsy/src/models/tutek2.jpg", glm::vec3(0.9f, 0.9f, 0.0f), glm::vec3(0.9, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.9f, 0.0f));
		m_Instruction3 = new TexturedRect("../Clumsy/src/models/tutek3.jpg", glm::vec3(0.9f, 0.9f, 0.0f), glm::vec3(0.9, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.9f, 0.0f));
		m_InstructionLevel2 = new TexturedRect("../Clumsy/src/models/tutek4.jpg", glm::vec3(0.9f, 0.9f, 0.0f), glm::vec3(0.9, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.9f, 0.0f));

		m_PokemonGUI = new PokemonGUI(buttonShader);
		m_PokemonRect = new TexturedRect("../Clumsy/src/models/battle2.jpg", glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
	    m_PokemonEnemy = new TexturedRect("../Clumsy/src/models/ent.jpg", glm::vec3(0.7f, 0.8f, 0.0f), glm::vec3(0.7f, 0.4f, 0.0f), glm::vec3(0.5f, 0.4f, 0.0f), glm::vec3(0.5f, 0.8f, 0.0f));
	    m_PokemonPlayer = new TexturedRect("../Clumsy/src/models/lumberjack.jpg", glm::vec3(-0.6f, 0.4f, 0.0f), glm::vec3(-0.6f, 0.0f, 0.0f), glm::vec3(-0.8f, 0.0f, 0.0f), glm::vec3(-0.8f, 0.4f, 0.0f));

		background = new DestructionBar(glm::vec3(-0.5f, -0.82f, 0.5f), glm::vec3(0.14f, 0.52f, 0.25f), buttonShader);
		destructionBar = new DestructionBar(glm::vec3(-0.5f, -0.83f, 0.5f), glm::vec3(0.65f, 0.52f, 0.09f), buttonShader);

	}

	TextureClass RenderEngine::loadTextureFromFile(const char* file, bool alpha)
	{
		// create texture object
		TextureClass texture;
		if (alpha)
		{
			texture.Internal_Format = GL_RGBA;
			texture.Image_Format = GL_RGBA;
		}
		// load image
		int width, height, nrChannels;
		unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
		// now generate texture
		texture.Generate(width, height, data);
		// and finally free image data
		stbi_image_free(data);
		return texture;
	}

	void RenderEngine::CreateInstance(GLFWwindow* window, Window* window2, Camera* camera)
	{
		assert(!m_Instance);
		m_Instance = new RenderEngine(window, window2, camera);
	}

	RenderEngine* RenderEngine::GetInstance()
	{
		assert(m_Instance);
		return m_Instance;
	}	

	void RenderEngine::Render(GameObject object)
	{
		glEnable(GL_DEPTH_TEST);
		/*glDepthMask(GL_TRUE);
		glDepthFunc(GL_ALWAYS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

		m_Counter = 0;

		float time = (float)glfwGetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;
		processInput(timestep.GetSeconds());

		// lighting info
		glm::vec3 lightPos(20.0f, 40.0f, -20.0f);

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;

		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		// render scene from light's point of view
		simpleDepthShader->use();
		simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE0);
		object.RenderAll(*simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*glBindFramebuffer(GL_READ_FRAMEBUFFER, Effects->DepthFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Effects->MSFBO);*/
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*projection = glm::perspective(glm::radians(m_Camera->GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = m_Camera->GetViewMatrix();*/

		m_Shader->use();
		glm::mat4 projection = glm::perspective(glm::radians(m_Camera->GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = m_Camera->GetViewMatrix();
		m_Shader->setMat4("projection", projection);
		m_Shader->setMat4("view", view);
		// set light uniforms
		m_Shader->SetDirectionalLight(0.6, m_Camera->GetPosition(), lightPos, lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		object.RenderAll(*m_Shader);
		
		for (int i = 0; i < m_Cubes.size(); i++)
		{
			m_Cubes[i]->Render(shaderCube);
		}

		if (isPlayed == true) {
			particleSystem->GenerateNewParticles(timestep.GetSeconds(), m_TreePosition);
			fireTime += timestep.GetSeconds();
			if (fireTime >= fireMaxTime)
			{
				isPlayed = false;
				fireTime = 0;
			}
		}
		particleSystem->Update(timestep.GetSeconds(), view, projection);
		particleSystem->Render(view, projection);

		if (enemySpawn == true && isPlayed == false) {
			for (int i = 0; i < m_TreeSpawnPosition.size(); i++)
			{
				greenParticle->GenerateNewParticles(timestep.GetSeconds(), m_TreeSpawnPosition[i]);
			}
			particleTime += timestep.GetSeconds();
			if (particleTime >= particleMaxTime)
			{
				enemySpawn = false;
				particleTime = 0;
			}
		}
		

		greenParticle->Update(timestep.GetSeconds(), view, projection);
		greenParticle->Render(view, projection); //?

		CreateTilesIllumination();

		for (int i = 0; i < m_ParticlesOnTiles.size(); i++)
		{
			m_TilesIllumination->GenerateTileIllumination(timestep.GetSeconds(), m_ParticlesOnTiles[i]);
		}

		m_TilesIllumination->Update(timestep.GetSeconds(), view, projection);
		m_TilesIllumination->Render(view, projection);

	
		if (m_MoveEnemy == true)
		{
			enemy->m_Rmc->m_Model = enemyWalkModel;
			enemyTime += timestep.GetSeconds();
			if (enemyTime >= enemymaxTime)
			{
				enemy->m_Rmc->m_Model = enemyModel;
				m_MoveEnemy = false;
				enemyTime = 0;
			}
		}


		if (m_MovePlayer == true && TurnSystem::GetInstance()->GetActivePlayer()->m_Rmc != nullptr)
		{
			for (int j = 0; j < TurnSystem::GetInstance()->GetPlayers().size(); j++)
			{
				if (TurnSystem::GetInstance()->GetPlayers()[j]->isTurn) 
				{
					TurnSystem::GetInstance()->GetActivePlayer()->m_Rmc->m_Model = playerWalkModel;
				}
			}
			playerTime += timestep.GetSeconds();
			if (playerTime >= enemymaxTime)
			{
				TurnSystem::GetInstance()->GetActivePlayer()->m_Rmc->m_Model = playerModel;
				m_MovePlayer = false;
				playerTime = 0;
			}
		}
	}

	void RenderEngine::RenderGUI()
	{
		glDisable(GL_DEPTH_TEST);

		glDisable(GL_CULL_FACE);
		buttonShader->use();
		background->Render(glm::vec3(m_XScaleBackground, 0.10f, 0.3f)); //zmieniæ Y
		destructionBar->Render(glm::vec3(m_ScaleUp, 0.13f, 0.3f));
		glEnable(GL_CULL_FACE);

		glDisable(GL_DEPTH_TEST);

		glm::mat4 projectionGUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
		textShader->use();
		textShader->setMat4("projection", projectionGUI);
		
		mainMenuShader->use();
	
		if (m_ThirdInstruction)
		{
			if (m_FirstInstruction && m_SecondInstruction) {
				m_TexturedRect->Render(mainMenuShader);
			} else if (m_FirstInstruction == false && m_SecondInstruction == true) {
				m_Instruction2->Render(mainMenuShader);
			}
			else {
				m_Instruction3->Render(mainMenuShader);
			}
		}

		if (m_SecondLevelInstruction) {
			m_InstructionLevel2->Render(mainMenuShader);
		}

		// fail info
		if (m_MoveTooFar)
		{
			gui->RenderText(textShader, "I can't go that far at once!", SCR_WIDTH / 2 - 200.0f, SCR_HEIGHT - 200.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
		if (m_TooMuchWood)
		{
			gui->RenderText(textShader, "Not enough space for wood!", SCR_WIDTH / 2 - 225.0f, SCR_HEIGHT - 250.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
		}
		if (m_TileOccupied)
		{
			gui->RenderText(textShader, "Tile already occupied!", SCR_WIDTH / 2 - 185.0f, SCR_HEIGHT - 300.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
		}

		Player* player = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetActivePlayer());
		if (player)
		{
			gui->RenderText(textShader, "Wood: " + std::to_string(player->GetWoodCount()) + " / " + std::to_string(player->GetMaxWood()), 
				25.0f, SCR_HEIGHT - 100.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(textShader, "Actions: " + std::to_string(player->GetAvailableActions()), 
				25.0f, SCR_HEIGHT - 150.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
		}

		if (!m_StoreGUI->IsEnabled() && !m_WarehouseGUI->IsEnabled())
		{
			buttonShader->use();
			m_ButtonCameraOnPlayer->Render(buttonShader);
			m_ButtonEndTurn->Render(buttonShader);
			m_ButtonRestart->Render(buttonShader);
			
			gui->RenderText(textShader, m_ButtonCameraOnPlayer->GetText(), 25.0f, SCR_HEIGHT - 200.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(textShader, m_ButtonEndTurn->GetText(), 25.0f, SCR_HEIGHT - 250.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
			gui->RenderText(textShader, m_ButtonRestart->GetText(), 25.0f, SCR_HEIGHT - 300.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));						
		}

		m_StoreGUI->Render(mainMenuShader ,buttonShader, textShader, SCR_WIDTH, SCR_HEIGHT);
		m_WarehouseGUI->Render(mainMenuShader ,buttonShader, textShader, SCR_WIDTH, SCR_HEIGHT);	
	
	}

	void RenderEngine::RenderMainMenu()
	{
		glDisable(GL_DEPTH_TEST);

		glm::mat4 projectionGUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
		textShader->use();
		textShader->setMat4("projection", projectionGUI);

		m_MenuGUI->Render(mainMenuShader, buttonShader, textShader, SCR_WIDTH, SCR_HEIGHT);
	}
	

	void RenderEngine::RenderCreditsGUI() {
		glDisable(GL_DEPTH_TEST);

		glm::mat4 projectionGUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
		textShader->use();
		textShader->setMat4("projection", projectionGUI);

		m_CreditsGUI->Render(mainMenuShader, buttonShader, textShader, SCR_WIDTH, SCR_HEIGHT);
	
	}

	void RenderEngine::RenderEndGame()
	{
		glDisable(GL_DEPTH_TEST);

		glm::mat4 projectionGUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
		textShader->use();
		textShader->setMat4("projection", projectionGUI);

		m_EndGUI->Render(mainMenuShader, buttonShader, textShader, SCR_WIDTH, SCR_HEIGHT);
	}

	void RenderEngine::RenderBetweenLevelsGUI() {
		glDisable(GL_DEPTH_TEST);

		glm::mat4 projectionGUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
		textShader->use();
		textShader->setMat4("projection", projectionGUI);

		m_BetweenLevelsGUI->Render(mainMenuShader, buttonShader, textShader, SCR_WIDTH, SCR_HEIGHT);
	}

	void RenderEngine::RenderPokemonGUI()
	{
		glDisable(GL_DEPTH_TEST);

		glm::mat4 projectionGUI = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
		textShader->use();
		textShader->setMat4("projection", projectionGUI);

		m_PokemonRect->Render(mainMenuShader);
		m_PokemonPlayer->Render(mainMenuShader);
		m_PokemonEnemy->Render(mainMenuShader);
		m_PokemonGUI->Render(mainMenuShader, buttonShader, textShader, SCR_WIDTH, SCR_HEIGHT);
	}

	void RenderEngine::CleanUp()
	{
		m_Window->~Window();
	}

	void RenderEngine::processInput(float deltaTime)
	{
		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_GLFWWindow, true);

		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_W) == GLFW_PRESS) {
			isFrustumSet = false;
			m_Camera->ProcessKeyboard(UP, deltaTime);
		}

		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_S) == GLFW_PRESS) {
			isFrustumSet = false;
			m_Camera->ProcessKeyboard(DOWN, deltaTime);
		}

		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_A) == GLFW_PRESS) {
			isFrustumSet = false;
			m_Camera->ProcessKeyboard(LEFT, deltaTime);
		}

		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_D) == GLFW_PRESS) {
			isFrustumSet = false;
			m_Camera->ProcessKeyboard(RIGHT, deltaTime);
		}

		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_R) == GLFW_PRESS) {
			isFrustumSet = false;
			m_Camera->ProcessKeyboard(FORWARD, deltaTime);
		}

		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_F) == GLFW_PRESS) {
			isFrustumSet = false;
			m_Camera->ProcessKeyboard(BACKWARD, deltaTime);
		}

		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_G) == GLFW_PRESS) {
			Effects->m_Grey = !Effects->m_Grey;
		}

		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_O) == GLFW_PRESS) {
			m_StoreGUI->SetEnabled(!m_StoreGUI->IsEnabled());
		}

		if (glfwGetKey(m_GLFWWindow, GLFW_KEY_P) == GLFW_PRESS) {
			m_WarehouseGUI->SetEnabled(!m_WarehouseGUI->IsEnabled());
		}

	}

	unsigned int RenderEngine::loadCubemap(std::vector<std::string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrComponents;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}

	void RenderEngine::UpdateCubes()
	{
		for (int i = 0; i < m_Cubes.size(); i++)
			m_Cubes[i]->Update();
	}

	void RenderEngine::CreateTilesIllumination()
	{
		ClearTileIlluminationPositions();
		//Illuminating tiles
		Player* player = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetActivePlayer());

		if (player) {
			glm::vec3 playerPosition = player->m_Rmc->m_Transform.GetPos();
			for (int i = 0; i < groundSand.size(); i++) {
				if (glm::length(playerPosition - groundSand[i].GetPos()) < 1.5f) {
					if (!CheckIfTileOccupied(player, groundSand[i])) {
						SetTileIlluminationPosition(groundSand[i]);
					}
				}
			}
			for (int i = 0; i < groundBurned.size(); i++) {
				if (glm::length(playerPosition - groundBurned[i].GetPos()) < 1.5f) {
					if (!CheckIfTileOccupied(player, groundBurned[i])) {
						SetTileIlluminationPosition(groundBurned[i]);
					}
				}
			}
			for (int i = 0; i < ground.size(); i++) {
				if (glm::length(playerPosition - ground[i].GetPos()) < 1.5f) {
					if (!CheckIfTileOccupied(player, ground[i])) {
						if (!CheckIfTree(ground[i])) {
							SetTileIlluminationPosition(ground[i]);
						}
						else if (CheckIfTree(ground[i]) && player->IsIncrementingWoodCountPossible()) {
							SetTileIlluminationPosition(ground[i]);
						}
					}
				}
			}
		}
	}
	

	bool RenderEngine::CheckIfTileOccupied(Player* player, Transform desiredHex)
	{
		for (int i = 0; i < TurnSystem::GetInstance()->GetPlayers().size(); i++)
		{
			Player* other = dynamic_cast<Player*>(TurnSystem::GetInstance()->GetPlayers()[i]->GetGameObject());
			if (other && other != player)
			{
				if (glm::length(other->m_Rmc->m_Transform.GetPos() - desiredHex.GetPos()) < 0.8f)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool RenderEngine::CheckIfTree(Transform desiredHex)
	{
		for (int k = 0; k < treeTransforms.size(); k++)
		{
			if (desiredHex.GetPos() == treeTransforms[k].GetPos())
			{
				return true;
			}
		}
		return false;
	}

}