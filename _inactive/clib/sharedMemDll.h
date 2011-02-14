#ifndef _SHAREDMEMORYDLL_
#define _SHAREDMEMORYDLL_

#ifdef  __cplusplus
extern "C" {
#endif

#define DllExport				__stdcall

#define DF_CONSOLECLASS  "ConsoleWindowClass"

typedef struct {
	HANDLE		hd;		/* ���L�������n���h�� */
	char		*mem;	/* ���L�������擪�|�C���^(�e���ŃL���X�g����) */
} sharedMem_t;

/*
 *
 *
 *		�v���g�^�C�v�錾
 *
 */
/*
 * �@�\		�F���L�������쐬
 *
 * ����		�F[OUT] pt	 - ���L�������n���h���|�C���^
 *			  [IN] nst   - ���L��������
 *			  [IN] len	 - �T�C�Y(�T�C�Y)
 *
 * �߂�l	�F[ 0] ����
 *			  [-1] ID�擾�G���[
 *                ���ӁI
 *                     ���L�������n���h���̈�(sharedMem_t)�̎��̈�́A�Ăяo������
 *                     �m�ۂ��鎖�B
 * ����		�F
 */
int	DllExport dll_createSharedMemory(sharedMem_t *, char *, int);
/*
 * �@�\		�F���L�������I�[�v��
 *
 * ����		�F[OUT] pt	 - ���L�������n���h���|�C���^
 *			  [IN] nst   - ���L��������
 *
 * �߂�l	�F[0]  ����
 *			  [-1] �̈�m�ێ��s, ID�l���G���[, �|�C���^�l���G���[
 *
 * ����		�F
 */
int DllExport dll_openSharedMemory(sharedMem_t *, char *);
/*
 * �@�\		�F���L���������
 *
 * ����		�F[IN] pt	 - ���L�������n���h���|�C���^
 *
 * �߂�l	�F[ 0] �A���}�b�v����
 *			  [-1] �A���}�b�v���s
 *
 * ����		�F
 */
int DllExport dll_freeSharedMemory(sharedMem_t *);
/*
 * �@�\		�FBYTE��������
 *
 * ����		�F[IN] pt	  - ���L�������n���h���|�C���^
 *            [IN] offset - �擪�|�C���^����̋���(BYTE)
 *            [IN] buf    - �������ޒl
 *
 * �߂�l	�F�Ȃ�
 *
 * ����		�F
 */
void DllExport dll_setByte(sharedMem_t *, int, char);
/*
 * �@�\		�FBYTE�ǂݍ���
 *
 * ����		�F[IN]  pt	   - ���L�������n���h���|�C���^
 *            [IN]  offset - �擪�|�C���^����̋���(BYTE)
 *            [OUT] buf    - �ǂݍ��ݒl�i�[�̈�
 *
 * �߂�l	�F�Ȃ�
 *
 * ����		�F
 */
void DllExport dll_getByte(sharedMem_t *, int, char *);
/*
 * �@�\		�FWORD��������
 *
 * ����		�F[IN] pt	  - ���L�������n���h���|�C���^
 *            [IN] offset - �擪�|�C���^����̋���(BYTE)
 *            [IN] buf    - �������ޒl
 *
 * �߂�l	�F�Ȃ�
 *
 * ����		�F
 */
void DllExport dll_setWord(sharedMem_t *, int, WORD);
/*
 * �@�\		�FWORD�ǂݍ���
 *
 * ����		�F[IN]  pt	   - ���L�������n���h���|�C���^
 *            [IN]  offset - �擪�|�C���^����̋���(BYTE)
 *            [OUT] buf    - �ǂݍ��ݒl�i�[�̈�
 *
 * �߂�l	�F�Ȃ�
 *
 * ����		�F
 */
void DllExport dll_getWord(sharedMem_t *, int, WORD *);
/*
 * �@�\		�FLONG��������
 *
 * ����		�F[IN] pt	  - ���L�������n���h���|�C���^
 *            [IN] offset - �擪�|�C���^����̋���(BYTE)
 *            [IN] buf    - �������ޒl
 *
 * �߂�l	�F�Ȃ�
 *
 * ����		�F
 */
void DllExport dll_setLong(sharedMem_t *, int, LONG);
/*
 * �@�\		�FLONG��������
 *
 * ����		�F[IN]  pt	   - ���L�������n���h���|�C���^
 *            [IN]  offset - �擪�|�C���^����̋���(BYTE)
 *            [OUT] buf    - �ǂݍ��ݒl�i�[�̈�
 *
 * �߂�l	�F�Ȃ�
 *
 * ����		�F
 */
void DllExport dll_getLong(sharedMem_t *, int, LONG *);
/*
 * �@�\		�F�w��o�C�g��������
 *
 * ����		�F[IN] pt	  - ���L�������n���h���|�C���^
 *            [IN] offset - �擪�|�C���^����̋���(BYTE)
 *            [IN] len    - �������݃T�C�Y(BYTE)
 *            [IN] buf    - �������ޒl
 *
 * �߂�l	�F�Ȃ�
 *
 * ����		�F
 */
void DllExport dll_setSharedMemorys(sharedMem_t *, int, int, char *);
/*
 * �@�\		�F�w��o�C�g�ǂݍ���
 *
 * ����		�F[IN] pt	  - ���L�������n���h���|�C���^
 *            [IN] offset - �擪�|�C���^����̋���(BYTE)
 *            [IN] len    - �ǂݍ��݃T�C�Y(BYTE)
 *            [IN] buf    - �ǂݍ��ݒl�i�[�̈�
 *
 * �߂�l	�F�Ȃ�
 *
 * ����		�F
 */
void DllExport dll_getSharedMemorys(sharedMem_t *, int, int, char *);
/*
 * �@�\		�F�R���\�[���̃E�B���h�E�n���h���擾
 *
 * ����		�F[IN] lpszWindow - �E�B���h�E�^�C�g����
 *
 * �߂�l	�F�E�B���h�E�n���h��
 *
 * ����		�F�{�֐��́A�R���\�[���A�v���P�[�V�����̋N������ɓǂݍ���ł��������B
 */
HWND DllExport dll_getConsoleHandle();
/*
 * �@�\		�F�E�B���h�E��\��
 *
 * ����		�F[IN] hwnd - �E�B���h�E�n���h��
 *
 * �߂�l	�F[ TRUE] - �E�B���h�E���ȑO�ɕ\������Ă����ꍇ
 *					[FALSE] - ��\���������ꍇ
 *
 * ����		�F
 */
BOOL DllExport dll_showWindow(HWND);
/*
 * �@�\		�F�E�B���h�E��\��
 *
 * ����		�F[IN] hwnd - �E�B���h�E�n���h��
 *
 * �߂�l	�F[ TRUE] - �E�B���h�E���ȑO�ɕ\������Ă����ꍇ
 *					[FALSE] - ��\���������ꍇ
 *
 * ����		�F
 */
BOOL DllExport dll_hideWindow(HWND);


#ifdef  __cplusplus
}
#endif

#endif


