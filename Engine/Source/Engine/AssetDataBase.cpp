#include "AssetDataBase.h"

#include "Device/DirectX11Device.h"


#include "MySTL/Utility.h"

#include "Window/Window.h"

#include "AssetFile\Bone\BoneFileData.h"
#include "AssetFile\Mesh\MeshFileData.h"
#include "AssetFile\Prefab\PrefabFileData.h"
#include "AssetFile\Shader\ShaderFileData.h"
#include "AssetFile\Material\TextureFileData.h"

decltype(AssetFactory::m_Factory) AssetFactory::m_Factory;
class __AssetFactory :public AssetFactory{
public: __AssetFactory(){}
};

static __AssetFactory factory;

AssetFactory::AssetFactory(){
#ifndef _ASSET_TEMP
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataPtr(const char*)>>(std::string("tesmesh"), [](const char* filename){ return MeshAssetData::Create(filename); }));
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataPtr(const char*)>>(std::string("tedmesh"), [](const char* filename){ return MeshAssetData::Create(filename); }));
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataPtr(const char*)>>(std::string("tebone"), [](const char* filename){ return BoneAssetData::Create(filename); }));
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataPtr(const char*)>>(std::string("json"), [](const char* filename){ return PrefabAssetData::Create(filename); }));
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataPtr(const char*)>>(std::string("fx"), [](const char* filename){ return ShaderAssetData::Create(filename); }));
#else
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("tesmesh"), [](const char* filename){ return AssetDataTemplate<MeshFileData>::Create(filename); }));
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("tedmesh"), [](const char* filename){ return AssetDataTemplate<MeshFileData>::Create(filename); }));

	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("tebone"), [](const char* filename){ return AssetDataTemplate<BoneFileData>::Create(filename); }));

	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("json"), [](const char* filename){ return AssetDataTemplate<PrefabFileData>::Create(filename); }));

	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("fx"), [](const char* filename){ return AssetDataTemplate<ShaderFileData>::Create(filename); }));

	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("png"), [](const char* filename){ return AssetDataTemplate<TextureFileData>::Create(filename); }));
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("jpg"), [](const char* filename){ return AssetDataTemplate<TextureFileData>::Create(filename); }));
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("jpge"), [](const char* filename){ return AssetDataTemplate<TextureFileData>::Create(filename); }));
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("bmp"), [](const char* filename){ return AssetDataTemplate<TextureFileData>::Create(filename); }));
	m_Factory.insert(std::make_pair<std::string, std::function<AssetDataTemplatePtr(const char*)>>(std::string("tif"), [](const char* filename){ return AssetDataTemplate<TextureFileData>::Create(filename); }));
#endif
}
#ifndef _ASSET_TEMP

//static
AssetDataPtr AssetFactory::Create(const char* filename){

	std::string str = filename;
	
	auto type = behind_than_find_last_of(str, ".");
	if (type == "") return NULL;

	auto make = m_Factory.find(type);
	if (make == m_Factory.end())return NULL;

	return make->second(filename);

}

#else
AssetDataTemplatePtr AssetFactory::Create(const char* filename){

	std::string str = filename;

	auto type = behind_than_find_last_of(str, ".");
	if (type == "") return NULL;

	auto make = m_Factory.find(type);
	if (make == m_Factory.end())return NULL;

	return make->second(filename);

}

#endif

//static
decltype(AssetDataBase::m_AssetCache) AssetDataBase::m_AssetCache;


#ifndef _ASSET_TEMP


MeshAssetData::~MeshAssetData()
{
}

MeshAssetData::MeshAssetData()
	:AssetData(_AssetFileType)
{
}

//static
AssetDataPtr MeshAssetData::Create(const char* filename)
{
	
	//auto temp = make_shared<MeshAssetData>();

	struct PrivateFactory : public  MeshAssetData{
		PrivateFactory() : MeshAssetData(){}
	};
	auto temp = make_shared<PrivateFactory>();

	temp->m_MeshFileData.Create(filename);
	if (!temp->m_MeshFileData.GetPolygonsData())return NULL;
	temp->m_MeshBufferData.Create(&temp->m_MeshFileData);
	if (!temp->m_MeshBufferData.GetVertexBuffer())return NULL;

	return temp;
}
void MeshAssetData::FileUpdate()
{
	m_MeshFileData.FileUpdate();
	m_MeshBufferData.Create(&m_MeshFileData);
}

const MeshFileData& MeshAssetData::GetFileData() const{
	return m_MeshFileData;
}
const MeshBufferData& MeshAssetData::GetBufferData() const{
	return m_MeshBufferData;
}










BoneAssetData::~BoneAssetData()
{
}

BoneAssetData::BoneAssetData()
	:AssetData(_AssetFileType)
{
}

//static
AssetDataPtr BoneAssetData::Create(const char* filename)
{
	//auto temp = make_shared<BoneAssetData>();

	struct PrivateFactory : public  BoneAssetData{
		PrivateFactory() : BoneAssetData(){}
	};
	auto temp = make_shared<PrivateFactory>();

	temp->m_BoneFileData.Create(filename);

	return temp;
}

void BoneAssetData::FileUpdate()
{
	m_BoneFileData.FileUpdate();
}


const BoneFileData& BoneAssetData::GetFileData() const{
	return m_BoneFileData;
}



PrefabAssetData::PrefabAssetData()
	:AssetData(_AssetFileType)
{
}
PrefabAssetData::~PrefabAssetData()
{
}


//static
AssetDataPtr PrefabAssetData::Create(const char* filename)
{

	struct PrivateFactory : public  PrefabAssetData{
		PrivateFactory() : PrefabAssetData(){}
	};
	auto temp = make_shared<PrivateFactory>();

	temp->m_PrefabFileData.Create(filename);

	return temp;
}
void PrefabAssetData::FileUpdate()
{
	m_PrefabFileData.FileUpdate();
}

const PrefabFileData& PrefabAssetData::GetFileData() const{
	return m_PrefabFileData;
}

#include "Game/Game.h"
#include "Game/Actor.h"
void PrefabAssetData::CreateInspector(){
	m_PrefabFileData.GetActor()->CreateInspector();

	auto data = Window::CreateInspector();
	std::function<void()> collback = [&](){
		auto before = m_PrefabFileData.Apply();

		Game::GetAllObject([&](Actor* tar){
			auto str = tar->Prefab();
			if (m_PrefabFileData.GetFileName() == str){

				tar->PastePrefabParam(before);

			}
		});
	};
	Window::AddInspector(new InspectorButtonDataSet("Apply", collback), data);

	Window::ViewInspector("Prefab", NULL, data);
}


ShaderAssetData::ShaderAssetData()
	:AssetData(_AssetFileType)
{
}
ShaderAssetData::~ShaderAssetData()
{
}


//static
AssetDataPtr ShaderAssetData::Create(const char* filename)
{

	struct PrivateFactory : public  ShaderAssetData{
		PrivateFactory() : ShaderAssetData(){}
	};
	auto temp = make_shared<PrivateFactory>();

	temp->m_FileData.Create(filename);

	return temp;
}
void ShaderAssetData::FileUpdate()
{
	m_FileData.FileUpdate();
}

const ShaderFileData& ShaderAssetData::GetFileData() const{
	return m_FileData;
}

void ShaderAssetData::CreateInspector(){

	auto data = Window::CreateInspector();
	std::function<void()> collback = [&](){
		m_FileData.FileUpdate();

	};
	Window::AddInspector(new InspectorButtonDataSet("Compile", collback), data);

	Window::ViewInspector("Shader", NULL, data);
}


#else
#endif

void AssetDataTemplate<MeshFileData>::CreateInspector(){

}

void AssetDataTemplate<BoneFileData>::CreateInspector(){

}

#include "Game/Game.h"
void AssetDataTemplate<PrefabFileData>::CreateInspector(){
	m_FileData->GetActor()->CreateInspector();

	auto data = Window::CreateInspector();
	std::function<void()> collback = [&](){
		auto before = m_FileData->Apply();

		Game::GetAllObject([&](Actor* tar){
			auto str = tar->Prefab();
			if (m_FileData->GetFileName() == str){

				tar->PastePrefabParam(before);

			}
		});
	};
	Window::AddInspector(new InspectorButtonDataSet("Apply", collback), data);

	Window::ViewInspector("Prefab", NULL, data);
}

void AssetDataTemplate<ShaderFileData>::CreateInspector(){

	auto data = Window::CreateInspector();
	std::function<void()> collback = [&](){
		m_FileData->FileUpdate();

	};
	Window::AddInspector(new InspectorButtonDataSet("Compile", collback), data);

	Window::ViewInspector("Shader", NULL, data);
}