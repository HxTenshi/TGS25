#pragma once

class AssetFileData{
public:
	AssetFileData(){}
	virtual ~AssetFileData(){}

private:
	//�R�s�[�֎~
	AssetFileData(const AssetFileData&) = delete;
	AssetFileData& operator=(const AssetFileData&) = delete;
};
