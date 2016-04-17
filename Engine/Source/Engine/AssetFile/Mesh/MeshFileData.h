#pragma once
#include "../AssetFileData.h"

class IPolygonsData;

class MeshFileData : public AssetFileData{
public:
	MeshFileData();
	~MeshFileData();

	void Create(const char* filename);

	const IPolygonsData* GetPolygonsData() const;

private:
	//�R�s�[�֎~
	MeshFileData(const MeshFileData&) = delete;
	MeshFileData& operator=(const MeshFileData&) = delete;

	IPolygonsData* m_Polygons;	// ���_�ƃC���f�b�N�X�f�[�^
};
