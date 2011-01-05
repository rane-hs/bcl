// FileSystem.h: ファイル・フォルダ処理
//	いずれはBoost::FileSystemに切り替えていく予定である
//////////////////////////////////////////////////////////////////////

#ifndef __BCL_FILESYSTEM_H__
#define __BCL_FILESYSTEM_H__

#include "time24.h"
#include <string>
#include <vector>
#include <fstream>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <windows.h>

namespace bcl{ //bcl::

class FilePath{
	const str_t drv, dir, fname, ext;
public:
	FilePath(const str_t &drv_, const str_t &dir_, const str_t &fname_, const str_t &ext_):
	  drv(drv_),dir(dir_),fname(fname_),ext(ext_){}
	FilePath(const str_t &fPath=L".\\"){
		char_t fpath_buf[2048];
		_wfullpath(fpath_buf, fPath.c_str(), sizeof(fpath_buf));
		const str_t fullPath = fpath_buf;
		char_t	l_drv[32];
		char_t	l_dir[512];
		char_t	l_fname[256];
		char_t	l_ext[32];
		_wsplitpath(fullPath.c_str(), l_drv, l_dir, l_fname, l_ext);
		FilePath(l_drv, l_dir, l_fname, l_ext);
	};
	const str_t Fname(){		return fname;	}
	const str_t Ext(){		return ext;	}
	const str_t Drv(){		return drv;	}
	const str_t Dir(){		return dir;	}
	const str_t DirPath(){	return drv+dir;	}
	const str_t FileNameExt(){	return fname+ext;	}
};

inline const std::wstring DirPath(const std::wstring &fullpath){
	return bcl::FilePath(fullpath).DirPath();
};
inline const std::wstring FileNameExt(const std::wstring &fullpath){
	return bcl::FilePath(fullpath).FileNameExt();
};

namespace Dir{


	// ディレクトリ確認
	inline const bool IsExist(const char_t *fpath)
	{
		// パスの分割
		const str_t ext = bcl::FilePath(fpath).Ext();
		str_t drv_dir = bcl::FilePath(fpath).DirPath();

		// 拡張子がなければ、ファイル名をフォルダとする
		if (ext.length() < 1)
			drv_dir += ext;

		if(drv_dir.length() < 1) return false;

		// ディレクトリの最後の'\'を消す
		const char_t lastChar = drv_dir[drv_dir.length()-1];
		if ((lastChar == '\\') || (lastChar == '/'))
			drv_dir = drv_dir.substr(0, drv_dir.length()-1);

		// ディレクトリの検索
		WIN32_FIND_DATA	pFindFileData;
		HANDLE			hFind = FindFirstFile(drv_dir.c_str(), &pFindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
			return false;

		FindClose(hFind);
		return true;
	};
	inline bool	IsExist(const str_t &fpath){ return IsExist(fpath.c_str());};
#if defined(_MSC_VER) && (_MSC_VER > 1300)	// > VC6
	inline bool	IsExist(const char *fpath){ return IsExist(widen(fpath).c_str());};
	inline bool	IsExist(const std::string &fpath){ return IsExist(widen(fpath).c_str());};
#endif
	// ディレクトリ作成
	inline const char Make(const char_t *fpath)
	{
		if(IsExist(fpath)) return 0;

		//パスの\\を/に置換する
		str_t strPath = fpath;
		bcl::replace(strPath, L"\\", L"/");
		// パスの分割
		FilePath	dividedPath(strPath);
		//フォルダなし
		if( dividedPath.Dir().length() < 1) 
			return 0;
		//拡張子の有無をチェック
		str_t	mkPath;
		if (dividedPath.Ext().length() < 1) {
			mkPath = dividedPath.Drv() + dividedPath.Dir() + dividedPath.Fname();
		}else{
			// ディレクトリの最後の'/'を消す
			str_t	dirBuf = dividedPath.Dir();
			if(dirBuf[dirBuf.length() - 1] == '/')
				dirBuf = dirBuf.substr(0, dirBuf.length() -1 );
			mkPath = dividedPath.Drv() + dirBuf;
		}

		//ファイルパスを最初から捜査しつつフォルダを階層ごとに生成する
		for(size_t i=0;i<mkPath.length(); i++){
			if (i >= 1) {
				if ((mkPath[i-1] > 0) && (mkPath[i] == '/'))
					int ret = CreateDirectory(mkPath.substr(0, i-1).c_str(), NULL);
			}
		}
		int ret = CreateDirectory(mkPath.c_str(), NULL);
		if (ret == -1)
			return -1;

		return 0;
	}
	inline const char Make(const std::string &fpath){ return Make(fpath.c_str());};
	// ディレクトリチェック
	inline const char Check(const char *fpath)
	{
		char ret = 0;

		if (!IsExist(fpath)) {
			// ディレクトリ作成
			ret = Make(fpath);
		}
		return ret;
	};
	inline const char Delete(const char *dirpath)
	{
		if (!Dir::IsExist(dirpath)) {
			return 1;
		}
		if(-1 == _rmdir(dirpath))
			return -1;
		else
			return 0;
	};
	inline const char Clear(const char *fpath)
	{
		long hFind;
		struct _finddata_t dt;

		//存在確認
		if(IsExist(fpath) == false)
			return false;
		//ファイルの検索
		const std::string searchDef = bcl::format("%s\\*", fpath);

		hFind = _findfirst(searchDef.c_str(), &dt);
		if (hFind != -1) {
			do {
				if (strcmp(dt.name, ".") == 0 || strcmp(dt.name, "..") == 0)
					continue;

				const std::string delPath = bcl::format("%s\\%s", fpath, dt.name);
				if (dt.attrib == _A_SUBDIR) {
					Dir::Clear(delPath.c_str());
					Dir::Delete(delPath.c_str());
				} else {
					remove(delPath.c_str());
				}
			} while (_findnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return TRUE;
	};
	inline const bool IsExistDef(const char *searchDef)
	{
		long hFind;
		struct _finddata_t dt;

		hFind = _findfirst(searchDef, &dt);
		if (hFind != -1) {
			do {
				if (strcmp(dt.name, ".") == 0 || strcmp(dt.name, "..") == 0)
					continue;

				if (dt.attrib & _A_SUBDIR) {
					_findclose(hFind);
					return true;
				}
			} while (_findnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return false;
	}
};	//Dir::
namespace File{
	inline const char Read(const char *fpath, std::vector<std::string> *pTxtList)
	{
		std::ifstream fs;

		fs.open(fpath);
		if(!fs.is_open())
			return -1;
		try{
			std::string lineStr;
			pTxtList->clear();
			//1st-
			while(!fs.eof()){
				std::getline(fs, lineStr);
				pTxtList->push_back(lineStr);
				break;
			}
		}catch(...){
			fs.close();
			return -2;
		}
		fs.close();	
		return 0;
	};

	// ファイル削除
	inline const bool Delete(const char_t *fpath)
	{
		WIN32_FIND_DATA	pFindFileData;
		HANDLE			hFind;
		int				ret = 0;

		// ファイル検索
		const str_t dirPath = bcl::DirPath(fpath);
		hFind = FindFirstFile(dirPath.c_str(), &pFindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
			return false;

		do {
			if ((pFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
				// ファイル削除
				char	strFile[_MAX_PATH];
				sprintf(strFile, "%s%s", dirPath.c_str(), pFindFileData.cFileName);
				ret = remove(strFile);
			}
		} while (FindNextFile(hFind, &pFindFileData));
		FindClose(hFind);

		if (ret == -1)
			return false;

		return true;
	};
	/********************************************************************
	*	関数：copyFile
	*	機能：ファイルコピー
	*	引数：spath             [IN]  コピー元ファイル
			：fpath             [IN]  コピー先ファイル
	*	戻値：成否
			：   0 = 成功
			：  -1 = コピー元ファイルが存在しない
			：  -2 = 編集領域の確保失敗
			：  -3 = コピー元ファイルオープン失敗
			：  -4 = コピー先ファイルオープン失敗
	*	説明：
	********************************************************************/
	inline const char Copy(const char_t *spath, const char_t *fpath)
	{
		return ::CopyFile(spath, fpath, false);
	}
	inline const char Copy(const str_t &from, const str_t &to)
	{
		return Copy(from.c_str(), to.c_str());
	}
#if defined(_MSC_VER) && (_MSC_VER > 1300)	// > VC6
	inline const char Copy(const char *spath, const char *fpath)
	{
		return Copy(bcl::widen(spath), bcl::widen(fpath));
	};
	inline const char Copy(const std::string &from, const std::string &to)
	{
		return Copy(from.c_str(), to.c_str());
	}
	inline const int Move(const char_t *spath, const char_t *fpath)
	{
		BOOL nRet = Copy(spath, fpath);
		if(nRet == FALSE)
			return -1;
		return DeleteFile(spath);
	};
#endif
	inline const bool IsExist(const str_t &fpath)
	{
		OFSTRUCT	ob;
		ob.cBytes = sizeof(OFSTRUCT);
		HANDLE		hdl = CreateFile(fpath.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == hdl)
			return false;
		else{
			CloseHandle(hdl);
			return true;
		}
	};
#if defined(_MSC_VER) && (_MSC_VER > 1200)	// > VC6
	inline const bool IsExist(const char *fpath)
	{
		return IsExist(bcl::widen(fpath));
	}
	inline const bool IsExist(const std::string &fpath)
	{
		return IsExist(bcl::widen(fpath));
	}
#endif
	/********************************************************************
	  関数  ： IsExistDef
	  機能  ： フォルダ内ファイル削除
	  引数  ： fpath             [IN]  検索フォルダ
	  戻り値：	TRUE：成功
				FALSE：失敗
	  説明  ： 指定された検索条件にヒットするファイルがある場合、trueを返します
			： TRUEではないので注意
	********************************************************************/
	inline const bool IsExistDef(const char *searchDef)
	{
		long hFind;
		struct _finddata_t dt;

		hFind = _findfirst(searchDef, &dt);
		if (hFind != -1) {
			do {
				if (strcmp(dt.name, ".") == 0 || strcmp(dt.name, "..") == 0)
					continue;

				if (dt.attrib == _A_SUBDIR) {
					continue;
				} else {
					_findclose(hFind);
					return true;
				}
			} while (_findnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return false;
	};
	/********************************************************************
	  関数  ： FindFileDef
	  機能  ： フォルダ内ファイル
	  引数  ： fpath             [IN]  検索フォルダ
	  戻り値：	TRUE：成功
				FALSE：失敗
	  説明  ： 指定された検索条件にヒットするファイルがある場合、フルパスを返します
	********************************************************************/
	inline const std::wstring FindDef(const wchar_t *searchDef)
	{
		long hFind;
		struct _wfinddata_t dt;

		hFind = _wfindfirst(searchDef, &dt);
		if (hFind != -1) {
			do {
				if (wcscmp(dt.name, L".") == 0 || wcscmp(dt.name, L"..") == 0)
					continue;

				if (dt.attrib == _A_SUBDIR) {
					continue;
				} else {
					_findclose(hFind);
					return bcl::format(L"%s%s", bcl::FilePath(searchDef).DirPath().c_str(), dt.name);
				}
			} while (_wfindnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return L"";
	};
	inline const std::string FindDef(const char *searchDef)
	{	return bcl::narrow(FindDef(bcl::widen(searchDef).c_str())); }

#if defined(_MSC_VER) && (_MSC_VER > 1200)	// > VC6
	inline void FindDef(const wchar_t *searchDef, std::vector<std::wstring> *pList)
	{
		long hFind;
		struct _wfinddata_t dt;
		pList->clear();

		hFind = _wfindfirst(searchDef, &dt);
		if (hFind != -1) {
			do {
				if (wcscmp(dt.name, L".") == 0 || wcscmp(dt.name, L"..") == 0)
					continue;

				if (dt.attrib == _A_SUBDIR) {
					continue;
				} else {
					pList->push_back(bcl::format(L"%s%s", bcl::FilePath(searchDef).DirPath().c_str(), dt.name));
				}
			} while (_wfindnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
	};
	inline const int RemoveDef(const wchar_t *searchDef)
	{
		long hFind;
		struct _wfinddata_t dt;
		int out = 0;

		hFind = _wfindfirst(searchDef, &dt);
		if (hFind != -1) {
			do {
				if (wcscmp(dt.name, L".") == 0 || wcscmp(dt.name, L"..") == 0)
					continue;

				if (dt.attrib == _A_SUBDIR) {
					continue;
				} else {
					bcl::File::Delete(bcl::format(L"%s%s", bcl::FilePath(searchDef).DirPath().c_str(), dt.name).c_str());
					out++;
				}
			} while (_wfindnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return out;
	};
	inline const bcl::time24	mTime(const char *file_name)
	{
		const bcl::time24 errTime(0,0,0,0,0,0);
		if(!IsExist(file_name))	return errTime;
		struct _stat st;
		const int ret = _stat(file_name, &st);
		if(ret != 0) 	return errTime;

		return bcl::time24(st.st_mtime);
	};
	inline const bcl::time24	mTime(const wchar_t *file_name)
	{
		const bcl::time24 errTime(0,0,0,0,0,0);
		if(!IsExist(file_name))	return errTime;
		struct _stat st;
		const int ret = _wstat(file_name, &st);
		if(ret != 0) 	return errTime;

		return bcl::time24(st.st_mtime);
	};
#else
	inline void FindDef(const char *searchDef, std::vector<std::string> *pList)
	{
		long hFind;
		struct _finddata_t dt;
		pList->clear();

		hFind = _findfirst(searchDef, &dt);
		if (hFind != -1) {
			do {
				if (strcmp(dt.name, ".") == 0 || strcmp(dt.name, "..") == 0)
					continue;

				if (dt.attrib == _A_SUBDIR) {
					continue;
				} else {
					pList->push_back(bcl::format(L"%s%s", bcl::FilePath(searchDef).DirPath().c_str(), dt.name));
				}
			} while (_findnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
	};
	inline const int RemoveDef(const char *searchDef)
	{
		long hFind;
		struct _finddata_t dt;
		int out = 0;

		hFind = _findfirst(searchDef, &dt);
		if (hFind != -1) {
			do {
				if (strcmp(dt.name, ".") == 0 || strcmp(dt.name, "..") == 0)
					continue;

				if (dt.attrib == _A_SUBDIR) {
					continue;
				} else {
					bcl::File::Delete(bcl::format(L"%s%s", bcl::FilePath(searchDef).DirPath().c_str(), dt.name).c_str());
					out++;
				}
			} while (_findnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return out;
	};
	inline const bcl::time24	mTime(const char *file_name)
	{
		const bcl::time24 errTime(0,0,0,0,0,0);
		if(!IsExist(file_name))	return errTime;
		struct _stat st;
		const int ret = _stat(file_name, &st);
		if(ret != 0) 	return errTime;

		return bcl::time24(st.st_mtime);
	};
	inline const bcl::time24	mTime(const wchar_t *file_name)
	{
		const bcl::time24 errTime(0,0,0,0,0,0);
		if(!IsExist(file_name))	return errTime;
		struct _stat st;
		const int ret = _wstat(file_name, &st);
		if(ret != 0) 	return errTime;

		return bcl::time24(st.st_mtime);
	};
#endif
};
namespace FileSystem{
	//ファイルからフォルダ指定でのコピー・移動

	inline const int CopyFileToDir(const char_t *spath, const char_t *toDir){
		return File::Copy(spath, bcl::format(L"%s\\%s", toDir, bcl::FilePath(spath).FileNameExt().c_str()).c_str());
	}
	inline const int MoveFileToDir(const char_t *spath, const char_t *toDir){
		return File::Move(spath, bcl::format(L"%s\\%s", toDir, bcl::FilePath(spath).FileNameExt().c_str()).c_str());
	}
};	//FileSystem::

} //bcl::

#endif // __BCL_FILESYSTEM_H__
