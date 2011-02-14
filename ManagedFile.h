//#include "systemconst.h"
#ifndef __UPDATEINFO_H__
#define __UPDATEINFO_H__
#include <bcl/FileSystem.h>
#include <bcl/bclstr.h>
#include <bcl/Time24.h>

namespace bcl{	//bcl::

/*!------------------------------------------------------------------
	class define
-------------------------------------------------------------------*/
/*!--------------------------------------------------------
*	[ファイル情報定義クラス]
*	履歴：2008-09-02 新規 H.Seki
*		：xxxx-xx-xx 変更 xxx
---------------------------------------------------------*/
class ManagedFile{
public:
	//constructor
	ManagedFile(const bcl::char_t *fpath = NULL)
	{
		if(fpath)	filePath_ = fpath;
		fUpdTime = bcl::NowTime();
	}
	//更新検出処理
	const bool IsUpd()
	{
		if(!ExistFile()) return false;
		const bcl::time24 bufTime(FileTime());
		if(fUpdTime < bufTime){
#if defined(_DEBUG) && defined (__AFXWIN_H__)
			TRACE(bcl::format("%04d/%02d/%02d %02d:%02d:%02d -> %04d/%02d/%02d %02d:%02d:%02d\n",
				fUpdTime.year(), fUpdTime.month(), fUpdTime.day(),	fUpdTime.hour(), fUpdTime.minute(), fUpdTime.second(),
				bufTime.year(), bufTime.month(), bufTime.day(), bufTime.hour(), bufTime.minute(), bufTime.second()
				).c_str());

#endif
			return true;
		}else
			return false;
	}
	const bcl::time24 SetTime(){
		return fUpdTime = FileTime();
	}
	void SetTime(const bcl::time24 &s){
		fUpdTime = s;
	}
	const bool ExistFile()
	{
		return bcl::File::IsExist(filePath_.c_str());
	};
	//ファイルの時間をセット
	void set_mtime(bcl::time24 *s){fUpdTime = *s;};
	const bcl::time24 get_mtime(){return fUpdTime;};
	const bcl::time24 FileTime()
	{
		if(!ExistFile())
			return bcl::time24(1970, 1, 1, 0, 0, 0);
		else
			return bcl::time24(bcl::File::mTime(filePath_.c_str()));
	}
	bcl::str_t FilePath() const{ return filePath_;}
protected:
	bcl::str_t		filePath_;
	bcl::time24		fUpdTime;
};

}; //bcl::

#endif // __UPDATEINFO_H__
