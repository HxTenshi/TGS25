//
//#include "hwndMovie.h"
//
////�R���X�g���N�^
//CMFSession::CMFSession(HWND hWnd)
//	: CUnknownBase(NULL), m_hWnd(hWnd), m_nStatusCode(MFSSTATUS_NONE), m_mtMediaLength(0), m_fAudioVolume(1.0f), m_fAudioBalance(0.0f),
//	m_lpMediaSession(NULL), m_lpByteStream(NULL), m_lpMediaSource(NULL), m_lpPresentationClock(NULL), m_lpVideoDisplay(NULL), m_lpAudioVolume(NULL)
//{
//	NonDelegatingAddRef();
//}
//
////�f�X�g���N�^
//CMFSession::~CMFSession()
//{
//	ReleaseMovie();
//}
//
////�C���^�[�t�F�C�X�̗v��
//STDMETHODIMP CMFSession::NonDelegatingQueryInterface(REFIID riid, void **ppv)
//{
//	if (IsEqualIID(riid, IID_IMFAsyncCallback)) return GetInterface(static_cast<IMFAsyncCallback *>(this), ppv);
//	else return CUnknownBase::NonDelegatingQueryInterface(riid, ppv);
//}
//
////���f�B�A��ǂݍ���
//HRESULT CMFSession::LoadMovie(const TCHAR *lpFileName)
//{
//	IMFByteStream *lpByteStream = NULL; IMFSourceResolver *lpSourceResolver = NULL; IUnknown *lpSource = NULL;
//	IMFTopology *lpTopology = NULL; IMFPresentationDescriptor *lpPresentDesc = NULL; IMFClock *lpClock = NULL;
//
//#ifdef _UNICODE
//	const WCHAR *wszFileName;
//#else
//	WCHAR wszFileName[4096];
//#endif
//
//	MF_OBJECT_TYPE objtype; DWORD count; HRESULT hr;
//
//	//���łɓǂݍ��܂�Ă���Ƃ��͉�����s��
//	if (InnerTestStatus(MFSSTATUS_LOAD)){ ReleaseMovie(); }
//
//
//	do{
//		//�e�I�u�W�F�N�g�̏�����
//		m_nStatusCode = MFSSTATUS_NONE;
//
//		//�t�@�C������TCHAR����WCHAR�ւƕϊ�����
//#ifdef _UNICODE
//		wszFileName = lpFileName;
//#else
//		MultiByteToWideChar(CP_ACP, 0, lpFileName, -1, wszFileName, sizeof(wszFileName) / sizeof(WCHAR));
//#endif
//
//		//MediaSession�̍쐬
//		FAILED_BREAK(hr, MFCreateMediaSession(NULL, &m_lpMediaSession));
//
//		//�C�x���g�擾���������g�Ɋ��蓖��
//		FAILED_BREAK(hr, m_lpMediaSession->BeginGetEvent(static_cast<IMFAsyncCallback *>(this), NULL));
//
//		//ByteStream�̍쐬
//		FAILED_BREAK(hr, MFCreateFile(MF_ACCESSMODE_READ, MF_OPENMODE_FAIL_IF_NOT_EXIST, MF_FILEFLAGS_NONE, wszFileName, &m_lpByteStream));
//		FAILED_BREAK(hr, SetByteStreamContentType(lpFileName, NULL));
//
//		//SourceResolver�̍쐬
//		FAILED_BREAK(hr, MFCreateSourceResolver(&lpSourceResolver));
//
//		//IMFMediaSource�̍쐬
//		objtype = MF_OBJECT_INVALID;
//		FAILED_BREAK(hr, lpSourceResolver->CreateObjectFromByteStream(m_lpByteStream, NULL, MF_RESOLUTION_MEDIASOURCE, NULL, &objtype, &lpSource));
//		FAILED_BREAK(hr, lpSource->QueryInterface(IID_PPV_ARGS(&m_lpMediaSource)));
//
//		//IMFTopology�̍쐬
//		FAILED_BREAK(hr, MFCreateTopology(&lpTopology));
//
//		//MediaSource����PresentationDescription���擾����
//		FAILED_BREAK(hr, m_lpMediaSource->CreatePresentationDescriptor(&lpPresentDesc));
//
//		//Description�̐����擾
//		FAILED_BREAK(hr, lpPresentDesc->GetStreamDescriptorCount(&count));
//
//		//�擾���ꂽ�eStream�ɑ΂��ăg�|���W�[�̏�Ԃ����蓖�Ă�
//		for (DWORD i = 0; i < count; i++){
//			IMFStreamDescriptor *lpStreamDesc = NULL; IMFTopologyNode *lpSourceNode = NULL, *lpOutputNode = NULL; BOOL bIsSelected = FALSE;
//
//			do{
//				//StreamDescription���擾
//				FAILED_BREAK(hr, lpPresentDesc->GetStreamDescriptorByIndex(i, &bIsSelected, &lpStreamDesc));
//
//				//�Ώۂ̃X�g���[�����g���Ȃ��Ƃ��͍�Ƃ��s��Ȃ�
//				if (!bIsSelected) break;
//
//				//�Ώۂ̃X�g���[���ɑ΂���Node���쐬
//				FAILED_BREAK(hr, CreateSourceNode(lpPresentDesc, lpStreamDesc, &lpSourceNode));
//				FAILED_BREAK(hr, CreateOutputNode(lpStreamDesc, &lpOutputNode));
//
//				//�e�m�[�h���g�|���W�[�ɓo�^
//				FAILED_BREAK(hr, lpTopology->AddNode(lpSourceNode));
//				FAILED_BREAK(hr, lpTopology->AddNode(lpOutputNode));
//
//				//�m�[�h��ڑ�
//				FAILED_BREAK(hr, lpSourceNode->ConnectOutput(0, lpOutputNode, 0));
//
//				//�Ώۂ̃X�g���[���̏�������
//				hr = S_OK;
//			} while (0);
//
//			if (lpOutputNode != NULL){ lpOutputNode->Release(); }
//			if (lpSourceNode != NULL){ lpSourceNode->Release(); }
//			if (lpStreamDesc != NULL){ lpStreamDesc->Release(); }
//			if (FAILED(hr)) break;
//		}
//
//		//�g�|���W�[�̏����Ɏ��s�����Ƃ��͎��s����
//		if (FAILED(hr)) break;
//
//		//�g�|���W�[���Z�b�V�����ɐݒ�
//		FAILED_BREAK(hr, m_lpMediaSession->SetTopology(0, lpTopology));
//
//		//�N���b�N���擾
//		FAILED_BREAK(hr, m_lpMediaSession->GetClock(&lpClock));
//		FAILED_BREAK(hr, lpClock->QueryInterface(IID_PPV_ARGS(&m_lpPresentationClock)));
//
//		//���f�B�A�����擾
//		FAILED_BREAK(hr, lpPresentDesc->GetUINT64(MF_PD_DURATION, (UINT64*)&m_mtMediaLength));
//
//		//�Z�b�V�����쐬����
//		m_fAudioVolume = 1.0f; m_fAudioBalance = 0.0f; m_nStatusCode |= MFSSTATUS_LOAD; hr = S_OK;
//	} while (0);
//
//	//�g�p�����C���^�[�t�F�C�X�����
//	if (lpClock != NULL){ lpClock->Release(); }
//	if (lpPresentDesc != NULL){ lpPresentDesc->Release(); }
//	if (lpTopology != NULL){ lpTopology->Release(); }
//	if (lpSource != NULL){ lpSource->Release(); }
//	if (lpSourceResolver != NULL){ lpSourceResolver->Release(); }
//	if (lpByteStream != NULL){ lpByteStream->Release(); }
//
//	return hr;
//}
//
////���f�B�A���������
//BOOL CMFSession::ReleaseMovie(void)
//{
//	HRESULT hr;
//
//	if (m_lpMediaSession != NULL){
//		if (m_lpVideoDisplay != NULL){
//			m_lpVideoDisplay->Release();
//			m_lpVideoDisplay = NULL;
//		}
//		if (m_lpAudioVolume != NULL){
//			m_lpAudioVolume->Release();
//			m_lpAudioVolume = NULL;
//		}
//
//		hr = m_lpMediaSession->Close();
//		if (FAILED(hr)) return FALSE;
//	}
//
//	if (m_lpMediaSource != NULL){ m_lpMediaSource->Shutdown(); }
//	if (m_lpMediaSession != NULL){ m_lpMediaSession->Shutdown(); }
//
//	if (m_lpPresentationClock != NULL){
//		m_lpPresentationClock->Release();
//		m_lpPresentationClock = NULL;
//	}
//	if (m_lpMediaSource != NULL){
//		m_lpMediaSource->Release();
//		m_lpMediaSource = NULL;
//	}
//	if (m_lpByteStream != NULL){
//		m_lpByteStream->Release();
//		m_lpByteStream = NULL;
//	}
//	if (m_lpMediaSession != NULL){
//		m_lpMediaSession->Release();
//		m_lpMediaSession = NULL;
//	}
//
//	m_nStatusCode = MFSSTATUS_NONE;
//	return TRUE;
//}
//
////�Đ�
//BOOL CMFSession::PlayMovie(BOOL bIsLoop)
//{
//	if (!InnerTestStatus(MFSSTATUS_LOAD)) return FALSE;
//	if (!InnerTestStatus(MFSSTATUS_PAUSE)){
//		PROPVARIANT varStart;
//		PropVariantInit(&varStart);
//		m_lpMediaSession->Start(NULL, &varStart);
//		if (bIsLoop) m_nStatusCode |= MFSSTATUS_LOOP;
//		m_nStatusCode = (m_nStatusCode & ~MFSSTATUS_PAUSE) | MFSSTATUS_PLAY;
//	}
//	return TRUE;
//}
//
////��~
//BOOL CMFSession::StopMovie(void)
//{
//	if (!InnerTestStatus(MFSSTATUS_LOAD)) return FALSE;
//	if (InnerTestStatus(MFSSTATUS_PLAY)){
//		m_lpMediaSession->Stop();
//		m_nStatusCode &= ~(MFSSTATUS_PLAY | MFSSTATUS_PAUSE);
//	}
//	return TRUE;
//}
//
////�Đ������ǂ���
//BOOL CMFSession::IsPlayMovie(void) const
//{
//	return InnerTestStatus(MFSSTATUS_PLAY);
//}
//
////�ꎞ��~
//BOOL CMFSession::PauseMovie(void)
//{
//	if (!InnerTestStatus(MFSSTATUS_LOAD)) return FALSE;
//	if (InnerTestStatus(MFSSTATUS_PAUSE)){
//		PROPVARIANT varStart;
//		PropVariantInit(&varStart);
//		m_lpMediaSession->Start(NULL, &varStart);
//		m_nStatusCode &= ~MFSSTATUS_PAUSE;
//	}
//	else if (InnerTestStatus(MFSSTATUS_PLAY)){
//		m_lpMediaSession->Pause();
//		m_nStatusCode |= MFSSTATUS_PAUSE;
//	}
//
//	return TRUE;
//}
//
////������(ms)
//BOOL CMFSession::ForwardMovie(uint32_t nTime)
//{
//	return SeekMovieRel((int64_t)MilliSecondToMediaTime(nTime));
//}
//
////�����߂�(ms)
//BOOL CMFSession::BackwardMovie(uint32_t nTime)
//{
//	return SeekMovieRel(-(int64_t)MilliSecondToMediaTime(nTime));
//}
//
////�Đ��ʒu�̈ړ�(ms)
//BOOL CMFSession::SeekMovie(uint32_t nTime)
//{
//	return SeekMovieAbs(MilliSecondToMediaTime(nTime));
//}
//
////�Đ��ʒu�̑��Έړ�(100ns)
//BOOL CMFSession::SeekMovieRel(int64_t llTime)
//{
//	MFTIME vMFTime; HRESULT hr;
//
//	if (m_lpPresentationClock == NULL) return FALSE;
//	hr = m_lpPresentationClock->GetTime(&vMFTime);
//	if (FAILED(hr)) return FALSE;
//
//	return SUCCEEDED(InnerSeekTime(vMFTime + llTime));
//}
//
////�Đ��ʒu�̐�Έړ�(100ns)
//BOOL CMFSession::SeekMovieAbs(uint64_t ullTime)
//{
//	return SUCCEEDED(InnerSeekTime(ullTime));
//}
//
////�Đ��E�B���h�E�̐ݒ�
//BOOL CMFSession::SetPlayWindow(const POINT *lpPlayPos, const SIZE *lpPlaySize)
//{
//	RECT rcRect; POINT ptPlay; SIZE sizePlay, sizeVideo;
//
//	if (!IsEnableVideo()) return FALSE;
//
//	if (m_lpVideoDisplay != NULL){
//		m_lpVideoDisplay->GetNativeVideoSize(&sizeVideo, NULL);
//
//		if (lpPlayPos != NULL){ ptPlay = *lpPlayPos; }
//		else{ ptPlay.x = ptPlay.y = 0; }
//		if (lpPlaySize != NULL){ sizePlay = *lpPlaySize; }
//		else{ sizePlay = sizeVideo; }
//
//		SetRect(&rcRect, ptPlay.x, ptPlay.y, ptPlay.x + sizePlay.cx, ptPlay.y + sizePlay.cy);
//		m_lpVideoDisplay->SetVideoPosition(NULL, &rcRect);
//	}
//	return TRUE;
//}
//
////�X�s�[�h�̕ύX
//BOOL CMFSession::SetSpeed(double dSpeed)
//{
//	IMFGetService *lpGetService = NULL; IMFRateSupport *lpRateSupport = NULL; IMFRateControl *lpRateControl = NULL; float fSpeed, fNearSpeed; HRESULT hr;
//
//	do{
//		fSpeed = (float)dSpeed;
//		FAILED_BREAK(hr, m_lpMediaSession->QueryInterface(IID_PPV_ARGS(&lpGetService)));
//		FAILED_BREAK(hr, lpGetService->GetService(MF_RATE_CONTROL_SERVICE, IID_PPV_ARGS(&lpRateSupport)));
//		FAILED_BREAK(hr, lpRateSupport->IsRateSupported(FALSE, fSpeed, &fNearSpeed));
//		FAILED_BREAK(hr, lpGetService->GetService(MF_RATE_CONTROL_SERVICE, IID_PPV_ARGS(&lpRateControl)));
//		FAILED_BREAK(hr, lpRateControl->SetRate(FALSE, fSpeed));
//	} while (0);
//
//	if (lpRateControl != NULL){ lpRateControl->Release(); }
//	if (lpRateSupport != NULL){ lpRateSupport->Release(); }
//	if (lpGetService != NULL){ lpGetService->Release(); }
//	return SUCCEEDED(hr);
//}
//
////�p���̕ω���ݒ肷��
//BOOL CMFSession::SetPan(int nAbsolutePan)
//{
//	float fBalance; HRESULT hr;
//
//	if (!IsEnableAudio()) return FALSE;
//	if (nAbsolutePan < MFS_PANLEFT || nAbsolutePan > MFS_PANRIGHT) return FALSE;
//
//	fBalance = (float)nAbsolutePan / (float)MFS_VOLUMEMAX;
//	if (fBalance == m_fAudioBalance) return TRUE;
//	m_fAudioBalance = fBalance;
//
//	hr = InnerSetVolume();
//	return SUCCEEDED(hr);
//}
//
////���ʂ̐ݒ������
//BOOL CMFSession::SetVolume(int nAbsoluteVolume)
//{
//	float fVolume; HRESULT hr;
//
//	if (!IsEnableAudio()) return FALSE;
//	if (nAbsoluteVolume < MFS_VOLUMEMIN || nAbsoluteVolume > MFS_VOLUMEMAX) return FALSE;
//
//	fVolume = (float)(nAbsoluteVolume - MFS_VOLUMEMIN) / (float)(MFS_VOLUMEMAX - MFS_VOLUMEMIN);
//	if (fVolume == m_fAudioVolume) return TRUE;
//	m_fAudioVolume = fVolume;
//
//	hr = InnerSetVolume();
//	return SUCCEEDED(hr);
//}
//
//
////���f�B�A�̍Đ����Ԃ��擾
//uint32_t CMFSession::GetMovieTime(void) const
//{
//	return MediaTimeToMilliSecond(m_mtMediaLength);
//}
//
////���f�B�A���ԂōĐ����Ԃ��擾
//uint64_t CMFSession::GetMediaTime(void) const
//{
//	return m_mtMediaLength;
//}
//
////���f�B�A�̉摜�̃T�C�Y���擾
//BOOL CMFSession::GetMovieSize(int32_t *lpnWidth, int32_t *lpnHeight) const
//{
//	SIZE sizeVideo; HRESULT hr;
//
//	if (m_lpVideoDisplay == NULL) return FALSE;
//	if (lpnWidth == NULL || lpnHeight == NULL) return FALSE;
//
//	hr = m_lpVideoDisplay->GetNativeVideoSize(&sizeVideo, NULL);
//	if (FAILED(hr)) return FALSE;
//
//	*lpnWidth = sizeVideo.cx; *lpnHeight = sizeVideo.cy;
//	return TRUE;
//}
//
////���f�B�A���f�����C����ێ����Ă��邩�ǂ���
//BOOL CMFSession::HasMediaVideo(void) const
//{
//	return InnerTestStatus(MFSSTATUS_HASVIDEOLINE);
//}
//
////���f�B�A���������C����ێ����Ă��邩�ǂ���
//BOOL CMFSession::HasMediaAudio(void) const
//{
//	return InnerTestStatus(MFSSTATUS_HASAUDIOLINE);
//}
//
////ByteStream��ContentType��ݒ肷��
//HRESULT CMFSession::SetByteStreamContentType(const TCHAR *lpFileName, const TCHAR *lpContentType)
//{
//	IMFAttributes *lpAttributes = NULL; const MEDIACONTENTTYPE *lpMediaType; const TCHAR *lp, *lpLastPath, *lpLastExt; HRESULT hr;
//
//	do{
//		CheckPointer(m_lpByteStream, E_POINTER);
//		FAILED_BREAK(hr, m_lpByteStream->QueryInterface(IID_PPV_ARGS(&lpAttributes)));
//
//		if (lpContentType != NULL){
//#ifdef _UNICODE
//			lpAttributes->SetString(MF_BYTESTREAM_CONTENT_TYPE, lpContentType);
//#else
//			WCHAR wszContentType[64];
//			MultiByteToWideChar(CP_ACP, 0, lpContentType, -1, wszContentType, sizeof(wszContentType) / sizeof(WCHAR));
//			lpAttributes->SetString(MF_BYTESTREAM_CONTENT_TYPE, wszContentType);
//#endif //_UNICODE
//			hr = S_OK; break;
//		}
//
//		for (lp = lpLastPath = lpLastExt = lpFileName; *lp != TEXT('\0'); lp++){
//			if (_istlead(*lp) && _isttrail(*(lp + 1))){ lp++; }
//			else if (*lp == TEXT('\\') || *lp == TEXT('/')){ lpLastPath = lp; }
//			else if (*lp == TEXT('.')){ lpLastExt = lp; }
//		}
//		if (lpLastPath >= lpLastExt){ hr = S_FALSE; break; } lpLastExt++;
//
//		for (lpMediaType = g_avMediaType, hr = S_FALSE; lpMediaType->ext != NULL; lpMediaType++){
//			if (_tcsicmp(lpMediaType->ext, lpLastExt) == 0){
//				hr = lpAttributes->SetString(MF_BYTESTREAM_CONTENT_TYPE, lpMediaType->type);
//				break;
//			}
//		}
//	} while (0);
//
//	if (lpAttributes != NULL){ lpAttributes->Release(); }
//	return hr;
//}
//
////�\�[�X���̃g�|���W�[�m�[�h���쐬����
//HRESULT CMFSession::CreateSourceNode(IMFPresentationDescriptor *lpPresentDesc, IMFStreamDescriptor *lpStreamDesc, IMFTopologyNode **lplpNode)
//{
//	IMFTopologyNode *lpNode = NULL; HRESULT hr;
//
//	do{
//		//SourceStream�p�̃m�[�h���쐬
//		FAILED_BREAK(hr, MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &lpNode));
//
//		//�K�v�ȑ�����ݒ肷��
//		FAILED_BREAK(hr, lpNode->SetUnknown(MF_TOPONODE_SOURCE, m_lpMediaSource));
//		FAILED_BREAK(hr, lpNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, lpPresentDesc));
//		FAILED_BREAK(hr, lpNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, lpStreamDesc));
//
//		//�쐬�����������̂Ŗ߂�
//		*lplpNode = lpNode; lpNode = NULL; hr = S_OK;
//	} while (0);
//
//	if (lpNode != NULL){ lpNode->Release(); }
//	return hr;
//}
//
////�o�͕��̃g�|���W�[�m�[�h���쐬����
//HRESULT CMFSession::CreateOutputNode(IMFStreamDescriptor *lpStreamDesc, IMFTopologyNode **lplpNode)
//{
//	IMFTopologyNode *lpNode = NULL; IMFMediaTypeHandler *lpHandler = NULL; IMFActivate *lpActivate = NULL; GUID guidMajorType; DWORD dwStreamID; HRESULT hr;
//
//	do{
//		memset(&guidMajorType, 0x00, sizeof(guidMajorType));
//
//		//�X�g���[����ID���擾
//		lpStreamDesc->GetStreamIdentifier(&dwStreamID);
//
//		//�X�g���[���̃��f�C�A��ʂ��擾
//		FAILED_BREAK(hr, lpStreamDesc->GetMediaTypeHandler(&lpHandler));
//		FAILED_BREAK(hr, lpHandler->GetMajorType(&guidMajorType));
//
//		//�o�͕��̃g�|���W�[�m�[�h���쐬
//		FAILED_BREAK(hr, MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &lpNode));
//
//		//�����_�����쐬
//		if (IsEqualGUID(guidMajorType, MFMediaType_Audio)){
//			//�I�[�f�B�I�����_�����쐬
//			FAILED_BREAK(hr, MFCreateAudioRendererActivate(&lpActivate));
//			m_nStatusCode |= MFSSTATUS_HASAUDIOLINE;
//		}
//		else if (IsEqualGUID(guidMajorType, MFMediaType_Video)){
//			//�r�f�I�����_�����쐬
//			FAILED_BREAK(hr, MFCreateVideoRendererActivate(m_hWnd, &lpActivate));
//			m_nStatusCode |= MFSSTATUS_HASVIDEOLINE;
//		}
//
//		//�o�͂ł����ʂłȂ��Ƃ��͎��s����
//		else{ hr = E_FAIL; break; }
//
//		//�m�[�h�ɏo�͂�ݒ肷��
//		FAILED_BREAK(hr, lpNode->SetObject(lpActivate));
//
//		//�쐬�����������̂Ŗ߂�
//		*lplpNode = lpNode; lpNode = NULL; hr = S_OK;
//	} while (0);
//
//	if (lpActivate != NULL){ lpActivate->Release(); }
//	if (lpHandler != NULL){ lpHandler->Release(); }
//	if (lpNode != NULL){ lpNode->Release(); }
//
//	return S_OK;
//}
//
//
////�Đ��ʒu��ݒ肷��
//HRESULT CMFSession::InnerSeekTime(MFTIME nTime)
//{
//	if (!InnerTestStatus(MFSSTATUS_LOAD)) return E_ABORT;
//
//	if (nTime < 0){ nTime = 0; }
//	else if ((uint64_t)nTime >= m_mtMediaLength){ nTime = m_mtMediaLength; }
//
//	PROPVARIANT varStart;
//	varStart.vt = VT_I8;
//	varStart.hVal.QuadPart = nTime;
//
//	m_lpMediaSession->Start(NULL, &varStart);
//	if (!InnerTestStatus(MFSSTATUS_PLAY)){ m_lpMediaSession->Stop(); }
//	else if (InnerTestStatus(MFSSTATUS_PAUSE)){ m_lpMediaSession->Pause(); }
//
//	return S_OK;
//}
//
////�{�����[����ݒ肷��
//HRESULT CMFSession::InnerSetVolume(void)
//{
//	float *lpfVolumes; uint32_t i, nChannels;
//
//	CheckPointer(m_lpAudioVolume, E_FAIL);
//
//	m_lpAudioVolume->GetChannelCount(&nChannels);
//	lpfVolumes = (float *)_malloca(sizeof(float) * nChannels);
//
//	for (i = 0; i < nChannels; i++){
//		if (i & 0x01){ lpfVolumes[i] = (m_fAudioBalance >= 0.0f ? 1.0f : (1.0f + m_fAudioBalance)) * m_fAudioVolume; }
//		else{ lpfVolumes[i] = (m_fAudioBalance <= 0.0f ? 1.0f : (1.0f - m_fAudioBalance)) * m_fAudioVolume; }
//	}
//
//	if (nChannels & 0x01){ lpfVolumes[nChannels - 1] = m_fAudioVolume; }
//	m_lpAudioVolume->SetAllVolumes(nChannels, lpfVolumes);
//	_freea(lpfVolumes);
//
//	return S_OK;
//}
//
//
////�r�f�I�L���e�X�g
//bool CMFSession::IsEnableVideo(void) const
//{
//	return InnerTestStatus(MFSSTATUS_LOAD | MFSSTATUS_HASVIDEOLINE);
//}
//
////�T�E���h�L���e�X�g
//bool CMFSession::IsEnableAudio(void) const
//{
//	return InnerTestStatus(MFSSTATUS_LOAD | MFSSTATUS_HASAUDIOLINE);
//}
//
////�����I�ȃX�e�[�^�X�e�X�g
//bool CMFSession::InnerTestStatus(uint32_t nStatus) const
//{
//	return (m_nStatusCode & nStatus) == nStatus;
//}
//
////�~���b�����f�B�A�^�C���ɕϊ�����
//uint64_t CMFSession::MilliSecondToMediaTime(uint32_t t)
//{
//	return (uint64_t)t * (1000 * 10);
//}
//
////���f�B�A�^�C�����~���b�ɕϊ�����
//uint32_t CMFSession::MediaTimeToMilliSecond(uint64_t t)
//{
//	return (uint32_t)(t / (1000 * 10));
//}
//
//
////�����p�����[�^���擾����
//STDMETHODIMP CMFSession::GetParameters(DWORD *pdwFlags, DWORD *pdwQueue)
//{
//	return E_NOTIMPL;
//}
//
////�񓯊��������s��ꂽ�Ƃ��̃R�[���o�b�N
//STDMETHODIMP CMFSession::Invoke(IMFAsyncResult *pAsyncResult)
//{
//	IMFMediaEvent *lpMediaEvent; MediaEventType type; HRESULT hr;
//
//	do{
//		lpMediaEvent = NULL;
//
//		//�C�x���g�L���[����C�x���g���擾
//		FAILED_BREAK(hr, m_lpMediaSession->EndGetEvent(pAsyncResult, &lpMediaEvent));
//		FAILED_BREAK(hr, lpMediaEvent->GetType(&type));
//
//		//�Z�b�V�����I���łȂ��Ƃ��͍ēx�C�x���g�擾��L���ɂ���
//		if (type != MESessionClosed){ m_lpMediaSession->BeginGetEvent(static_cast<IMFAsyncCallback *>(this), NULL); }
//
//		//�Z�b�V�������L���ł���Ƃ��̓C�x���g���E�B���h�E�v���V�[�W�����甭�s���Ă��炤
//		if (m_lpMediaSession != NULL){
//			::PostMessage(m_hWnd, WM_MFSNOTIFY, (WPARAM)this, (LPARAM)lpMediaEvent);
//			lpMediaEvent = NULL;
//		}
//
//		hr = S_OK;
//	} while (0);
//
//	if (lpMediaEvent != NULL){ lpMediaEvent->Release(); }
//	return hr;
//}
//
//
////�ĕ`����s��
//BOOL CMFSession::Repaint(void)
//{
//	if (m_lpVideoDisplay == NULL) return FALSE;
//	m_lpVideoDisplay->RepaintVideo();
//	return TRUE;
//}
//
////�C�x���g�������s
//BOOL CMFSession::HandleEvent(LPARAM lParam)
//{
//	IUnknown *lpUnknown; IMFMediaEvent *lpMediaEvent = NULL; MF_TOPOSTATUS topostatus; MediaEventType type; HRESULT hrStatus, hr;
//
//	if (lParam == NULL) return FALSE;
//
//	do{
//		lpUnknown = (IUnknown *)lParam;
//		FAILED_BREAK(hr, lpUnknown->QueryInterface(IID_PPV_ARGS(&lpMediaEvent)));
//		FAILED_BREAK(hr, lpMediaEvent->GetType(&type));
//		FAILED_BREAK(hr, lpMediaEvent->GetStatus(&hrStatus));
//		if (FAILED(hrStatus)){ hr = hrStatus; break; }
//
//		switch (type){
//		case MESessionTopologyStatus:
//			FAILED_BREAK(hr, lpMediaEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, (UINT32 *)&topostatus));
//			switch (topostatus){
//			case MF_TOPOSTATUS_READY:
//			{
//				IMFGetService *lpGetService = NULL;
//
//				do{
//					FAILED_BREAK(hr, m_lpMediaSession->QueryInterface(IID_PPV_ARGS(&lpGetService)));
//					if (InnerTestStatus(MFSSTATUS_HASVIDEOLINE)){ FAILED_BREAK(hr, lpGetService->GetService(MR_VIDEO_RENDER_SERVICE, IID_PPV_ARGS(&m_lpVideoDisplay))); }
//					if (InnerTestStatus(MFSSTATUS_HASAUDIOLINE)){ FAILED_BREAK(hr, lpGetService->GetService(MR_STREAM_VOLUME_SERVICE, IID_PPV_ARGS(&m_lpAudioVolume))); }
//				} while (0);
//				if (lpGetService != NULL){ lpGetService->Release(); }
//			} break;
//			} break;
//
//		case MEEndOfPresentation:
//			if (InnerTestStatus(MFSSTATUS_LOOP)){
//				PROPVARIANT varStart;
//				varStart.vt = VT_I8;
//				varStart.hVal.QuadPart = 0;
//				m_lpMediaSession->Start(NULL, &varStart);
//			}
//			else{ m_nStatusCode &= ~(MFSSTATUS_PLAY | MFSSTATUS_PAUSE); }
//			hr = S_OK;
//			break;
//
//		default: hr = S_OK;
//		}
//	} while (0);
//
//	if (lpMediaEvent != NULL){ lpMediaEvent->Release(); }
//	if (lpUnknown != NULL){ lpUnknown->Release(); }
//	return SUCCEEDED(hr);
//}
//
//
//
//
////LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
////
////int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
////{
////	HWND hMainWnd; HRESULT hr;
////
////	CMFSession *g_lpSession = NULL;
////	MFStartup(MF_VERSION);
////	g_lpSession = new CMFSession(hMainWnd);
////	hr = g_lpSession->LoadMovie(TEXT("EngineResource/vi.wmv"));
////	hr = g_lpSession->PlayMovie(FALSE);
////	//g_lpSession->SetPlayWindow();
////	
////	g_lpSession->ReleaseMovie();
////	delete g_lpSession;
////	g_lpSession = NULL;
////	MFShutdown();
////}
//
////LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
////{
////	switch (uMsg){
////	case WM_MFSNOTIFY:
////		if (wParam != NULL){ ((CMFSession *)wParam)->HandleEvent(lParam); }
////		return 0;
////
////	case WM_PAINT:
////	{
////		PAINTSTRUCT ps; HDC hdc;
////		hdc = BeginPaint(hWnd, &ps);
////		�E�E�E
////			EndPaint(hWnd, &ps);
////		if (g_lpSession != NULL){ g_lpSession->Repaint(); }
////	} break;
////
////	�E�E�E
////	}
////	return DefWindowProc(hWnd, uMsg, wParam, lParam);
////}