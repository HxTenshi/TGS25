//#include <windows.h>
////#include <objbase.h>
////#include <process.h>
//#include <tchar.h>
//
//#include <mfapi.h>
//#include <mfidl.h>
//#include <mferror.h>
//#include <evr.h>
//#include <mbstring.h>
//
//#include "../unknownbase.h"
//
//#pragma comment(lib,"mf.lib")
//#pragma comment(lib,"mfplat.lib")
//#pragma comment(lib,"mfuuid.lib")
//#pragma comment(lib,"strmiids.lib")
//
////���[�h���ꂽ���f�B�A��ʂ�񋓂���
//typedef struct _mediacontenttype{
//	const TCHAR *ext;
//	const WCHAR *type;
//} MEDIACONTENTTYPE;
//
////�X�e�[�^�X�R�[�h
//#define MFSSTATUS_NONE				0x00000000
//#define MFSSTATUS_LOAD				0x00000001
//#define MFSSTATUS_PLAY				0x00000002
//#define MFSSTATUS_PAUSE				0x00000004
//#define MFSSTATUS_LOOP				0x00000008
//#define MFSSTATUS_HASAUDIOLINE			0x00000100
//#define MFSSTATUS_HASVIDEOLINE			0x00000200
//
////���s����Goto����
//#define FAILED_BREAK(hr,x)		{ (hr) = (x); if(FAILED(hr)){ break; } }
//
//
//static const MEDIACONTENTTYPE g_avMediaType[] = {
//		{ TEXT("mp4"), L"video/mp4" },
//		{ TEXT("wmv"), L"video/x-ms-wmv" },
//		{ TEXT("avi"), L"video/x-msvideo" },
//		{ NULL, NULL }
//};
//#ifdef _UNICODE
////#define _istlead(c)	0
//#define _isttrail(c)	0
//#else
////#define _istlead(c)		_ismbblead(c)
//#define _isttrail(c)	_ismbbtrail(c)
//#endif //_UNICODE
//
//
//
//#ifndef __mfsession_h__
//#define __mfsession_h__
//
//#define WM_MFSNOTIFY					(WM_USER + 0x1001)
//
//#define MFS_VOLUMEMAX					1000
//#define MFS_VOLUMEMIN					0
//#define MFS_PANLEFT					-(MFS_VOLUMEMAX)
//#define MFS_PANCENTER					MFS_VOLUMEMIN
//#define MFS_PANRIGHT					MFS_VOLUMEMAX
//
//typedef int int32_t;
//typedef __int64 int64_t;
//typedef unsigned int uint32_t;
//typedef unsigned __int64 uint64_t;
//
////���f�B�A�Z�b�V�����Ǘ�
//class CMFSession : public CUnknownBase, public IMFAsyncCallback{
//public:
//	CMFSession(HWND hWnd); //�R���X�g���N�^
//	virtual ~CMFSession(); //�f�X�g���N�^
//
//	//IUnknown�̋@�\����������
//	DECLARE_IUNKNOWN;
//	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv); //�C���^�[�t�F�C�X�̗v��
//
//	//IMFAsyncCallback�̎���
//	STDMETHODIMP GetParameters(DWORD *pdwFlags, DWORD *pdwQueue); //�����p�����[�^���擾����
//	STDMETHODIMP Invoke(IMFAsyncResult *pAsyncResult); //�񓯊��������s��ꂽ�Ƃ��̃R�[���o�b�N
//
//	virtual HRESULT LoadMovie(const TCHAR *lpFileName); //���f�B�A��ǂݍ���
//	virtual BOOL ReleaseMovie(void); //�������
//	virtual BOOL PlayMovie(BOOL bIsLoop = FALSE); //�Đ�
//	virtual BOOL StopMovie(void); //��~
//	virtual BOOL IsPlayMovie(void) const; //�Đ������ǂ���
//	virtual BOOL PauseMovie(void); //�ꎞ��~
//	virtual BOOL ForwardMovie(uint32_t nTime); //������(ms)
//	virtual BOOL BackwardMovie(uint32_t nTime); //�����߂�(ms)
//	virtual BOOL SeekMovie(uint32_t nTime); //�Đ��ʒu�̈ړ�(ms)
//	virtual BOOL SeekMovieRel(int64_t llTime); //�Đ��ʒu�̑��Έړ�(100ns)
//	virtual BOOL SeekMovieAbs(uint64_t ullTime); //�Đ��ʒu�̐�Έړ�(100ns)
//
//	virtual BOOL SetSpeed(double dSpeed); //�X�s�[�h�̕ύX
//	virtual BOOL SetPan(int nAbsolutePan); //�p���̕ω���ݒ肷��
//	virtual BOOL SetVolume(int nAbsoluteVolume); //���ʂ̐ݒ������
//
//	virtual uint32_t GetMovieTime(void) const; //���f�B�A�̍Đ����Ԃ��擾
//	virtual uint64_t GetMediaTime(void) const; //���f�B�A���ԂōĐ����Ԃ��擾
//	virtual BOOL GetMovieSize(int32_t *lpnWidth, int32_t *lpnHeight) const; //���f�B�A�̉摜�̃T�C�Y���擾
//	virtual BOOL HasMediaVideo(void) const; //���f�B�A���f�����C����ێ����Ă��邩�ǂ���
//	virtual BOOL HasMediaAudio(void) const; //���f�B�A���������C����ێ����Ă��邩�ǂ���
//
//	virtual BOOL SetPlayWindow(const POINT *lpPlayPos = NULL, const SIZE *lpPlaySize = NULL); //�Đ��E�B���h�E�̐ݒ�
//	virtual BOOL Repaint(void); //�ĕ`����s��
//	virtual BOOL HandleEvent(LPARAM lParam); //�C�x���g�������s
//
//protected:
//	HRESULT SetByteStreamContentType(const TCHAR *lpFileName, const TCHAR *lpContentType); //ByteStream��ContentType��ݒ肷��
//	HRESULT CreateSourceNode(IMFPresentationDescriptor *lpPresentDesc, IMFStreamDescriptor *lpStreamDesc, IMFTopologyNode **lplpNode); //�\�[�X���̃g�|���W�[�m�[�h���쐬����
//	HRESULT CreateOutputNode(IMFStreamDescriptor *lpStreamDesc, IMFTopologyNode **lplpNode); //�o�͕��̃g�|���W�[�m�[�h���쐬����
//	HRESULT InnerSeekTime(MFTIME nTime); //�Đ��ʒu��ݒ肷��
//	HRESULT InnerSetVolume(void); //�{�����[����ݒ肷��
//
//	inline bool IsEnableVideo(void) const; //�r�f�I�L���e�X�g
//	inline bool IsEnableAudio(void) const; //�T�E���h�L���e�X�g
//	inline bool InnerTestStatus(uint32_t nStatus) const; //�����I�ȃX�e�[�^�X�e�X�g
//	static inline uint64_t MilliSecondToMediaTime(uint32_t t); //�~���b�����f�B�A�^�C���ɕϊ�����
//	static inline uint32_t MediaTimeToMilliSecond(uint64_t t); //���f�B�A�^�C�����~���b�ɕϊ�����
//
//private:
//	HWND m_hWnd; //�E�B���h�E�n���h��
//	uint32_t m_nStatusCode; //�X�e�[�^�X�R�[�h
//	MFTIME m_mtMediaLength; //���f�B�A��(100ns)
//	float m_fAudioVolume; //�I�[�f�B�I�{�����[��
//	float m_fAudioBalance; //�I�[�f�B�I�o�����X
//
//	IMFMediaSession *m_lpMediaSession; //���f�B�A�Z�b�V����
//	IMFByteStream *m_lpByteStream; //�o�C�g�X�g���[��
//	IMFMediaSource *m_lpMediaSource; //���f�B�A�\�[�X
//	IMFPresentationClock *m_lpPresentationClock; //�v���[���e�[�V�����N���b�N
//	IMFVideoDisplayControl *m_lpVideoDisplay; //�r�f�I�f�B�X�v���C
//	IMFAudioStreamVolume *m_lpAudioVolume; //�I�[�f�B�I�{�����[��
//};
//
//#endif //__mfsession_h__
