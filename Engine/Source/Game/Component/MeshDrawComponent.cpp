
#include "MeshDrawComponent.h"

#include "Window/Window.h"
#include "Game/Actor.h"
#include "ModelComponent.h"
#include "TextureModelComponent.h"
#include "MaterialComponent.h"

#include "Graphic/Model/Model.h"

#include "Game/Game.h"

MeshDrawComponent::MeshDrawComponent(){
}

void MeshDrawComponent::EngineUpdate(){
	Update();
}

void MeshDrawComponent::Update(){

	if (!mModel){
		mModel = gameObject->GetComponent<ModelComponent>();
		if (!mModel)
			mModel = gameObject->GetComponent<TextureModelComponent>();
		if (!mModel)return;
	}
	if (!mModel->mModel)return;

	if (!mMaterial){
		mMaterial = gameObject->GetComponent<MaterialComponent>();
	}
	if (!mMaterial)return;
	Game::AddDrawList(DrawStage::Init, std::function<void()>([&](){
		mModel->SetMatrix();
	}));
	Game::AddDrawList(DrawStage::Diffuse, std::function<void()>([&](){
		Model& model = *mModel->mModel;

		model.Draw(mMaterial);
	}));

}

void MeshDrawComponent::CreateInspector(){
	auto data = Window::CreateInspector();
	Window::ViewInspector("MeshRender", this, data);
}

void MeshDrawComponent::IO_Data(I_ioHelper* io){
	(void)io;
#define _KEY(x) io->func( x , #x)

#undef _KEY
}