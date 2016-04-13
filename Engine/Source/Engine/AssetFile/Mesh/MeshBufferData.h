#pragma once

#include <vector>

#include "MySTL/Release_self.h"

class MeshFileData;


class MeshBufferData{

public:
	MeshBufferData();
	~MeshBufferData();

	void Create(const MeshFileData* meshfile);

private:
	//�R�s�[�֎~
	MeshBufferData(const MeshBufferData&) = delete;
	MeshBufferData& operator=(const MeshBufferData&) = delete;

	//����͌��݃��f���o�b�t�@�[������Ă�
	//���㒼��
	Release_self<ID3D11Buffer> m_VertexBuffer;	// �o�b�t�@�[
	Release_self<ID3D11Buffer> m_IndexBuffer;	// �o�b�t�@�[

	struct Mesh{
		UINT m_IndexNum;	// ���_Index�̃f�[�^��
		UINT m_StartIndex;	// ���_Index�̃f�[�^�J�n�ʒu
	};

	std::vector<Mesh> m_Meshs;	// ���b�V���P�ʂ̒��_Index�f�[�^
	UINT m_Stride;	// ���_�f�[�^�̃T�C�Y

public:

	auto GetVertexBuffer()const->ID3D11Buffer*;//decltype(m_VertexBuffer.Get());
	auto GetIndexBuffer()const->ID3D11Buffer*;
	auto GetMesh()const->std::add_const<decltype(m_Meshs)>::type&;
	auto GetStride()const->std::add_const<decltype(m_Stride)>::type&;

};