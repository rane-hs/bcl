///////////////////////////////////////////////////////////////////////////
//*	�@�\  �F���O�^�C���N���X
//  �X�V�����F
//          �F
//          �F
//          �F
//          �F
///////////////////////////////////////////////////////////////////////////
#ifndef _CLAG_TIME_H_
#define _CLAG_TIME_H_

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#include "./clib/timeDll.h"

#ifdef  __cplusplus
}
#endif



/*-------------------------------------------------------------------
  �\���́F���O�^�C�����
  ����  �Fflg					���O�^�C���t���O
        �Ftime					�ϑ�����
-------------------------------------------------------------------*/
typedef struct {
	int		flg;				// ���O�^�C���t���O
	time_t	time;				// �ϑ�����(time_t�^)
	tm24_t	time24;				// �ϑ�����(tm24_t�^)
} lag_time_t;

/*-------------------------------------------------------------------
  �N���X��`
-------------------------------------------------------------------*/
class CLagTime {
public:
	lag_time_t	inf;			// ���O�^�C�����
	int			houroNo;		// ���H�ԍ�
	int			lag_minus;		// ���O�^�C��(�|�b)
	int			lag_plus;		// ���O�^�C��(�{�b)

	// �R���X�g���N�^
	CLagTime();
	// �f�X���N�^
	~CLagTime();

	// ������
	int	init(int no, int m1, int m2);
	// ���O�^�C������
	int	judgment();
};

#endif
