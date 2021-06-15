#pragma once

#include <vector>

#include "MathLib/mat4.h"

#include "components/Mesh.h"
#include "components/Light.h"
#include "components/Transform.h"

namespace RNDR {

	class Renderer;

	class Scene {
	public:
		Scene() {};
		~Scene() {};
		Scene(Scene&) = delete;
		Scene& operator=(Scene&) = delete;

		size_t addMesh(const components::Mesh&, const components::Transform& transform = components::Transform());
		void removeMesh(size_t index);

		void setTransform(size_t index, const components::Transform& transform);
		components::Transform& getTransform(size_t index);

		void addLight(const components::Light&);
		components::Light& getLight();

	private:
		std::vector<components::Mesh> meshes;
		std::vector<components::Transform> transforms;

		components::Light light; // only one light is supported

		friend Renderer;
	};

}
