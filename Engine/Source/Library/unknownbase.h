//#ifndef __unknownbase_h__
//#define __unknownbase_h__
//
////���z�֐��e�[�u�������������Ȃ��悤�ɂ���錾
//#ifndef AM_NOVTABLE
//
//#ifdef  _MSC_VER
//#if _MSC_VER >= 1100
//#define AM_NOVTABLE __declspec(novtable)
//#else
//#define AM_NOVTABLE
//#endif
//#endif //MSC_VER
//
//#endif //AM_NOVTABLE
//
//#ifndef INONDELEGATINGUNKNOWN_DEFINED
//DECLARE_INTERFACE(INonDelegatingUnknown)
//{
//	STDMETHOD(NonDelegatingQueryInterface) (THIS_ REFIID, LPVOID *) PURE;
//	STDMETHOD_(ULONG, NonDelegatingAddRef)(THIS)PURE;
//	STDMETHOD_(ULONG, NonDelegatingRelease)(THIS)PURE;
//};
//#define INONDELEGATINGUNKNOWN_DEFINED
//#endif
//
////IUnknown�̋@�\����������
//class AM_NOVTABLE CUnknownBase : public INonDelegatingUnknown
//{
//public:
//	CUnknownBase(IUnknown *lpUnknown); //�R���X�g���N�^
//	virtual ~CUnknownBase(); //�f�X�g���N�^
//
//	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv); //�C���^�[�t�F�C�X��v������
//	STDMETHODIMP_(ULONG) NonDelegatingAddRef(void); //�Q�ƃJ�E���g�̑���
//	STDMETHODIMP_(ULONG) NonDelegatingRelease(void); //�Q�ƃJ�E���g�̌���
//
//	//�I�[�i�[�I�u�W�F�N�g���擾����
//	IUnknown *GetOwner(void) const { return const_cast<IUnknown *>(m_lpUnknown); }
//
//protected:
//	volatile long m_lRefCount; //�Q�ƃJ�E���g
//
//private:
//	//�I�u�W�F�N�g�̃R�s�[�̋֎~
//	CUnknownBase(const CUnknownBase &unk); //�R�s�[�R���X�g���N�^
//	CUnknownBase & operator = (const CUnknownBase &unk); //�R�s�[�I�y���[�^
//
//private:
//	const IUnknown *m_lpUnknown; //IUnknown�C���^�[�t�F�C�X
//};
//
////�C���^�[�t�F�C�X�̕ϊ����Ɏg�p����⏕�֐�
//STDAPI GetInterface(IUnknown *lpUnknown, void **ppv);
//STDAPI GetInterfaceEx(INonDelegatingUnknown *lpUnknown, void **ppv);
//
////�f�o�b�O�`�F�b�N�}�N��
//#define CheckPointer(p,ret)			{ if((p) == NULL){ return (ret); } }
//#define ValidateReadPtr(p,cb)		0
//#define ValidateWritePtr(p,cb)		0
//#define ValidateReadWritePtr(p,cb)	0
//
////IUnknown�̋@�\����������Ƃ��ɍs���}�N��
//#define DECLARE_IUNKNOWN                                        \
//    STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppv) {      \
//        return GetOwner()->QueryInterface(riid,ppv);            \
//    };                                                          \
//    STDMETHODIMP_(ULONG) AddRef() {                             \
//        return GetOwner()->AddRef();                            \
//    };                                                          \
//    STDMETHODIMP_(ULONG) Release() {                            \
//        return GetOwner()->Release();                           \
//    };
//
//
//#endif //__unknownbase_h__