#pragma once
#include "../AssetFileData.h"
#include"MeshBufferData.h"


class IPolygonsData;

class MeshFileData : public AssetFileData{
public:
	MeshFileData();
	~MeshFileData();

	bool Create(const char* filename) override;
	bool FileUpdate() override;

	const MeshBufferData& GetBufferData() const;

	const IPolygonsData* GetPolygonsData() const;

private:
	//コピー禁止
	MeshFileData(const MeshFileData&) = delete;
	MeshFileData& operator=(const MeshFileData&) = delete;

	IPolygonsData* m_Polygons;	// 頂点とインデックスデータ
	MeshBufferData m_MeshBufferData;
};
