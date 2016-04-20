#pragma once

#define _ASSET_TEMP

#include "MySTL/ptr.h"

enum class AssetFileType{
	None,
	Prefab,
	Temesh,
	Tebone,
	Shader,
	Vmd,
	Image,
	Count,
};

#ifndef _ASSET_TEMP

class AssetData{
public:

	AssetData(AssetFileType type = AssetFileType::None)
		:m_AssetFileType(type){

	}
	virtual ~AssetData(){}
	virtual void CreateInspector(){}
	virtual void FileUpdate() = 0;

	const AssetFileType m_AssetFileType;

	static const AssetFileType _AssetFileType = AssetFileType::None;


protected:
	void* m_Data;
	char m_GUID[33];

private:
	void operator = (const AssetData&);
	AssetData(const AssetData&);

};
using AssetDataPtr = shared_ptr < AssetData > ;

#else
class IAssetDataTemplate{
public:
	IAssetDataTemplate(AssetFileType type = AssetFileType::None)
		:m_AssetFileType(type){
	}
	virtual ~IAssetDataTemplate(){}
	virtual void CreateInspector() = 0;
	virtual void FileUpdate() = 0;

	static const AssetFileType _AssetFileType = AssetFileType::None;
	const AssetFileType m_AssetFileType;
};

using AssetDataTemplatePtr = shared_ptr < IAssetDataTemplate >;
#endif

#ifndef _ASSET_TEMP
#else
#endif

#include <functional>
#include <map>
class AssetFactory{
public:
#ifndef _ASSET_TEMP
	static AssetDataPtr Create(const char* filename);
#else
	static AssetDataTemplatePtr Create(const char* filename);
#endif
private:
#ifndef _ASSET_TEMP
	static std::map<std::string, std::function<AssetDataPtr(const char*)>> m_Factory;
#else
	static std::map<std::string, std::function<AssetDataTemplatePtr(const char*)>> m_Factory;
#endif
protected:
	AssetFactory();
};

class AssetDataBase{
public:
	
	template <class T>
	static void Instance(const char* filename, shared_ptr<T>& out){

		auto file = m_AssetCache.find(filename);

#ifndef _ASSET_TEMP
		AssetDataPtr data;
#else
		AssetDataTemplatePtr data;
#endif
		if (file == m_AssetCache.end()){

			data = AssetFactory::Create(filename);
			if (data){
				m_AssetCache.insert(std::make_pair(filename, data));
			}
		}
		else{
			data = file->second;
		}
		
		if (data && 
			(T::_AssetFileType == data->m_AssetFileType) ||
			T::_AssetFileType == AssetFileType::None){
			out = data;
		}
	}

	static void FileUpdate(const char* filename){

		auto file = m_AssetCache.find(filename);
		if (file != m_AssetCache.end()){

			file->second->FileUpdate();
		}
		else{
#ifndef _ASSET_TEMP
			shared_ptr<AssetData> temp;
#else
			AssetDataTemplatePtr temp;
#endif
			Instance(filename,temp);
		}
	}

	static void CreateInspector(const char* filename){
#ifndef _ASSET_TEMP
		shared_ptr<AssetData> temp;
#else
		AssetDataTemplatePtr temp;
#endif
		Instance(filename, temp);
		if (temp){
			temp->CreateInspector();
		}
	}


private:
#ifndef _ASSET_TEMP
	static std::map<std::string, AssetDataPtr> m_AssetCache;
#else
	static std::map<std::string, AssetDataTemplatePtr> m_AssetCache;
#endif
};


#ifndef _ASSET_TEMP

#include "Engine/AssetFile/Mesh/MeshFileData.h"
#include "Engine/AssetFile/Mesh/MeshBufferData.h"

class MeshAssetData : public AssetData{
public:
	virtual ~MeshAssetData();

	static AssetDataPtr MeshAssetData::Create(const char* filename);
	void FileUpdate()override;

	const MeshFileData& GetFileData() const;
	const MeshBufferData& GetBufferData() const;

	static const AssetFileType _AssetFileType = AssetFileType::Temesh;

private:

	MeshAssetData();


	//コピー禁止
	MeshAssetData(const MeshAssetData&) = delete;
	MeshAssetData& operator=(const MeshAssetData&) = delete;

	MeshFileData	m_MeshFileData;		// ファイルを読み込んだデータ
	MeshBufferData	m_MeshBufferData;	// 描画に使用するデータ

};

using MeshAssetDataPtr = shared_ptr < MeshAssetData >;


#include "Engine/AssetFile/Bone/BoneFileData.h"

class BoneAssetData : public AssetData{
public:
	virtual ~BoneAssetData();

	static AssetDataPtr BoneAssetData::Create(const char* filename);
	void FileUpdate()override;

	const BoneFileData& GetFileData() const;

	static const AssetFileType _AssetFileType = AssetFileType::Tebone;

private:

	BoneAssetData();


	//コピー禁止
	BoneAssetData(const BoneAssetData&) = delete;
	BoneAssetData& operator=(const BoneAssetData&) = delete;

	BoneFileData	m_BoneFileData;		// ファイルを読み込んだデータ

};

using BoneAssetDataPtr = shared_ptr < BoneAssetData >;


#include "AssetFile/Prefab/PrefabFileData.h"

class PrefabAssetData : public AssetData{
public:
	virtual ~PrefabAssetData();

	static AssetDataPtr PrefabAssetData::Create(const char* filename);

	void FileUpdate()override;

	const PrefabFileData& GetFileData() const;

	void CreateInspector() override;

	static const AssetFileType _AssetFileType = AssetFileType::Prefab;

private:

	PrefabAssetData();


	//コピー禁止
	PrefabAssetData(const PrefabAssetData&) = delete;
	PrefabAssetData& operator=(const PrefabAssetData&) = delete;

	PrefabFileData	m_PrefabFileData;		// ファイルを読み込んだデータ

};

using PrefabAssetDataPtr = shared_ptr < PrefabAssetData >;


#include "AssetFile/Shader/ShaderFileData.h"

class ShaderAssetData : public AssetData{
public:
	virtual ~ShaderAssetData();

	static AssetDataPtr ShaderAssetData::Create(const char* filename);

	void FileUpdate()override;

	const ShaderFileData& GetFileData() const;

	void CreateInspector() override;

	static const AssetFileType _AssetFileType = AssetFileType::Shader;

private:

	ShaderAssetData();


	//コピー禁止
	ShaderAssetData(const ShaderAssetData&) = delete;
	ShaderAssetData& operator=(const ShaderAssetData&) = delete;

	ShaderFileData	m_FileData;		// ファイルを読み込んだデータ

};

using ShaderAssetDataPtr = shared_ptr < ShaderAssetData >;


#endif


template<class T>
class AssetDataTemplate : public IAssetDataTemplate {
public:

	AssetDataTemplate()
		:IAssetDataTemplate(_AssetFileType){
	}

	static AssetDataTemplatePtr Create(const char* filename);
	static AssetDataTemplatePtr Create(T* fileData);

	virtual ~AssetDataTemplate(){}
	void CreateInspector(){}
	void FileUpdate();

	const T* GetFileData();

	static const AssetFileType _AssetFileType;


protected:
	T* m_FileData;

private:
	AssetDataTemplate<T>& operator = (const AssetDataTemplate<T>&) = delete;
	AssetDataTemplate<T>(const AssetDataTemplate<T>&) = delete;

};

class MeshFileData;
using MeshAssetDataPtr = shared_ptr < AssetDataTemplate<MeshFileData> >;
const AssetFileType AssetDataTemplate<MeshFileData>::_AssetFileType = AssetFileType::Temesh;
void AssetDataTemplate<MeshFileData>::CreateInspector();

class BoneFileData;
using BoneAssetDataPtr = shared_ptr < AssetDataTemplate<BoneFileData> >;
const AssetFileType AssetDataTemplate<BoneFileData>::_AssetFileType = AssetFileType::Tebone;
void AssetDataTemplate<BoneFileData>::CreateInspector();

class PrefabFileData;
using PrefabAssetDataPtr = shared_ptr < AssetDataTemplate<PrefabFileData> >;
const AssetFileType AssetDataTemplate<PrefabFileData>::_AssetFileType = AssetFileType::Prefab;
void AssetDataTemplate<PrefabFileData>::CreateInspector();

class ShaderFileData;
using ShaderAssetDataPtr = shared_ptr < AssetDataTemplate<ShaderFileData> >;
const AssetFileType AssetDataTemplate<ShaderFileData>::_AssetFileType = AssetFileType::Shader;
void AssetDataTemplate<ShaderFileData>::CreateInspector();

class TextureFileData;
using TextureAssetDataPtr = shared_ptr < AssetDataTemplate<TextureFileData> >;
const AssetFileType AssetDataTemplate<TextureFileData>::_AssetFileType = AssetFileType::Image;
void AssetDataTemplate<TextureFileData>::CreateInspector(){}

template <class T>
const AssetFileType AssetDataTemplate<T>::_AssetFileType = AssetFileType::None;

#include "details.h"