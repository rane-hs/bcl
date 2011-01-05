//const.h
//局種別番号・項目番号・フラグ定義などに用いる各種定数
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
		ビット定義
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
	enum flag1_normal{		/* 0～127 はデータ値有効 */
		normal = 0,  		//正常
		limit  = 10, 		//範囲異常（データ値有効）
		div_float  = 20, 	//変化量オーバ
		fixed  = 30, 		//一定値
		test   = 40, 		//試験中
		correct= 50, 		//手修正
		note   = 60, 		//欠測を含む
		recal  = 70, 		//再計算
		river_pub_not_over  = 120, //再計算
		flag1_term
	};

	enum flag1_error{		/* 128～255 はデータ値無効 */
		disable = 130,      //非実装
		now_close   = 140,      //閉局（休止）
		now_mainte  = 150,      //点検中（保守中）
		tout    = 160,      //ラグタイムアウト（未入力）
		invalid = 170,      //無効
		none    = 180,      //ファイルなし(未入力)
		err1    = 190,      //欠測
		err2    = 200,      //スケール異常
		flag1_e_term
	};

/* flag2 */
	enum flag2{
		rise60 = bb_bit7,  //上昇中（60分前のデータとの比較）
		down60 = bb_bit6,  //下降中（60分前のデータとの比較）
		sp1    = bb_bit5,  //特情＋
		sp2    = bb_bit4,  //特情－
		rise30 = bb_bit3,  //上昇中（30分前のデータとの比較）
		down30 = bb_bit2,  //下降中（30分前のデータとの比較）
		rise   = bb_bit1,  //上昇中（直近のデータとの比較）
		down   = bb_bit0,  //下降中（直近のデータとの比較）
		flag2_term
	};

/* flag3 */
	enum flag3{
		war1 = bb_bit7,    //警報１  弱
		war2 = bb_bit6,    //警報２  ↓
		war3 = bb_bit5,    //警報３  ↓
		war4 = bb_bit4,    //警報４  ↓
		war5 = bb_bit3,    //警報５  ↓
		war6 = bb_bit2,    //警報６  ↓
		war7 = bb_bit1,    //警報７  ↓
		war8 = bb_bit0,    //警報８  強
		flag3_term
	};
	enum sys_local{
		entry_on = 1,        //実装
		CYCLE_1M = 1,        //１分周期
		TCALC_IP = 1,        //1,イベントポーリング演算を行う
		TCALC_10,            //2,10分周期演算を行う
		TCALC_01,            //3,１分周期演算を行う
		DCALC_DO = 1,        //日集計演算を行う
		sys_local_term
	};
	enum sys_action{
		running = 0,		// 稼働中
		mainte,				//1 : 保守中
		close,				//2 : 閉局
		sys_action_term
	};

};

M__END_NAMESPACE_BCL
