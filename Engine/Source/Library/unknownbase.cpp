//#include "unknownbase.h"
//
//#pragma warning(disable:4355)
//
////�R���X�g���N�^
//CUnknownBase::CUnknownBase(IUnknown *lpUnknown)
//	: m_lRefCount(0), m_lpUnknown(lpUnknown != NULL ? lpUnknown : reinterpret_cast<IUnknown *>(static_cast<INonDelegatingUnknown *>(this)))
//{
//
//}
//
////�f�X�g���N�^
//CUnknownBase::~CUnknownBase()
//{
//
//}
//
////�C���^�[�t�F�C�X��v������
//STDMETHODIMP CUnknownBase::NonDelegatingQueryInterface(REFIID riid, void **ppv)
//{
//	//�|�C���^�`�F�b�N
//	CheckPointer(ppv, E_POINTER);
//	ValidateReadWritePtr(ppv, sizeof(PVOID));
//
//	//IUnknown�̂݃C���^�[�t�F�C�X�̎󂯌p�����s��
//	if (IsEqualIID(riid, IID_IUnknown)){
//		GetInterface(reinterpret_cast<IUnknown *>(static_cast<INonDelegatingUnknown *>(this)), ppv);
//		return S_OK;
//	}
//	else{
//		*ppv = NULL;
//		return E_NOINTERFACE;
//	}
//}
//
////�Q�ƃJ�E���g�p�̑傫�����̒l��Ԃ��e���v���[�g�֐�
//template <class T> inline static T ourmax(const T &a, const T &b)
//{
//	return a > b ? a : b;
//}
//
////�Q�ƃJ�E���g�̑���
//STDMETHODIMP_(ULONG) CUnknownBase::NonDelegatingAddRef(void)
//{
//	long lRef = InterlockedIncrement(&m_lRefCount);
//	return ourmax((ULONG)lRef, 1ul);
//}
//
////�Q�ƃJ�E���g�̌���
//STDMETHODIMP_(ULONG) CUnknownBase::NonDelegatingRelease(void)
//{
//	long lRef = InterlockedDecrement(&m_lRefCount);
//	if (lRef == 0){ m_lRefCount++; delete this; return (ULONG)0; }
//	else return ourmax((ULONG)lRef, 1ul);
//}
//
////�C���^�[�t�F�C�X�̕ϊ����Ɏg�p����⏕�֐�
//STDAPI GetInterface(IUnknown *lpUnknown, void **ppv)
//{
//	CheckPointer(ppv, E_POINTER);
//	*ppv = lpUnknown; lpUnknown->AddRef();
//	return S_OK;
//}
//
//STDAPI GetInterfaceEx(INonDelegatingUnknown *lpUnknown, void **ppv)
//{
//	CheckPointer(ppv, E_POINTER);
//	*ppv = lpUnknown; lpUnknown->NonDelegatingAddRef();
//	return S_OK;
//}