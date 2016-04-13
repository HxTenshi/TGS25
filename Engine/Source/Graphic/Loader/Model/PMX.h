#pragma once

#include <string>

#include <vector>

class pmx{
public:
	void Read(int* p, unsigned char size, FILE* fp);
	bool mLoadResult;
#pragma pack(push,1)	//�A���C�������g������I�t��
	//�w�b�_�[
	struct t_header{
		unsigned char magic[4];
		float version;
		unsigned char configNum;
		unsigned char config[8];
		enum{
			ENCODE = 0,	//[0] - �G���R�[�h����  | 0:UTF16 1:UTF8
			ADDUV,		//[1] - �ǉ�UV�� 	| 0�`4 �ڍׂ͒��_�Q��
			VIDX,		//[2] - ���_Index�T�C�Y | 1,2,4 �̂����ꂩ
			TIDX,		//[3] - �e�N�X�`��Index�T�C�Y | 1,2,4 �̂����ꂩ
			MIDX,		//[4] - �ގ�Index�T�C�Y | 1,2,4 �̂����ꂩ
			BIDX,		//[5] - �{�[��Index�T�C�Y | 1,2,4 �̂����ꂩ
			PIDX,		//[6] - ���[�tIndex�T�C�Y | 1,2,4 �̂����ꂩ
			RIDX,		//[7] - ����Index�T�C�Y | 1,2,4 �̂����ꂩ
		};
		enum {
			ENCODE_UTF16 = 0,
			ENCODE_UTF8 = 1,
		};
	};
	t_header header;
	struct t_model_info{
		std::string modelName;
		std::string modelNameEn;
		std::string modelInfo;
		std::string modelInfoEn;
	};
	t_model_info modelInfo;

	void LoadText(FILE* fp, std::string& str);

	//���_���X�g
	int vert_count;

	template<class SizeType>
	struct t_bone_BDEF1
	{
		SizeType bone_num[1];

	};
	template<class SizeType>
	struct t_bone_BDEF2
	{
		SizeType bone_num[2];
		float bone_weight[1];
	};
	template<class SizeType>
	struct t_bone_BDEF4
	{
		SizeType bone_num[4];
		float bone_weight[4];
	};
	template<class SizeType>
	struct t_bone_SDEF
	{
		SizeType bone_num[2];
		float bone_weight;
		float C[3];
		float R0[3];
		float R1[3];
	};

	struct t_vertex_data{
		enum {
			ADDUV_MAX = 4,
			BDEF1 = 0,
			BDEF2 = 1,
			BDEF4 = 2,
			SDEF = 3,
			QDEF = 4,
		};
		struct t_vertex{
			struct t_v{
				float pos[3];
				float normal_vec[3];
				float uv[2];
			};
			t_v v;

			float avAdd[4][4];

			unsigned char weight_type;
			int bone_num[4];
			float bone_weight[4];
			//SDEF 
			float C[3];
			float R0[3];
			float R1[3];

			float edge_flag;
		};
		t_vertex *vertex;
	};
	t_vertex_data vertex_data;

	void NewVoidPointer(void** p, int newNum, unsigned char size);
	void DeleteVoidPointer(void** p, unsigned char size);
	
	//�ʒ��_���X�g
	int face_vert_count;
	void *face_vert_index;

	//�e�N�X�`�����X�g
	int texture_count;
	std::string* textureName;

	//Material
	int material_count;
	struct t_material{
		std::string MaterialName;
		std::string MaterialNameEn;
		struct t_m{
			float Diffuse[4];// �����F
			float Specular[3];//	����F
			float Specularity;
			float Ambient[3];//	���F
			unsigned char bitFlag;// �`��t���O(8bit) - �ebit 0:OFF 1 : ON,0x01 : ���ʕ`��, 0x02 : �n�ʉe, 0x04 : �Z���t�V���h�E�}�b�v�ւ̕`��, 0x08 : �Z���t�V���h�E�̕`��,0x10 : �G�b�W�`��

			float EdgeColor[4];
			float EdgeSize;
		} material;

		int TextureIdx;//�e�N�X�`��Index�T�C�Y | �ʏ�e�N�X�`��, �e�N�X�`���e�[�u���̎Q��Index
		int SphereIdx;//�e�N�X�`��Index�T�C�Y | �X�t�B�A�e�N�X�`��, �e�N�X�`���e�[�u���̎Q��Index  ���e�N�X�`���g���q�̐����Ȃ�

		unsigned char SphereMode;// | �X�t�B�A���[�h 0 : ���� 1 : ��Z(sph) 2 : ���Z(spa) 3 : �T�u�e�N�X�`��(�ǉ�UV1��x, y��UV�Q�Ƃ��Ēʏ�e�N�X�`���`����s��)
		unsigned char ToonMode;// ���LToon�t���O 0 : �p���l�͌�Toon 1 : �p���l�͋��LToon

		int ToonIdx;//���LToon�t���O:0 �̏ꍇ �e�N�X�`��Index�T�C�Y | Toon�e�N�X�`��, �e�N�X�`���e�[�u���̎Q��Index ���LToon�t���O: 1 �̏ꍇ ���LToon�e�N�X�`��[0�`9]->���ꂼ�� toon01.bmp�`toon10.bmp �ɑΉ�

		std::string memo;
		int face_vert_count;//	���̍ގ��Ŏg���ʒ��_���X�g�̃f�[�^��

	};
	t_material* material;// �ގ��f�[�^

	struct IkLink
	{
		int idxBone;
		unsigned char bLimit;
		float Min[3];//bLimit=1
		float Max[3];//bLimit=1
	};
	struct t_ik_data{
		int idx;// �{�[��Index�T�C�Y | IK�^�[�Q�b�g�{�[���̃{�[��Index;
		int Loop;
		float LimitAng;
		std::vector<IkLink> IKLink;
	};
	int bone_count;
	struct t_bone{
		enum{
			BIT_JOINT = 0x0001,			// �ڑ���(PMD�q�{�[���w��)�\�����@ -> 0:���W�I�t�Z�b�g�Ŏw�� 1:�{�[���Ŏw��
			BIT_ROT = 0x0002,			// ��]�\
			BIT_MOVE = 0x0004,			// �ړ��\
			BIT_VISIBLE = 0x0008,		// �\��
			BIT_MANIP = 0x0010,			// �����
			BIT_IK = 0x0020,			// IK
			BIT_GIVE_LOCAL = 0x0080,	// ���[�J���t�^ | �t�^�Ώ� 0:���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1:�e�̃��[�J���ό`��
			BIT_GIVE_ROT = 0x0100,		// ��]�t�^
			BIT_GIVE_MOVE = 0x0200,		// �ړ��t�^
			BIT_FIX_AXIS = 0x0400,		// ���Œ�
			BIT_LOCAL_AXIS = 0x0800,	// ���[�J����
			BIT_POST_PHYSICS = 0x1000,	// ������ό`
			BIT_EXT_PARENT = 0x2000,	// �O���e�ό`
		};
		std::string BoneName;
		std::string BoneNameEn;
		float bone_head_pos[3]; // x, y, z // �{�[���̃w�b�h�̈ʒu
		int parent_bidx;	//�{�[��Index�T�C�Y �e�{�[���ԍ�(�Ȃ��ꍇ��0xFFFF)
		int Hierarchy;//�ό`�K�w
		unsigned short bone_flag;		// �{�[���̎��

		float JointPos[3];	//BIT_JOINT=0
		int idxJoint;		//BIT_JOINT=1
		int idxGive;		//BIT_GIVE_ROT=1 || BIT_GIVE_MOVE=1
		float fGive;		//BIT_GIVE_ROT=1 || BIT_GIVE_MOVE=1
		float FixAxis[3];	//BIT_FIX_AXIS=1
		float AxisX[3];	//BIT_LOCAL_AXIS=1
		float AxisZ[3];	//BIT_LOCAL_AXIS=1
		int iExtParent;	//BIT_EXT_PARENT=1
		t_ik_data Ik;
	};
	t_bone* bone;

	unsigned long ik_count;


#pragma pack(pop)	//�A���C�������g����G���h

	//�R���X�g���N�^
	pmx(const char *);

	//�f�X�g���N�^
	~pmx();
};