//const.h
//�ǎ�ʔԍ��E���ڔԍ��E�t���O��`�Ȃǂɗp����e��萔
//[1st] 2009-07-14 H.Seki
//----------------------------------------------------
//
/* Usage
 //kno
  int knoriver = bcl::cnst::kno::river; //2
 //ino
  int inoRiverNow = bcl::cnst::ino.river::ri_now;	//2
 //flag1
  bf_data_t wk_dt;
  wk_dt.f[0] = bcl::cnst::frag1_err::invalid;		//170
*/
#include "bcl.h"

M__BEGIN_NAMESPACE_BCL

class inoSet{
private:
public:
	enum river{
		ri_recv_data = 1,
		ri_now,				//2
		ri_div_data,		//3
		ri_div_30min,		//4
		ri_div_1h,			//5
		ri_flow_recv,		//6
		ri_flow_now,		//7
		ri_flow_div,		//8
		ri_flow_div_30min,	//9
		ri_flow_div_1h,		//10
		ri_publication_time,//11
		ri_publication_data,//12
		ri_ino_river_term
	};
	enum flow{
		fl_recv = 6,	//6
		fl_now,			//7
		fl_div,			//8
		fl_div_30min,	//9
		fl_div_1h,		//10
		fl_ino_flow_term
	};
};

class cnst{
public:
	/*---------------------------------
		�r�b�g��`
	 ----------------------------------*/
	enum bb_bit{
		 bb_bit0 = 0x01,
		 bb_bit1 = 0x02,
		 bb_bit2 = 0x04,
		 bb_bit3 = 0x08,
		 bb_bit4 = 0x10,
		 bb_bit5 = 0x20,
		 bb_bit6 = 0x40,
		 bb_bit7 = 0x80,
		 bb_term
	};
	static inoSet ino;
	// kind no
	enum kno{
		rain = 1,
		river,		//2
		dam,		//3
		tide,		//4
		wind,		//5
		itv = 13,	//13
		kno_term
	};
	enum kno_cast{
		cast_rain = 100,
		kno_cast_term
	};
	/* flag1 */
	enum flag1_normal{		/* 0�`127 �̓f�[�^�l�L�� */
		normal = 0,  		//����
		limit  = 10, 		//�ُ͈͈�i�f�[�^�l�L���j
		div_float  = 20, 	//�ω��ʃI�[�o
		fixed  = 30, 		//���l
		test   = 40, 		//������
		correct= 50, 		//��C��
		note   = 60, 		//�������܂�
		recal  = 70, 		//�Čv�Z
		river_pub_not_over  = 120, //�Čv�Z
		flag1_term
	};

	enum flag1_error{		/* 128�`255 �̓f�[�^�l���� */
		disable = 130,      //�����
		now_close   = 140,      //�ǁi�x�~�j
		now_mainte  = 150,      //�_�����i�ێ璆�j
		tout    = 160,      //���O�^�C���A�E�g�i�����́j
		invalid = 170,      //����
		none    = 180,      //�t�@�C���Ȃ�(������)
		err1    = 190,      //����
		err2    = 200,      //�X�P�[���ُ�
		flag1_e_term
	};

/* flag2 */
	enum flag2{
		rise60 = bb_bit7,  //�㏸���i60���O�̃f�[�^�Ƃ̔�r�j
		down60 = bb_bit6,  //���~���i60���O�̃f�[�^�Ƃ̔�r�j
		sp1    = bb_bit5,  //����{
		sp2    = bb_bit4,  //����|
		rise30 = bb_bit3,  //�㏸���i30���O�̃f�[�^�Ƃ̔�r�j
		down30 = bb_bit2,  //���~���i30���O�̃f�[�^�Ƃ̔�r�j
		rise   = bb_bit1,  //�㏸���i���߂̃f�[�^�Ƃ̔�r�j
		down   = bb_bit0,  //���~���i���߂̃f�[�^�Ƃ̔�r�j
		flag2_term
	};

/* flag3 */
	enum flag3{
		war1 = bb_bit7,    //�x��P  ��
		war2 = bb_bit6,    //�x��Q  ��
		war3 = bb_bit5,    //�x��R  ��
		war4 = bb_bit4,    //�x��S  ��
		war5 = bb_bit3,    //�x��T  ��
		war6 = bb_bit2,    //�x��U  ��
		war7 = bb_bit1,    //�x��V  ��
		war8 = bb_bit0,    //�x��W  ��
		flag3_term
	};
	enum sys_local{
		entry_on = 1,        //����
		CYCLE_1M = 1,        //�P������
		TCALC_IP = 1,        //1,�C�x���g�|�[�����O���Z���s��
		TCALC_10,            //2,10���������Z���s��
		TCALC_01,            //3,�P���������Z���s��
		DCALC_DO = 1,        //���W�v���Z���s��
		sys_local_term
	};
	enum sys_action{
		running = 0,		// �ғ���
		mainte,				//1 : �ێ璆
		close,				//2 : ��
		sys_action_term
	};

};

M__END_NAMESPACE_BCL
