#pragma once

#include "Rendering/Models/Model.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "SOIL.h"

namespace Rendering {

	namespace Models {

		class Car : public Model {

		public:
			Car();
			~Car();

			void Create();
			virtual void Update(float dt) override final;
			virtual void Draw() override final;

			//Render Stuff
			glm::mat4 viewMatrix, projectMatrix;
			GLuint modelID, viewID, projectID, texID;
			//Texture Stuff
			int tex_width, tex_height;
		};
	}
}
