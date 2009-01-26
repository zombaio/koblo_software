
/*! \class CXloader
 * \brief Implements IUploader and IDownloader
 *
 * Supplies more complex and buffered method for posting/putting web-data
 * (lasse)
*/
class CXloader : public virtual IDownloader, public virtual IUploader
{
	friend class CURLMulti_Wrapper;
public:
	CXloader(tbool bIsUploader);
	virtual ~CXloader();
	
	//! IDestructable override
	virtual void Destroy();

	//! IDownloader implementation
	virtual tbool Init(const tchar* pszHost, const tchar* pszPage, tint32 iPort = 80, const tchar* pszUser = NULL, const tchar* pszPassword = NULL, tint32 iTimeOutSecs = 10);
	//! IUploader implementation
	virtual tbool Init(const tchar* pszHost, const tchar* pszPage, IFile* pfileToUpload, tchar* pszParamName = "Upload", tint32 iPort = 80, const tchar* pszUser = NULL, const tchar* pszPassword = NULL, tint32 iTimeOutSecs = 10);
	//! IUploader implementation
	virtual tbool SetNameOfUploadFile(tchar* pszOverrideName);
	//! IDownloader implementation
	virtual tbool SetReplyMIMEType(E_MIME_Type eMIME);
	//! IDownloader implementation
	virtual tbool SetSpecificVerb(EVerbType eVerb);
	//! IDownloader implementation
	virtual tbool DisableAutoRedirects();
	//! IDownloader implementation
	virtual tbool EnableAutoRedirects();
	//! IDownloader implementation
	virtual tbool AddParam(const tchar* pszParamName, const tchar* pcParamData, tint32 iParamDataLen);
	//! IDownloader implementation
	virtual tbool Start(IFile* pfileForDownload = NULL);
	//! IDownloader implementation
	virtual tbool GetReplyPortion(tchar* pszBuffer, tint32 iBufferSize, tint32* piPortionSize);
	//! IDownloader implementation
	virtual tbool Abort();
	//! IUploader implementation
	virtual tbool GetProgress(tint64* piUploadProgress, tint64* piUploadSize, tint64* piDownloadProgress, tint64* piDownloadSize);
	//! IDownloader implementation
	virtual tbool GetProgress(tint64* piDownloadProgress, tint64* piDownloadSize);

	//! IDownloader implementation
	virtual tbool IsDone();
	//! IDownloader implementation
	virtual tbool IsFailed();

	//! IDownloader implementation
	virtual tbool GetError(tchar* pszErrBuff, tint32 iErrBuffSize);

	// Callbacks
	size_t ReadFunction_ForUpload(void *ptr, size_t size, size_t nmemb);
	int SeekFunction_ForUpload(curl_off_t offset, int origin);
	size_t WriteFunction_ForReply(void *ptr, size_t size, size_t nmemb);

protected:
	tbool mbIsUploader;

	std::string msHost;
	std::string msPage;
	IFile* mpfileToUpload;
	std::string msUploadFileParamName;
	std::string msUploadFileNameAndExtOnly;
	tint32 miPort;
	std::string msUser;
	std::string msPassword;
	tuint32 muiTimeOutSecs;

	E_MIME_Type meMIMEType;
	const tchar* GetMIMEString();
	
	EVerbType meSpecificVerb;
	EVerbType GetActuallyUsedVerb();
	const tchar* GetVerbString(EVerbType eVerb);

	tbool mbAllowRedirects;
	tbool SetAllowRedirects(tbool bAllow);
	
	std::list<std::string> mlist_sParamNames;
	std::list<tint32> mlist_iParamDataLen;
	std::list<tchar*> mlist_pszParamDataUrlEncoded;
	tint32 miParamsAssembledLen;
	tchar* mpszParamsAssembled;
	tbool AssembleParams(EVerbType eVerb);
	void WipeParams();
	CMutex mMutex_ForParams;
	void CloseFile_IgnoreError();

	// File for writing reply directly into
	IFile* mpfileForReply;

	std::list<CXloader_ReplyChainLink*> mlist_pReplyChain;
	CMutex mMutex_ForReplyBuffer;
	void ZapReplyBuffer();

	tuint64 muiUploadProgress;
	tuint64 muiReplyProgress;
	tuint64 muiReplySize;

	//volatile tuint32 muiAliveMs;
	//void RefreshAlive();
	//tbool IsAlive();
	
	std::string msLastError;
	void SetError(const tchar* pszError);
	CMutex mMutex_ForErrors;

	
	//void Constructor_OSSpecific();
	//void Destructor_OSSpecific();

	CMutex mMutex_Connection;
	tbool OpenConnection();
	//tbool OpenConnection_OSSpecific();
	void CloseConnection();
	//void CloseConnection_OSSpecific();
	
	//tbool DoPortion_OSSpecific(tuint64* puiUploadProgress, tchar* pszReplyBuffer, tint32 iReplyBufferSize, tint32* piReplyPortionSize, tuint64* puiReplyTotalSize);
	
	void SetIsUninitialized();
	void SetIsInitialized();
	void SetIsTransfering();
	void SetMultiSaysDone(CURLcode status);
	void SetIsDone();
	void SetIsFailed();

	tbool IsInitialized();
	tbool IsTransfering();
	
	
private:
	volatile tbool mbIsInitialized;
	volatile tbool mbIsTransfering;
	volatile tbool mbIsFailed;
	volatile tbool mbIsMultiDone;
	volatile tbool mbIsDone;

	tchar mpszErrorBuffer[CURL_ERROR_SIZE];

	CURL* mpCURLEasyHandle;
	curl_httppost* mpFormPost_First;
	curl_httppost* mpFormPost_Last;
	curl_slist* mpSList_ExtraHeaders;
	
	tbool AssembleParams_ForUrlEncoded(EVerbType eVerb);
	tbool AssembleParams_ForMultiPartForm(EVerbType eVerb);

	tbool SetOpt(CURLoption iOption, const tchar* pszOption, const void* pData, const tchar* pszExtraInfo = "");
	tbool SetOpt(CURLoption iOption, const tchar* pszOption, tint32 iData, const tchar* pszExtraInfo = "");
	tbool SetOpt(CURLoption iOption, const tchar* pszOption, tuint32 uiData, const tchar* pszExtraInfo = "");
	tbool SetOpt(CURLoption iOption, const tchar* pszOption, const tchar* pszData, const tchar* pszExtraInfo = "");
	tbool SetOpt(CURLoption iOption, const tchar* pszOption, const std::string& rsData, const tchar* pszExtraInfo = "");
	tbool SetOpt(CURLoption iOption, const tchar* pszOption, tbool bData, const tchar* pszExtraInfo = "");
	
	
protected:
	
#ifdef _WIN32
//	#include "CXloaderWin.h"
#endif // _WIN32
#ifdef _Mac
//	#include "CXloaderOSX.h"
#endif // _Mac

};