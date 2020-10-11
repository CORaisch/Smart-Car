#pragma once

#include <map>
#include "Managers/Shader_Manager.h"
#include "Rendering/IGameObject.h"
#include "Rendering/Models/Car.h"
#include "Rendering/Models/Obstacle.h"
#include "Rendering/Camera.h"
#include "GameLogic/Game_Logic.h"

using namespace Rendering;

namespace Managers {

	class Models_Manager {

	public:
		Models_Manager(EventHandler* eh, Camera* cam);
		~Models_Manager();

		void Draw();
		void Update(float dt);
		void DeleteModel(const std::string& gameModelName);
		const IGameObject& GetModel(const std::string& gameModelName) const;
		std::map<std::string, IGameObject*>& getModelList();

	private:
		std::map<std::string, IGameObject*> gameModelList;
	};
}
