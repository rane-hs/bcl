// FileSystem.h: �t�@�C���E�t�H���_����
//	�������Boost::FileSystem�ɐ؂�ւ��Ă����\��ł���
//////////////////////////////////////////////////////////////////////

#ifndef __BCL_FILESYSTEM_H__
#define __BCL_FILESYSTEM_H__

#include <bcl/bclstr.h>
#include <string>
#include <vector>
#include <fstream>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <windows.h>

namespace bcl{ //bcl::

	class FilePathW{
		std::wstring drv, dir, fname, ext;
	public:
		FilePathW(const std::wstring &drv_, const std::wstring &dir_, const std::wstring &fname_, const std::wstring &ext_):
		  drv(drv_),dir(dir_),fname(fname_),ext(ext_){}
		FilePathW(const std::wstring &fPath){
			wchar_t fpath_buf[2048];
			_wfullpath(fpath_buf, fPath.c_str(), 2048);
			wchar_t	l_drv[32];
			wchar_t	l_dir[512];
			wchar_t	l_fname[256];
			wchar_t	l_ext[32];
			int retCode = _wsplitpath_s(fpath_buf, l_drv, 32, l_dir, 512, l_fname, 256, l_ext, 32);
			drv = l_drv;
			dir = l_dir;
			fname = l_fname;
			ext = l_ext;
		};
		const std::wstring Fname() const {		return fname;	}
		const std::wstring Ext() const {		return ext;	}
		const std::wstring Drv() const {		return drv;	}
		const std::wstring Dir() const {		return dir;	}
		const std::wstring DirPath() const {	return drv+dir;	}
		const std::wstring FileNameExt() const {	return fname+ext;	}
	};
	class FilePathA{
		std::string drv, dir, fname, ext;
	public:
		FilePathA(const std::string &drv_, const std::string &dir_, const std::string &fname_, const std::string &ext_):
		  drv(drv_),dir(dir_),fname(fname_),ext(ext_){}
		FilePathA(const std::string &fPath){
			char	fpath_buf[2048];
			_fullpath(fpath_buf, fPath.c_str(), 2048);
			char	l_drv[32];
			char	l_dir[512];
			char	l_fname[256];
			char	l_ext[32];
#	if defined(_MSC_VER) && (_MSC_VER > 1200)	// > VC6
			int retCode = _splitpath_s(fpath_buf, l_drv, 32, l_dir, 512, l_fname, 256, l_ext, 32);
#	else
			_splitpath(fullPath.c_str(), l_drv, l_dir, l_fname, l_ext);
#	endif
			drv = l_drv;
			dir = l_dir;
			fname = l_fname;
			ext = l_ext;
		};
		const std::string Fname() const {		return fname;	}
		const std::string Ext() const {		return ext;	}
		const std::string Drv() const {		return drv;	}
		const std::string Dir() const {		return dir;	}
		const std::string DirPath() const {	return drv+dir;	}
		const std::string FileNameExt() const {	return fname+ext;	}
	};
#ifdef _UNICODE
	typedef FilePathW FilePath;
#else
	typedef FilePathA FilePath;
#endif

inline const bcl::str_t DirPath(const bcl::str_t &fullpath){
	return bcl::FilePath(fullpath).DirPath();
};
inline const bcl::str_t FileNameExt(const bcl::str_t &fullpath){
	return bcl::FilePath(fullpath).FileNameExt();
};

typedef intptr_t findHandle;


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

	// �t�@�C���폜
	inline const bool Delete(const wchar_t *fpath)
	{
		return ::DeleteFileW(fpath)==TRUE?true:false;
	};
	inline const bool Delete(const std::wstring &fpath)
	{
		return Delete(fpath.c_str());
	};
	inline const bool Delete(const char *fpath)
	{
		return ::DeleteFileA(fpath)==TRUE?true:false;
	};
	inline const bool Delete(const std::string &fpath)
	{
		return Delete(fpath.c_str());
	};
	/********************************************************************
	*	�֐��FcopyFile
	*	�@�\�F�t�@�C���R�s�[
	*	�����Fspath             [IN]  �R�s�[���t�@�C��
			�Ffpath             [IN]  �R�s�[��t�@�C��
	*	�ߒl�F����
			�F   0 = ����
			�F  -1 = �R�s�[���t�@�C�������݂��Ȃ�
			�F  -2 = �ҏW�̈�̊m�ێ��s
			�F  -3 = �R�s�[���t�@�C���I�[�v�����s
			�F  -4 = �R�s�[��t�@�C���I�[�v�����s
	*	�����F
	********************************************************************/
	//copy
	inline const char Copy(const wchar_t *spath, const wchar_t *fpath)
	{
		return ::CopyFileW(spath, fpath, false);
	}
	inline const char Copy(const std::wstring &from, const std::wstring &to)
	{
		return Copy(from.c_str(), to.c_str());
	}	inline const char Copy(const char *spath, const char *fpath)
	{
		return ::CopyFileA(spath, fpath, false);
	};
	inline const char Copy(const std::string &from, const std::string &to)
	{
		return Copy(from.c_str(), to.c_str());
	}
	//move
	inline const int Move(const char *spath, const char *fpath)
	{
		BOOL nRet = Copy(spath, fpath);
		if(nRet == FALSE)
			return -1;
		return DeleteFileA(spath);
	};
	inline const int Move(const std::string &spath, const std::string &fpath)
	{
		return Move(spath.c_str(), fpath.c_str());
	};
	inline const int Move(const wchar_t *spath, const wchar_t *fpath)
	{
		BOOL nRet = Copy(spath, fpath);
		if(nRet == FALSE)
			return -1;
		return DeleteFileW(spath);
	};
	inline const int Move(const std::wstring &spath, const std::wstring &fpath)
	{
		return Move(spath.c_str(), fpath.c_str());
	};

	//�t�@�C���̑��݃`�F�b�N
	inline const bool IsExist(const std::wstring &fpath)
	{
		OFSTRUCT	ob;
		ob.cBytes = sizeof(OFSTRUCT);
		HANDLE		hdl = CreateFileW(fpath.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == hdl)
			return false;
		else{
			CloseHandle(hdl);
			return true;
		}
	};
	inline const bool IsExist(const std::string &fpath)
	{
		OFSTRUCT	ob;
		ob.cBytes = sizeof(OFSTRUCT);
		HANDLE		hdl = CreateFileA(fpath.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == hdl)
			return false;
		else{
			CloseHandle(hdl);
			return true;
		}
	};

	/********************************************************************
	  �֐�  �F IsExistDef
	  �@�\  �F �t�H���_���t�@�C���폜
	  ����  �F fpath             [IN]  �����t�H���_
	  �߂�l�F	TRUE�F����
				FALSE�F���s
	  ����  �F �w�肳�ꂽ���������Ƀq�b�g����t�@�C��������ꍇ�Atrue��Ԃ��܂�
			�F TRUE�ł͂Ȃ��̂Œ���
	********************************************************************/
	inline const bool IsExistDef(const char *searchDef)
	{
		findHandle hFind;
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
	inline const bool IsExistDef(const std::string &searchDef)
	{
		return IsExistDef(searchDef.c_str());
	}
	/********************************************************************
	  �֐�  �F FindFileDef
	  �@�\  �F �t�H���_���t�@�C��
	  ����  �F fpath             [IN]  �����t�H���_
	  �߂�l�F	TRUE�F����
				FALSE�F���s
	  ����  �F �w�肳�ꂽ���������Ƀq�b�g����t�@�C��������ꍇ�A�t���p�X��Ԃ��܂�
	********************************************************************/
	inline const std::wstring FindDef(const wchar_t *searchDef)
	{
		findHandle hFind;
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
					return bcl::format(L"%s%s", bcl::FilePathW(searchDef).DirPath().c_str(), dt.name);
				}
			} while (_wfindnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return L"";
	};
	inline const std::wstring FindDef(const std::wstring &searchDef)
	{	return FindDef(searchDef.c_str()); }
	inline const std::string FindDef(const char *searchDef)
	{
		findHandle hFind;
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
					return bcl::format("%s%s", bcl::FilePathA(searchDef).DirPath().c_str(), dt.name);
				}
			} while (_findnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return "";
	};
	inline const std::string FindDef(const std::string &searchDef)
	{	return FindDef(searchDef.c_str()); }

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
					pList->push_back(bcl::format(L"%s%s", bcl::FilePathW(searchDef).DirPath().c_str(), dt.name));
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
					bcl::File::Delete(bcl::format(L"%s%s", bcl::FilePathW(searchDef).DirPath().c_str(), dt.name).c_str());
					out++;
				}
			} while (_wfindnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return out;
	};
	inline const time_t	mTime(const char *file_name)
	{
		const time_t errTime(0);
		if(!IsExist(file_name))	return errTime;
		struct _stat st;
		const int ret = _stat(file_name, &st);
		if(ret != 0) 	return errTime;

		return st.st_mtime;
	};
	inline const time_t	mTime(const wchar_t *file_name)
	{
		const time_t errTime(0);
		if(!IsExist(file_name))	return errTime;
		struct _stat st;
		const int ret = _wstat(file_name, &st);
		if(ret != 0) 	return errTime;

		return st.st_mtime;
	};

	inline void FindDef(const char *searchDef, std::vector<std::string> *pList)
	{
		findHandle hFind;
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
					pList->push_back(bcl::format("%s%s", bcl::FilePathA(searchDef).DirPath().c_str(), dt.name));
				}
			} while (_findnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
	};
	inline const int RemoveDef(const char *searchDef)
	{
		findHandle hFind;
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
					File::Delete(bcl::format("%s%s", bcl::FilePathA(searchDef).DirPath().c_str(), dt.name));
					out++;
				}
			} while (_findnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return out;
	};
	inline const int RemoveDef(const std::string &searchDef){ return RemoveDef(searchDef.c_str()); }

};

namespace Dir{

	// �f�B���N�g���m�F
	inline const bool IsExist(const wchar_t *fpath)
	{
		// �p�X�̕���
		const bcl::FilePathW detailedPath(fpath);
		const std::wstring ext = detailedPath.Ext();
		std::wstring drv_dir =detailedPath.DirPath();

		// �g���q���Ȃ���΁A�t�@�C�������t�H���_�Ƃ���
		if (ext.length() < 1)
			drv_dir += detailedPath.Fname();

		if(drv_dir.length() < 1) return false;

		// �f�B���N�g���̍Ō��'\'������
		const wchar_t lastChar = drv_dir[drv_dir.length()-1];
		if ((lastChar == L'\\') || (lastChar == L'/'))
			drv_dir = drv_dir.substr(0, drv_dir.length()-1);

		// �f�B���N�g���̌���
		WIN32_FIND_DATAW	pFindFileData;
		HANDLE			hFind = FindFirstFileW(drv_dir.c_str(), &pFindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
			return false;

		FindClose(hFind);
		return true;
	};
	inline bool	IsExist(const str_t &fpath){ return IsExist(fpath.c_str());};

	inline bool	IsExist(const char *fpath){ 
		// �p�X�̕���
		const bcl::FilePathA detailedPath(fpath);
		const std::string ext = detailedPath.Ext();
		std::string drv_dir =detailedPath.DirPath();

		// �g���q���Ȃ���΁A�t�@�C�������t�H���_�Ƃ���
		if (ext.length() < 1)
			drv_dir += detailedPath.Fname();

		if(drv_dir.length() < 1) return false;

		// �f�B���N�g���̍Ō��'\'������
		const char_t lastChar = drv_dir[drv_dir.length()-1];
		if ((lastChar == '\\') || (lastChar == '/'))
			drv_dir = drv_dir.substr(0, drv_dir.length()-1);

		// �f�B���N�g���̌���
		WIN32_FIND_DATAA	pFindFileData;
		HANDLE			hFind = FindFirstFileA(drv_dir.c_str(), &pFindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
			return false;

		FindClose(hFind);
		return true;
	};

	// �f�B���N�g���쐬
	inline const char Make(const wchar_t *fpath)
	{
		if(IsExist(fpath)) return 0;

		//�p�X��\\��/�ɒu������
		std::wstring strPath = fpath;
		bcl::replace(strPath, L"\\", L"/");

		// �p�X�̕���
		FilePathW	dividedPath(strPath);
		//�t�H���_�Ȃ�
		if( dividedPath.Dir().length() < 1) 
			return 0;
		//�g���q�̗L�����`�F�b�N
		std::wstring	mkPath;
		if (dividedPath.Ext().length() < 1) {
			mkPath = dividedPath.Drv() + dividedPath.Dir() + dividedPath.Fname();
		}else{
			// �f�B���N�g���̍Ō��'/'������
			std::wstring	dirBuf = dividedPath.Dir();
			if(dirBuf[dirBuf.length() - 1] == '\\')
				dirBuf = dirBuf.substr(0, dirBuf.length() -1 );
			mkPath = dividedPath.Drv() + dirBuf;
		}

		//�t�@�C���p�X���ŏ�����{�����t�H���_���K�w���Ƃɐ�������
		for(size_t i=0;i<mkPath.length(); i++){
			if (i >= 1) {
				if ((mkPath[i-1] > 0) && (mkPath[i] == '\\'))
					int ret = CreateDirectoryW(mkPath.substr(0, i).c_str(), NULL);
			}
		}
		int ret = CreateDirectoryW(mkPath.c_str(), NULL);
		if (ret == -1)
			return -1;

		return 0;
	}
	inline const char Make(const char *fpath)
	{
		if(IsExist(fpath)) return 0;

		//�p�X��\\��/�ɒu������
		std::string strPath = fpath;
		bcl::replace(strPath, "\\", "/");

		// �p�X�̕���
		FilePathA	dividedPath(strPath);
		//�t�H���_�Ȃ�
		if( dividedPath.Dir().length() < 1) 
			return 0;
		//�g���q�̗L�����`�F�b�N
		std::string	mkPath;
		if (dividedPath.Ext().length() < 1) {
			mkPath = dividedPath.Drv() + dividedPath.Dir() + dividedPath.Fname();
		}else{
			// �f�B���N�g���̍Ō��'/'������
			std::string	dirBuf = dividedPath.Dir();
			if(dirBuf[dirBuf.length() - 1] == '\\')
				dirBuf = dirBuf.substr(0, dirBuf.length() -1 );
			mkPath = dividedPath.Drv() + dirBuf;
		}

		//�t�@�C���p�X���ŏ�����{�����t�H���_���K�w���Ƃɐ�������
		for(size_t i=0;i<mkPath.length(); i++){
			if (i >= 1) {
				if ((mkPath[i-1] > 0) && (mkPath[i] == '\\'))
					int ret = CreateDirectoryA(mkPath.substr(0, i).c_str(), NULL);
			}
		}
		int ret = CreateDirectoryA(mkPath.c_str(), NULL);
		if (ret == -1)
			return -1;

		return 0;
	}
	inline const char Make(const std::string &fpath){ return Make(fpath.c_str());};
	inline const char Make(const std::wstring &fpath){ return Make(fpath.c_str());};
	// �f�B���N�g���`�F�b�N
	inline const char Check(const char *fpath)
	{
		char ret = 0;

		if (!IsExist(fpath)) {
			// �f�B���N�g���쐬
			ret = Make(fpath);
		}
		return ret;
	};
	inline const char Check(const std::string &fpath){ return Check(fpath.c_str());}
	inline const char Delete(const wchar_t *dirpath)
	{
		if (!Dir::IsExist(dirpath)) {
			return 1;
		}
		if(-1 == _wrmdir(dirpath))
			return -1;
		else
			return 0;
	};
	inline const char Delete(const std::wstring &dirpath){ return Delete(dirpath.c_str()); }
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
	inline const char Delete(const std::string &dirpath){ return Delete(dirpath.c_str()); }
	inline const char Clear(const wchar_t *fpath)
	{
		intptr_t hFind;
		struct _wfinddata_t dt;

		//���݊m�F
		if(IsExist(fpath) == false)
			return false;
		//�t�@�C���̌���
		const std::wstring searchDef = bcl::format(L"%s\\*", fpath);

		hFind = _wfindfirst(searchDef.c_str(), &dt);
		if (hFind != -1) {
			do {
				if (wcscmp(dt.name, L".") == 0 || wcscmp(dt.name, L"..") == 0)
					continue;

				const std::wstring delPath = bcl::format(L"%s\\%s", fpath, dt.name);
				if (dt.attrib == _A_SUBDIR) {
					Dir::Clear(delPath.c_str());
					Dir::Delete(delPath.c_str());
				} else {
					bcl::File::Delete(delPath);
				}
			} while (_wfindnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
		return TRUE;
	};
	inline const char Clear(const std::wstring &fpath){ return Clear(fpath.c_str());}
	inline const char Clear(const char *fpath)
	{
		intptr_t hFind;
		struct _finddata_t dt;

		//���݊m�F
		if(IsExist(fpath) == false)
			return false;
		//�t�@�C���̌���
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
	inline const char Clear(const std::string &fpath){ return Clear(fpath.c_str());}
	inline const bool IsExistDef(const char *searchDef)
	{
		findHandle hFind;
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
	inline void DeleteDef(const std::string &searchDef)
	{
		findHandle hFind;
		const FilePathA	detailedPath(searchDef);
		struct _finddata_t dt;
		hFind = _findfirst(searchDef.c_str(), &dt);
		if (hFind != -1) {
			do {
				if (strcmp(dt.name, ".") == 0 || strcmp(dt.name, "..") == 0)
					continue;

				if (dt.attrib & _A_SUBDIR) {
					Delete(bcl::format("%s\\%s", detailedPath.DirPath().c_str(), dt.name).c_str());
				}
			} while (_findnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
	}

	inline void DeleteDef(const std::wstring &searchDef)
	{
		findHandle hFind;
		const FilePathW	detailedPath(searchDef);

		struct _wfinddata_t dt;
		hFind = _wfindfirst(searchDef.c_str(), &dt);

		if (hFind != -1) {
			do {
				if (wcscmp(dt.name, L".") == 0 || wcscmp(dt.name, L"..") == 0)
					continue;

				if (dt.attrib & _A_SUBDIR) {
					Delete(bcl::format("%s\\%s", detailedPath.DirPath().c_str(), dt.name).c_str()
							);
				}
			} while (_wfindnext(hFind, &dt) == 0);
			_findclose(hFind);
		}
	}
};	//Dir::
namespace FileSystem{
	//�t�@�C������t�H���_�w��ł̃R�s�[�E�ړ�

	inline const int CopyFileToDir(const wchar_t *spath, const wchar_t *toDir){
		return bcl::File::Copy(spath, bcl::format(L"%s\\%s", toDir, bcl::FilePathW(spath).FileNameExt().c_str()).c_str());
	}
	inline const int MoveFileToDir(const wchar_t *spath, const wchar_t *toDir){
		return bcl::File::Move(spath, bcl::format(L"%s\\%s", toDir, bcl::FilePathW(spath).FileNameExt().c_str()).c_str());
	}

	inline const int CopyFileToDir(const char *spath, const char *toDir){
		return bcl::File::Copy(spath, bcl::format("%s\\%s", toDir, bcl::FilePathA(spath).FileNameExt().c_str()).c_str());
	}
	inline const int MoveFileToDir(const char *spath, const char *toDir){
		return bcl::File::Move(spath, bcl::format("%s\\%s", toDir, bcl::FilePathA(spath).FileNameExt().c_str()).c_str());
	}

};	//FileSystem::

} //bcl::

#endif // __BCL_FILESYSTEM_H__
