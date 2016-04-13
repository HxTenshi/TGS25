#pragma once

#include "MySTL/ptr.h"

enum class AssetFileType{
	None,
	Temesh,
	Tebone,
	Vmd,
	Image,
	Count,
};

class AssetData{
public:

	AssetData(AssetFileType type = AssetFileType::None)
		:m_AssetFileType(type){

	}

	const AssetFileType m_AssetFileType;

protected:
	void* m_Data;
	char m_GUID[33];

private:
	void operator = (const AssetData&);
	AssetData(const AssetData&);

};
using AssetDataPtr = shared_ptr < AssetData > ;



#include <functional>
#include <map>
class AssetFactory{
public:
	static AssetDataPtr Create(const char* filename);
private:
	static std::map<std::string, std::function<AssetDataPtr(const char*)>> m_Factory;
protected:
	AssetFactory();
};

class AssetDataBase{
public:
	
	template <class T>
	static void Instance(const char* filename, shared_ptr<T>& out){

		auto file = m_AssetCache.find(filename);
		AssetDataPtr data;
		if (file == m_AssetCache.end()){

			data = AssetFactory::Create(filename);
			m_AssetCache.insert(std::make_pair(filename, data));
		}
		else{
			data = file->second;
		}
		
		if (data && T::_AssetFileType == data->m_AssetFileType){
			out = data;
		}
	}

private:

	static std::map<std::string, AssetDataPtr> m_AssetCache;
};




#include "Engine/AssetFile/Mesh/MeshFileData.h"
#include "Engine/AssetFile/Mesh/MeshBufferData.h"

class MeshAssetData : public AssetData{
public:
	~MeshAssetData();

	static AssetDataPtr MeshAssetData::Create(const char* filename);

	const MeshFileData& GetFileData() const;
	const MeshBufferData& GetBufferData() const;

	static const AssetFileType _AssetFileType = AssetFileType::Temesh;

private:

	MeshAssetData();


	//�R�s�[�֎~
	MeshAssetData(const MeshAssetData&) = delete;
	MeshAssetData& operator=(const MeshAssetData&) = delete;

	MeshFileData	m_MeshFileData;		// �t�@�C����ǂݍ��񂾃f�[�^
	MeshBufferData	m_MeshBufferData;	// �`��Ɏg�p����f�[�^

};

using MeshAssetDataPtr = shared_ptr < MeshAssetData >;


#include "Engine/AssetFile/Bone/BoneFileData.h"

class BoneAssetData : public AssetData{
public:
	~BoneAssetData();

	static AssetDataPtr BoneAssetData::Create(const char* filename);

	const BoneFileData& GetFileData() const;

	static const AssetFileType _AssetFileType = AssetFileType::Tebone;

private:

	BoneAssetData();


	//�R�s�[�֎~
	BoneAssetData(const BoneAssetData&) = delete;
	BoneAssetData& operator=(const BoneAssetData&) = delete;

	BoneFileData	m_BoneFileData;		// �t�@�C����ǂݍ��񂾃f�[�^

};

using BoneAssetDataPtr = shared_ptr < BoneAssetData >;