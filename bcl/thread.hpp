#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include "time24.h"
#include <process.h>

namespace bcl{

/*
	スレッドベースクラス
	下記のスレッド処理で用いるので、スレッド専用クラスはこれを
	public継承すること

	----戻り値の定義----
	各処理は一部を除き、整数で値を返す
	0：なにもしていない（副作用もない）
	1～1000：正常終了
	1001～：警告終了（スレッド終了させるほどではないエラーで終了）
	負：異常終了（スレッドを終了しなければ問題になるエラーで終了）
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
	スレッド　テンプレート
*/
template<typename T>
unsigned int ThreadProc(void *param){
	T	procClass(param);
	//COleDateTimeが使えないときの使い分けが必要！
	//※現状、MFC環境専用となっている
	bcl::time24 bufTime, nowTime;
	const int initRet = procClass.Init();
	if(initRet < 0){
		ExitThread(-1);
	}
	int ret;
	while(1){
		if(procClass.QuitCheck())
			break;
		//秒処理
		ret = procClass.UpdateProc();
		if(ret < 0){
			//深刻なエラー
			ExitThread(-2);
		}else if(ret > 0){
			//警告表示
		}
		ret = procClass.UpdateProc1();
		if(ret < 0){
			//深刻なエラー
			ExitThread(-3);
		}else if(ret > 0){
			//警告表示
		}
		ret = procClass.UpdateProc2();
		if(ret < 0){
			//深刻なエラー
			ExitThread(-4);
		}else if(ret > 0){
			//警告表示
		}
		ret = procClass.UpdateProc3();
		if(ret < 0){
			//深刻なエラー
			ExitThread(-5);
		}else if(ret > 0){
			//警告表示
		}
		//時間指定処理
		if(procClass.IsNext()){
			ret = procClass.NextTimeProc();
			if(ret < 0){
				//深刻なエラー
				ExitThread(-6);
			}else if(ret > 0){
				//警告表示
			}
			procClass.SetNext();
		}
		//年処理
		nowTime.reset();
		if(nowTime.year() != bufTime.year()){
			ret = procClass.YearlyProc();
			if(ret < 0){
				//深刻なエラー
				ExitThread(-7);
			}else if(ret > 0){
				//警告表示
			}
		}
		//月処理
		if(nowTime.month() != bufTime.month()){
			ret = procClass.MonthlyProc();
			if(ret < 0){
				//深刻なエラー
				ExitThread(-8);
			}else if(ret > 0){
				//警告表示
			}
		}
		//日処理
		if(nowTime.day() != bufTime.day()){
			ret = procClass.DailyProc();
			if(ret < 0){
				//深刻なエラー
				ExitThread(-9);
			}else if(ret > 0){
				//警告表示
			}
		}
		//時処理
		if(nowTime.hour() != bufTime.hour()){
			ret = procClass.HourlyProc();
			if(ret < 0){
				//深刻なエラー
				ExitThread(-10);
			}else if(ret > 0){
				//警告表示
			}
		}
		//分処理
		if(nowTime.minute() != bufTime.minute()){
			ret = procClass.MinutelyProc();
			if(ret < 0){
				//深刻なエラー
				ExitThread(-11);
			}else if(ret > 0){
				//警告表示
			}
		}
		//格納
		bufTime = nowTime;
	}
	const int termRet = procClass.Terminate();
	if(ret < 0){
		//深刻なエラー
		ExitThread(-11);
	}else if(ret > 0){
		//警告表示
	}else
		ExitThread(0);
	return 0;
}

inline HANDLE CreateThread(void *pt, unsigned (__stdcall *fn)(void *), int *code)
{
	unsigned int threadID;
	HANDLE hThread;

	//戻り値の初期化
	*code = 0;

	/* スレッド作成 */
	hThread = (HANDLE)_beginthreadex(
			NULL,               //スレッドのセキュリティ属性(NULL:デフォルト)
			0,                  //初期スレッド スタック サイズ(0:デフォルト)
			fn,      //スレッド関数のアドレス
			(LPVOID)pt,         //新しいスレッドのための引数
			CREATE_SUSPENDED,   //作成後停止している
			&threadID);         //スレッドID
	if ( hThread == NULL ) {
		*code = errno;
		return NULL;
	}

	return hThread;
}


}; //namespace bcl{

#endif //__HPP__