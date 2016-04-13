
#include "PMD.h"
#include <windows.h>
#include <stdio.h>

//�R���X�g���N�^
pmd::pmd(const char *filename)
{
	FILE *hFP;
	hFP = fopen(filename, "rb");
	if (hFP != 0){
		//�w�b�_�[�ǂݍ���
		fread(&header, sizeof(struct t_header), 1, hFP);

		//���_���ǂݍ���
		fread(&vert_count, sizeof(vert_count), 1, hFP);

		//���_�f�[�^�ǂݍ���
		vertex = new t_vertex[vert_count];
		fread(vertex, 38, vert_count, hFP);

		//�ʒ��_���X�g�ǂݍ���
		fread(&face_vert_count, sizeof(face_vert_count), 1, hFP);

		//�ʒ��_���X�g�f�[�^�ǂݍ���
		face_vert_index = new unsigned short[face_vert_count];
		fread(face_vert_index, 2, face_vert_count, hFP);

		//�}�e���A�����X�g�ǂݍ���
		fread(&material_count, sizeof(material_count), 1, hFP);

		//�}�e���A�����X�g�f�[�^�ǂݍ���
		material = new t_material[material_count];
		fread(material, sizeof(t_material), material_count, hFP);

		// �{�[����
		fread(&bone_count, sizeof(bone_count), 1, hFP);
		bone = new t_bone[bone_count];
		fread(bone, sizeof(t_bone), bone_count, hFP);

		//IK��
		fread(&ik_data_count, sizeof(ik_data_count), 1, hFP);
		ik_data = new t_ik_data[ik_data_count];
		for (int i = 0; i < ik_data_count; i++){
			fread(&ik_data[i], sizeof(t_ik_data) - sizeof(unsigned short*), 1, hFP);
			ik_data[i].ik_child_bone_index = new unsigned short[ik_data[i].ik_chain_length];
			//for (int j = 0; j < ik_data[i].ik_chain_length; j++){
				fread(ik_data[i].ik_child_bone_index, sizeof(unsigned short), ik_data[i].ik_chain_length, hFP);
			//}
		}


		//toonMap�g��
		//0x71E8E
		fseek(hFP, 466574L, SEEK_SET);

		ZeroMemory(toon_file_name, sizeof(char[1000]));
		fread(&toon_file_name[0][0], sizeof(char[100]), 10, hFP);


		fclose(hFP);

		mLoadResult = true;
	}
	else{
		MessageBox(NULL, "PMD fopen", "Err", MB_ICONSTOP);
		mLoadResult = false;
	}
}

//�f�X�g���N�^
pmd::~pmd()
{
	if (!mLoadResult)return;
	delete[] vertex;
	delete[] face_vert_index;
	delete[] material;
	delete[] bone;
	for (int i = 0; i < ik_data_count; i++){
		delete[] ik_data[i].ik_child_bone_index;
	}
	delete[] ik_data;
}