#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include "time24.h"
#include <process.h>

namespace bcl{

/*
	�X���b�h�x�[�X�N���X
	���L�̃X���b�h�����ŗp����̂ŁA�X���b�h��p�N���X�͂����
	public�p�����邱��

	----�߂�l�̒�`----
	�e�����͈ꕔ�������A�����Œl��Ԃ�
	0�F�Ȃɂ����Ă��Ȃ��i����p���Ȃ��j
	1�`1000�F����I��
	1001�`�F�x���I���i�X���b�h�I��������قǂł͂Ȃ��G���[�ŏI���j
	���F�ُ�I���i�X���b�h���I�����Ȃ���Ζ��ɂȂ�G���[�ŏI���j
*/
class ThreadProcBase{
	bcl::time24 nextTime;
public:

	virtual const int Init(){ return 0;};
	virtual const int HourlyProc(){ return 0;};
	virtual const int MinutelyProc(){ return 0;};
	virtual const int YearlyProc(){ return 0;};
	virtual const int MonthlyProc(){ return 0;};
	virtual const int DailyProc(){ return 0;};
	virtual const int UpdateProc(){ return 0;};
	virtual const int UpdateProc1(){ return 0;};
	virtual const int UpdateProc2(){ return 0;};
	virtual const int UpdateProc3(){ return 0;};
	virtual const int NextTimeProc(){ return 0;};
	virtual const bool IsNext() const{ return false;};
	virtual void SetNext()
	{ 
		nextTime.reset();
		nextTime += clx::days(1);
		nextTime += clx::seconds(1);
	};
	virtual int Terminate(){ return 0;};
	virtual const bool QuitCheck(){ return false;};
	ThreadProcBase(){
//		TRACE("ThreadProcBase Constructor>>------------\n");
		nextTime.reset();
	};
};

/*
	�X���b�h�@�e���v���[�g
*/
template<typename T>
unsigned int ThreadProc(void *param){
	T	procClass(param);
	//COleDateTime���g���Ȃ��Ƃ��̎g���������K�v�I
	//������AMFC����p�ƂȂ��Ă���
	bcl::time24 bufTime, nowTime;
	const int initRet = procClass.Init();
	if(initRet < 0){
		ExitThread(-1);
	}
	int ret;
	while(1){
		if(procClass.QuitCheck())
			break;
		//�b����
		ret = procClass.UpdateProc();
		if(ret < 0){
			//�[���ȃG���[
			ExitThread(-2);
		}else if(ret > 0){
			//�x���\��
		}
		ret = procClass.UpdateProc1();
		if(ret < 0){
			//�[���ȃG���[
			ExitThread(-3);
		}else if(ret > 0){
			//�x���\��
		}
		ret = procClass.UpdateProc2();
		if(ret < 0){
			//�[���ȃG���[
			ExitThread(-4);
		}else if(ret > 0){
			//�x���\��
		}
		ret = procClass.UpdateProc3();
		if(ret < 0){
			//�[���ȃG���[
			ExitThread(-5);
		}else if(ret > 0){
			//�x���\��
		}
		//���Ԏw�菈��
		if(procClass.IsNext()){
			ret = procClass.NextTimeProc();
			if(ret < 0){
				//�[���ȃG���[
				ExitThread(-6);
			}else if(ret > 0){
				//�x���\��
			}
			procClass.SetNext();
		}
		//�N����
		nowTime.reset();
		if(nowTime.year() != bufTime.year()){
			ret = procClass.YearlyProc();
			if(ret < 0){
				//�[���ȃG���[
				ExitThread(-7);
			}else if(ret > 0){
				//�x���\��
			}
		}
		//������
		if(nowTime.month() != bufTime.month()){
			ret = procClass.MonthlyProc();
			if(ret < 0){
				//�[���ȃG���[
				ExitThread(-8);
			}else if(ret > 0){
				//�x���\��
			}
		}
		//������
		if(nowTime.day() != bufTime.day()){
			ret = procClass.DailyProc();
			if(ret < 0){
				//�[���ȃG���[
				ExitThread(-9);
			}else if(ret > 0){
				//�x���\��
			}
		}
		//������
		if(nowTime.hour() != bufTime.hour()){
			ret = procClass.HourlyProc();
			if(ret < 0){
				//�[���ȃG���[
				ExitThread(-10);
			}else if(ret > 0){
				//�x���\��
			}
		}
		//������
		if(nowTime.minute() != bufTime.minute()){
			ret = procClass.MinutelyProc();
			if(ret < 0){
				//�[���ȃG���[
				ExitThread(-11);
			}else if(ret > 0){
				//�x���\��
			}
		}
		//�i�[
		bufTime = nowTime;
	}
	const int termRet = procClass.Terminate();
	if(ret < 0){
		//�[���ȃG���[
		ExitThread(-11);
	}else if(ret > 0){
		//�x���\��
	}else
		ExitThread(0);
	return 0;
}

inline HANDLE CreateThread(void *pt, unsigned (__stdcall *fn)(void *), int *code)
{
	unsigned int threadID;
	HANDLE hThread;

	//�߂�l�̏�����
	*code = 0;

	/* �X���b�h�쐬 */
	hThread = (HANDLE)_beginthreadex(
			NULL,               //�X���b�h�̃Z�L�����e�B����(NULL:�f�t�H���g)
			0,                  //�����X���b�h �X�^�b�N �T�C�Y(0:�f�t�H���g)
			fn,      //�X���b�h�֐��̃A�h���X
			(LPVOID)pt,         //�V�����X���b�h�̂��߂̈���
			CREATE_SUSPENDED,   //�쐬���~���Ă���
			&threadID);         //�X���b�hID
	if ( hThread == NULL ) {
		*code = errno;
		return NULL;
	}

	return hThread;
}


}; //namespace bcl{

#endif //__HPP__