
#include "ComponentFactory.h"

#include "TransformComponent.h"
#include "PhysXComponent.h"
#include "PhysXColliderComponent.h"
#include "TextComponent.h"
#include "CameraComponent.h"
#include "ModelComponent.h"
#include "TextureModelComponent.h"
#include "AnimationComponent.h"
#include "MeshComponent.h"
#include "MeshDrawComponent.h"
#include "ScriptComponent.h"
#include "DirectionalLightComponent.h"
#include "PointLightComponent.h"
#include "PostEffectComponent.h"
#include "ParticleComponent.h"
#include "SoundComponent.h"
#include "MovieComponent.h"
#include "Component.h"



decltype(ComponentFactory::mFactoryComponents) ComponentFactory::mFactoryComponents;

class __ComponentFactory : public ComponentFactory{};
__ComponentFactory initialize;

ComponentFactory::ComponentFactory(){
	_NewFunc<TransformComponent>();
	_NewFunc<TextureModelComponent>();
	_NewFunc<TextComponent>();
	_NewFunc<ModelComponent>();
	_NewFunc<MeshComponent>();
	_NewFunc<MeshDrawComponent>();
	_NewFunc<MaterialComponent>();
	_NewFunc<AnimationComponent>();
	_NewFunc<CameraComponent>();
	_NewFunc<DirectionalLightComponent>();
	_NewFunc<PointLightComponent>();
	_NewFunc<ParticleComponent>();
	_NewFunc<ScriptComponent>();
	_NewFunc<PhysXComponent>();
	_NewFunc<PhysXColliderComponent>();
	_NewFunc<SoundComponent>();
	_NewFunc<MovieComponent>();
	_NewFunc<PostEffectComponent>();
}

shared_ptr<Component> ComponentFactory::Create(const std::string& ClassName){
	return _Create(("class " + ClassName));

}

std::unordered_map<std::string, std::function<shared_ptr<Component>()>>& ComponentFactory::GetComponents(){
	return mFactoryComponents;
}

shared_ptr<Component> ComponentFactory::_Create(std::string ClassName){
	auto p = mFactoryComponents.find(ClassName);
	if (p != mFactoryComponents.end()){
		return p->second();
	}
	_SYSTEM_LOG_ERROR("�R���|�[�l���g[" + ClassName+"]�̍쐬");
	return NULL;
}
