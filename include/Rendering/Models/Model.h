#pragma once

#include <vector>
#include "Rendering/IGameObject.h"

namespace Rendering {

	namespace Models {

		class Model :public IGameObject {

		public:
			Model();
			virtual ~Model();

			virtual void Draw() override;
			virtual void Update(float dt) override;
			virtual void SetProgram(GLuint shaderName) override;
			virtual GLuint GetProgram() override;
			virtual void Destroy() override;
			virtual void setEventHandler(EventHandler* eh) override;
			virtual void setCamera(Camera* cam) override;
			virtual std::vector<VertexFormat>& getVertices() override;
			virtual glm::mat4& getModelMatrix() override;
			virtual void setModelMatrix(const glm::mat4& model) override;

			virtual GLuint GetVao() const override;
			virtual const std::vector<GLuint>& GetVbos() const override;

		protected:
			GLuint vao;
			GLuint program;
			glm::mat4 model_matrix;
			std::vector<GLuint> vbos;
			std::vector<VertexFormat> vertices;
		};
	}
}
