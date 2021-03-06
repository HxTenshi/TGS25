
#include <functional>
#include "MaterialComponent.h"

#include "Window/Window.h"

#include"Graphic/Material/Material.h"

void MaterialComponent::LoadAssetResource(const std::string& path){

	File f(path);
	if (!f)return;

	mMaterials.clear();

	std::string name;
	f.In(&name);
	int materialnum;
	f.In(&materialnum);
	mMaterials.resize(materialnum);
	for (int i = 0; i < materialnum; i++){
		auto& material = mMaterials[i];
		auto hr = material.Create();
		if (FAILED(hr))return;

		int texnum;
		f.In(&texnum);
		for (int i = 0; i < texnum; i++){
			int slot;
			f.In(&slot);
			std::string filename;
			f.In(&filename);
			auto ioc = filename.find("$");
			while (std::string::npos != ioc){
				filename.replace(ioc, 1, " ");
				ioc = filename.find("$");
			}

			material.SetTexture(filename.c_str(), slot);
		}
	}
}
void MaterialComponent::SaveAssetResource(const std::string& path){

	File f;
	if (!f.Open(path))
		f.FileCreate();
	f.Clear();
	if (!f)return;
	std::string name = "Material";
	f.Out(name);
	int num = mMaterials.size();
	f.Out(num);
	for (int i = 0; i < num; i++){
		Material &mate = mMaterials[i];
		mate.ExportData(f);
	}
}

MaterialComponent::MaterialComponent(){
	mMaterials.resize(1);
	mAlbedo = XMFLOAT4(1, 1, 1, 1);
	mSpecular = XMFLOAT4(0, 0, 0, 1);
	mTexScale = XMFLOAT2(1, 1);
	mHeightPower = XMFLOAT2(2.0f, 1);
}
MaterialComponent::~MaterialComponent(){
}

void MaterialComponent::Initialize(){
	auto& mate = mMaterials[0];
	if (!mate.IsCreate()){
		if (mMaterialPath != ""){
			LoadAssetResource(mMaterialPath);
			for (auto& m : mMaterials){
				if (!m.IsCreate()){
					m.Create();
				}
			}
		}
		else{
			auto& m = mMaterials[0];
			m.Create();
		}
	}

	SetAlbedoColor(mAlbedo);
	SetSpecularColor(mSpecular);
	mMaterials[0].mCBMaterial.mParam.TexScale = mTexScale;
	mMaterials[0].mCBMaterial.mParam.HeightPower = mHeightPower;

	if (mAlbedoTextureName != "")mMaterials[0].SetTexture(mAlbedoTextureName.c_str(), 0);
	if (mNormalTextureName != "")mMaterials[0].SetTexture(mNormalTextureName.c_str(), 1);
	if (mHeightTextureName != "")mMaterials[0].SetTexture(mHeightTextureName.c_str(), 2);
}

void MaterialComponent::SetMaterial(UINT SetNo, Material& material){
	if (mMaterials.size() <= SetNo)mMaterials.resize(SetNo + 1);
	mMaterials[SetNo] = material;

}
Material MaterialComponent::GetMaterial(UINT GetNo) const{
	if (mMaterials.size() <= GetNo){
		return Material();
	}
	return mMaterials[GetNo];
}
void MaterialComponent::CreateInspector(){

	auto data = Window::CreateInspector();
	std::function<void(float)> collbackx = [&](float f){
		mAlbedo.x = f;
		SetAlbedoColor(mAlbedo);
	};

	std::function<void(float)> collbacky = [&](float f){
		mAlbedo.y = f;
		SetAlbedoColor(mAlbedo);
	};

	std::function<void(float)> collbackz = [&](float f){
		mAlbedo.z = f;
		SetAlbedoColor(mAlbedo);
	};

	std::function<void(float)> collbacka = [&](float f){
		mAlbedo.w = f;
		SetAlbedoColor(mAlbedo);
	};

	std::function<void(float)> collbackxs = [&](float f){
		mSpecular.x = f;
		SetSpecularColor(mSpecular);
	};

	std::function<void(float)> collbackys = [&](float f){
		mSpecular.y = f;
		SetSpecularColor(mSpecular);
	};

	std::function<void(float)> collbackzs = [&](float f){
		mSpecular.z = f;
		SetSpecularColor(mSpecular);
	};

	std::function<void(float)> collbackas = [&](float f){
		mSpecular.w = f;
		SetSpecularColor(mSpecular);
	};

	std::function<void(std::string)> collbacktex = [&](std::string name){
		mMaterials[0].SetTexture(name.c_str(), 0);
		mAlbedoTextureName = name;
	};
	std::function<void(std::string)> collbackntex = [&](std::string name){
		mMaterials[0].SetTexture(name.c_str(), 1);
		mNormalTextureName = name;
	};
	std::function<void(std::string)> collbackhtex = [&](std::string name){
		mMaterials[0].SetTexture(name.c_str(), 2);
		mHeightTextureName = name;
	};
	std::function<void(std::string)> collbackpath = [&](std::string name){
		mMaterialPath = name;
		LoadAssetResource(mMaterialPath);
		//TextureName = (**mppMaterials)[0].mTexture[0].mFileName;
	};
	std::function<void(std::string)> collbacksha = [&](std::string name){
		mShaderName = name;
		mMaterials[0].CreateShader(mShaderName.c_str());
	};

	std::function<void(float)> collbacktexsx = [&](float f){
		mTexScale.x = f;
		mMaterials[0].mCBMaterial.mParam.TexScale = mTexScale;
	};

	std::function<void(float)> collbacktexsy = [&](float f){
		mTexScale.y = f;
		mMaterials[0].mCBMaterial.mParam.TexScale = mTexScale;
	};
	std::function<void(float)> collbackH = [&](float f){
		mHeightPower.x = f;
		mMaterials[0].mCBMaterial.mParam.HeightPower = mHeightPower;
	};

	Window::AddInspector(new TemplateInspectorDataSet<std::string>("Material", &mMaterialPath, collbackpath), data);
	Window::AddInspector(new InspectorColorDataSet("Albedo", &mAlbedo.x, collbackx, &mAlbedo.y, collbacky, &mAlbedo.z, collbackz, &mAlbedo.w, collbacka), data);
	Window::AddInspector(new InspectorColorDataSet("Specular", &mSpecular.x, collbackxs, &mSpecular.y, collbackys, &mSpecular.z, collbackzs, NULL, [](float){}), data);
	Window::AddInspector(new InspectorSlideBarDataSet("Roughness",0,1,&mSpecular.w, collbackas), data);
	//Window::AddInspector(new InspectorSlideBarDataSet("r", 0.0f, 1.0f, &mAlbedo.x, collbackx), data);
	//Window::AddInspector(new InspectorSlideBarDataSet("g", 0.0f, 1.0f, &mAlbedo.y, collbacky), data);
	//Window::AddInspector(new InspectorSlideBarDataSet("b", 0.0f, 1.0f, &mAlbedo.z, collbackz), data);
	Window::AddInspector(new TemplateInspectorDataSet<std::string>("AlbedoTextre", &mAlbedoTextureName, collbacktex), data);
	Window::AddInspector(new TemplateInspectorDataSet<std::string>("NormalTextre", &mNormalTextureName, collbackntex), data);
	Window::AddInspector(new TemplateInspectorDataSet<std::string>("HeightTextre", &mHeightTextureName, collbackhtex), data);
	Window::AddInspector(new InspectorVector2DataSet("TextureScale", &mTexScale.x, collbacktexsx, &mTexScale.y, collbacktexsy), data);
	Window::AddInspector(new InspectorSlideBarDataSet("HightPower", -10, 10, &mHeightPower.x, collbackH), data);
	Window::AddInspector(new TemplateInspectorDataSet<std::string>("Shader", &mShaderName, collbacksha), data);
	Window::ViewInspector("Material", this, data);
}

void MaterialComponent::SetAlbedoColor(const XMFLOAT4& col){
	mAlbedo = col;
	mMaterials[0].mCBMaterial.mParam.Diffuse = mAlbedo;
}
void MaterialComponent::SetSpecularColor(const XMFLOAT4& col){
	mSpecular = col;
	mMaterials[0].mCBMaterial.mParam.Specular = mSpecular;
}

void MaterialComponent::IO_Data(I_ioHelper* io){
#define _KEY(x) io->func( x , #x)
	_KEY(mMaterialPath);
	_KEY(mAlbedo.x);
	_KEY(mAlbedo.y);
	_KEY(mAlbedo.z);
	_KEY(mAlbedo.w);
	_KEY(mSpecular.x);
	_KEY(mSpecular.y);
	_KEY(mSpecular.z);
	_KEY(mSpecular.w);
	_KEY(mAlbedoTextureName);
	_KEY(mNormalTextureName);
	_KEY(mHeightTextureName);
	_KEY(mTexScale.x);
	_KEY(mTexScale.y);
	_KEY(mHeightPower.x);
	_KEY(mHeightPower.y);
#undef _KEY
}