#include "Components/MeshRenderer.hpp"

#include "Modules/RenderModule.hpp"
#include "Modules/Window.hpp"

#include "Render/Renderer.hpp"
#include "Components/Transform.hpp"

#include "Resources/ResourceManager.hpp"

#include "Resources/Model.hpp"



Transform::ComponentRegistrer<Transform> Transform::componentRegistrer; // for some reason Transform.cpp wouldn t cooperate had to be pragmatic
MeshRenderer::ComponentRegistrer<MeshRenderer> MeshRenderer::componentRegistrer;


MeshRenderer::MeshRenderer() {
	shader = RenderModule::GetInstance()->CreateShader();
	shader->Initialize(Engine::GetModule<Window>());

}

void MeshRenderer::Render() const {

	if (model == nullptr) { return; }
	model->Render(shader, Transform()->GetMatrix());
}

void MeshRenderer::OnDestroy() {
	if (shader != nullptr) {
		delete shader;
		shader = nullptr;
	}
}

void MeshRenderer::LoadModel(const std::string& fileName_) {
	fileName = fileName_;
	model = ResourceManager::Load<Model>(fileName);
}

std::vector<Mesh*> MeshRenderer::GetMeshes() const {
	return model->GetMeshes();
}
