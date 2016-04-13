#pragma once

class pmd{
public:
	bool mLoadResult;
#pragma pack(push,1)	//�A���C�������g������I�t��
	//�w�b�_�[
	struct t_header{
		unsigned char magic[3];
		float version;
		unsigned char model_name[20];
		unsigned char comment[256];
	};
	t_header header;
	//���_���X�g
	unsigned long vert_count;
	struct t_vertex{
		float pos[3];
		float normal_vec[3];
		float uv[2];
		unsigned short bone_num[2];
		unsigned char bone_weight;
		unsigned char edge_flag;
	};
	t_vertex *vertex;
	//�ʒ��_���X�g
	unsigned long face_vert_count;
	unsigned short *face_vert_index;

	unsigned long material_count;
	struct t_material{
		float diffuse_color[3];// �����F
		float alpha;//	�����F�̕s�����x
		float specularity;
		float specular_color[3];//	����F
		float mirror_color[3];//	���F
		unsigned char toon_index;
		unsigned char edge_flag;//	�֊s�t���O
		unsigned long face_vert_count;//	���̍ގ��Ŏg���ʒ��_���X�g�̃f�[�^��
		char texture_file_name[20];//	�e�N�X�`���t�@�C����[20] + %0
	};
	t_material* material;// �ގ��f�[�^

	unsigned short bone_count;
	struct t_bone{
		char bone_name[20];	// �{�[����
		unsigned short parent_bidx;	// �e�{�[���ԍ�(�Ȃ��ꍇ��0xFFFF)
		unsigned short tail_pos_bidx; // tail�ʒu�̃{�[���ԍ�(�`�F�[�����[�̏ꍇ��0xFFFF) // �e�F�q��1�F���Ȃ̂ŁA��Ɉʒu���ߗp
		unsigned char bone_type;		// �{�[���̎��
		unsigned short ik_parent_bidx;// IK�{�[���ԍ�(�e��IK�{�[���B�Ȃ��ꍇ��0)
		float bone_head_pos[3]; // x, y, z // �{�[���̃w�b�h�̈ʒu
	};
	t_bone* bone;
	//  �E�{�[���̎��
	//	0:��] 1 : ��]�ƈړ� 2 : IK 3 : �s�� 4 : IK�e���� 5 : ��]�e���� 6 : IK�ڑ��� 7 : ��\��
	//	�E�{�[���̎��(MMD 4.0�`)
	//	8 : �P�� 9 : ��]�^��

	unsigned short ik_data_count; // IK�f�[�^��
	struct t_ik_data{
		unsigned short ik_bone_index; // IK�{�[���ԍ�
		unsigned short ik_target_bone_index; // IK�^�[�Q�b�g�{�[���ԍ� // IK�{�[�����ŏ��ɐڑ�����{�[��
		unsigned char ik_chain_length; // IK�`�F�[���̒���(�q�̐�)
		unsigned short iterations; // �ċA���Z�� // IK�l1
		float control_weight; // ���Z1�񂠂���̐����p�x // IK�l2
		unsigned short* ik_child_bone_index; //[ik_chain_length] IK�e�����̃{�[���ԍ�
	};
	t_ik_data* ik_data; // IK�f�[�^((11+2*ik_chain_length)/IK)


	//�\��X�g
	unsigned short skin_count; // �\�
	struct t_skin_vert_data{//(type�Fbase)
		unsigned long skin_vert_index; // �\��p�̒��_�̔ԍ�(���_���X�g�ɂ���ԍ�)
		float skin_vert_pos[3]; // x, y, z // �\��p�̒��_�̍��W(���_���̂̍��W)
	};
	//�Et_skin_vert_data(type�Fbase�ȊO)
	//	DWORD base_skin_vert_index; // �\��p�̒��_�̔ԍ�(base�̔ԍ��Bskin_vert_index)
	//float skin_vert_pos_offset[3]; // x, y, z // �\��p�̒��_�̍��W�I�t�Z�b�g�l(base�ɑ΂���I�t�Z�b�g)
	struct t_skin_data{
		;
		char skin_name[20]; //�@�\�
		unsigned long skin_vert_count; // �\��p�̒��_��
		unsigned short skin_type; // �\��̎�� // 0�Fbase�A1�F�܂�A2�F�ځA3�F���b�v�A4�F���̑�
		t_skin_vert_data* skin_vert_data; // �\��p�̒��_�̃f�[�^(16Bytes/vert)
	};
	t_skin_data* skin_data; // �\��f�[�^((25+16*skin_vert_count)/skin)


	//�\��g�p�\�����X�g
	unsigned char skin_disp_count; // �\��g�ɕ\������\�
	unsigned short* skin_index; // �\��ԍ�


	//�{�[���g�p�g�����X�g
	unsigned char bone_disp_name_count; // �{�[���g�p�̘g���� // �Z���^�[(1�ԏ�ɕ\�������g)�͊܂܂Ȃ�
	char* disp_name[50]; // �g��(50Bytes/�g)

	//�⑫�F
	//	PMDeditor���g���ꍇ�́A�g����0x0A00�ŏI��点��K�v������܂��B(0x00�݂̂��ƕ\������܂���)��0.0.4.2c�Ŋm�F�B
	//	MMD�ł͕�����I�[��0x00�݂̂ł��\���\�ł��B��6.08�Ŋm�F�B(�t�����f����0x0A00�ŏI�[���Ă���悤�ł�)



	//�{�[���g�p�\�����X�g
	unsigned long bone_disp_count; // �{�[���g�ɕ\������{�[���� (�g0(�Z���^�[)�������A���ׂẴ{�[���g�̍��v)
	struct t_bone_disp{
		unsigned short bone_index; // �g�p�{�[���ԍ�
		unsigned char bone_disp_frame_index; // �\���g�ԍ� // �Z���^�[:00 ���̕\���g:01�` // �Z���^�[�g�ɃZ���^�[(index 0)�ȊO�̃{�[�����w�肵�Ă��\������܂���B
	};
	t_bone_disp* bone_disp; // �g�p�{�[���f�[�^ (3Bytes/bone)







	char toon_file_name[10][100];
#pragma pack(pop)	//�A���C�������g����G���h

	//�R���X�g���N�^
	pmd(const char *);

	//�f�X�g���N�^
	~pmd();
};