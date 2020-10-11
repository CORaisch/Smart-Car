#pragma once

#include "Rendering/Models/Model.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

namespace Rendering {

	namespace Models {

		class Obstacle : public Model {

		public:
			Obstacle();
			~Obstacle();

			void Create();
			virtual void Update(float dt) override final;
			virtual void Draw() override final;

			//Render Stuff
			glm::mat4 viewMatrix, projectMatrix;
			GLuint modelID, viewID, projectID;
		};
	}
}
