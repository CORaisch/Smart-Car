#pragma once

#include "Managers/Shader_Manager.h"
#include "Managers/Models_Manager.h"
#include "Core/Init/IListener.h"
#include "Core/EventHandler.h"
#include "Rendering/Camera.h"

namespace Managers {

	class Scene_Manager : public Core::IListener {

	public:
		Scene_Manager(int size_x, int size_y);
		~Scene_Manager();

		virtual void notifyBeginFrame();
		virtual void notifyDisplayFrame();
		virtual void notifyEndFrame();
		virtual void notifyReshape(int width, int height, int previous_width, int previous_height);
		virtual void keyDown(unsigned char key, int x, int y);
		virtual void keyUp(unsigned char key, int x, int y);

	private:
		Managers::Models_Manager* models_manager;
		Managers::Shader_Manager* shader_manager;

		bool fullscreen;
		double currentTime, lastTime, deltaTime;
		EventHandler* input;
		Camera* camera;
		Game_Logic* logic_routine;
	};
}
