#pragma once

class IPolygonsData;

class MeshFileData{
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
