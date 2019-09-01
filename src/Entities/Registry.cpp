#include "Registry.h"

#include <Game.h>
#include <3D/SkinnedModel.h>
#include <3D/MeshPack.h>
#include <3D/Camera.h>
#include <Graphics/ShaderManager.h>
#include <Entities/Components/Transform.h>
#include <Entities/Components/Model.h>
#include <Entities/Components/Villager.h>

namespace OpenBlack::Entities
{
void Registry::DebugCreateEntities(float x, float y, float z)
{
	auto entity = _registry.create();
	_registry.assign<Transform>(entity, x, y, z, 2.5f, 180.0f, 111.0f, 0.0f);
	uint32_t meshId = 0;
	_registry.assign<Model>(entity, meshId);
	uint32_t health = 100;
	uint32_t age = 18;
	uint32_t hunger = 100;
	_registry.assign<Villager>(entity, health, age, hunger);
}

void Registry::DrawModels(const Camera& camera, Graphics::ShaderManager& shaderManager)
{
	auto& meshPack = Game::instance()->GetMeshPack();
	auto& view = _registry.view<Model, Transform>();
	const auto projectionMatrix = camera.GetViewProjectionMatrix();
	
	for (auto entity : view)
	{
		auto& model = view.get<Model>(entity);
		auto& position = view.get<Transform>(entity);
	
		auto _modelPosition = glm::vec3(position.x + model.xOffset, position.y + model.yOffset, position.z + model.zOffset);
		auto _modelRotation = glm::vec3(position.rotX, position.rotY, position.rotZ);
		auto _modelScale    = glm::vec3(position.scale);
	
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix           = glm::translate(modelMatrix, _modelPosition);
	
		modelMatrix = glm::rotate(modelMatrix, _modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, _modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, _modelRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	
		modelMatrix = glm::scale(modelMatrix, _modelScale);
	
		Graphics::ShaderProgram* objectShader = shaderManager.GetShader("SkinnedMesh");
		objectShader->Bind();
		objectShader->SetUniformValue("u_viewProjection", projectionMatrix);
		objectShader->SetUniformValue("u_modelTransform", modelMatrix);

		// Waiting on MeshPack implementation
		// const SkinnedModel& modelGeometry = meshPack.GetModel(model.meshId);
		// modelGeometry.Draw(objectShader);
	}
}

void Registry::Update()
{
}
} // namespace OpenBlack::Entities
