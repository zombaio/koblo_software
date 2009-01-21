
#include "ineInternalOS.h"

CXloader::CXloader(tbool bIsUploader)
{
	mbIsUploader = bIsUploader;
	mpfileToUpload = NULL;
	mpszParamsAssembled = NULL;
	miParamsAssembledLen = 0;
	mbIsInitialized = mbIsTransfering = mbIsFailed = mbIsDone = false;
	meMIMEType = MIME_TYPE_NONE;
	meSpecificVerb = VERB_DEFAULT;

	// Prepare CURL multi instance
	{
		GetLockForMultiInstance();
		//
		// Maybe create CURL multi instance
		if (gpCURLMulti == NULL) gpCURLMulti = curl_multi_init();
		// Hook CURL multi instance
		giCURLMultiHooks++;
		//
		ReleaseLockForMultiInstance();
	}
	
	Constructor_OSSpecific();
} // constructor


CXloader::~CXloader()
{
	Abort();
	WipeParams();
	CloseConnection();
	
	// Unhook and maybe release CURL multi instance
	{
		GetLockForMultiInstance();
		//
		// Unhook CURL multi instance
		giCURLMultiHooks--;
		// Maybe release CURL multi instance
		if (giCURLMultiHooks == 0) {
			// No more use for instance - release it
			curl_multi_cleanup(gpCURLMulti);
			gpCURLMulti = NULL;
		}
		//
		ReleaseLockForMultiInstance();
	}

	Destructor_OSSpecific();
} // destructor


void CXloader::GetLockForMultiInstance()
{
	// Attempt lock
	while (++giCURLMulti_Level != 1) {
		// We're not alone - release again
		giCURLMulti_Level--;
		// Sleep 1 - 3 ms (random)
		tint32 iSleepMS = (tint32)(rand() * (3.0f / RAND_MAX));
		iSleepMS++;
		ITime::SleepMS(iSleepMS);
	}
} // GetLockForMultiInstance


void CXloader::ReleaseLockForMultiInstance()
{
	giCURLMulti_Level--;
} // ReleaseLockForMultiInstance

	
IDownloader* IDownloader::Create()
{
	return dynamic_cast<IDownloader*>(new CXloader(false));
} // Create


IUploader* IUploader::Create()
{
	return dynamic_cast<IUploader*>(new CXloader(true));
} // Create


void CXloader::Destroy()
{
	delete dynamic_cast<CXloader*>(this);
} // Destroy


tbool CXloader::Init(const tchar* pszHost, const tchar* pszPage, tint32 iPort /*= 80*/, const tchar* pszUser /*= NULL*/, const tchar* pszPassword /*= NULL*/, tint32 iTimeOutSecs /*= 10*/)
{
	// Call IUploader code, just without file pointer
	return Init(pszHost, pszPage, NULL, iPort, pszUser, pszPassword, iTimeOutSecs);
} // Init (for IDownloader)


tbool CXloader::Init(const tchar* pszHost, const tchar* pszPage, IFile* pfileToUpload, tint32 iPort /*= 80*/, const tchar* pszUser /*= NULL*/, const tchar* pszPassword /*= NULL*/, tint32 iTimeOutSecs /*= 10*/)
{
	Abort();
	WipeParams();
	mbIsInitialized = false;
	
	// Verify sanity of host
	{
		if ((pszHost == NULL) || (*pszHost == '\0')) {
			SetError("No param name");
			return false;
		}
		for (tchar* pc = (tchar*)pszHost; *pc; pc++) {
			tchar c = *pc;
			if (
				((c < 'A') || (c > 'Z'))
				&&
				((c < 'a') || (c > 'z'))
				&&
				((c < '0') || (c > '9'))
				&&
				(c != '.')
				) {
				SetError("Not valid url-host");
				return false;
			}
		}
	}
	
	// Verify sanity of page
	{
		if (pszPage != NULL) {
			for (tchar* pc = (tchar*)pszPage; *pc; pc++) {
				tchar c = *pc;
				if (
					((c < 'A') || (c > 'Z'))
					&&
					((c < 'a') || (c > 'z'))
					&&
					((c < '0') || (c > '9'))
					&&
					(c != '.')
					&&
					(c != '/')
					) {
					SetError("Not valid url-document");
					return false;
				}
			}
		}
	}
	
	// Verify sanity of port
	if (iPort <= 0) {
		SetError("Not a valid port");
		return false;
	}
	
	// Verify sanity of time-out
	if (iTimeOutSecs < 0) {
		SetError("Negative time-out");
		return false;
	}
	
	msHost = pszHost;
	msPage = (pszPage == NULL) ? "" : pszPage;
	miPort = iPort;
	msUser = (pszUser == NULL) ? "" : pszUser;
	msPassword = (pszPassword == NULL) ? "" : pszPassword;
	muiTimeOutSecs = (tuint32)iTimeOutSecs;
	
	mbIsInitialized = true;
	meMIMEType = MIME_TYPE_NONE;
	meSpecificVerb = VERB_DEFAULT;
	
	return true;
} // Init (for IUploader)


tbool CXloader::SetReplyMIMEType(E_MIME_Type eMIME)
{
	if (mbIsFailed) {
		//SetError("Previous error");
		return false;
	}
	
	if (!mbIsInitialized) {
		SetError("Not initialized");
		return false;
	}
	
	if (mbIsTransfering) {
		SetError("We can't set media type as we've already begun transfer");
		return false;
	}
	
	// Success
	meMIMEType = eMIME;
	return true;
} // SetReplyMIMEType


const tchar* CXloader::GetMIMEString()
{
	switch (meMIMEType) {
		case MIME_TYPE_TEXT:	return "text/*";
		case MIME_TYPE_HTML:	return "text/html";
		case MIME_TYPE_XML:		return "text/xml";
		case MIME_TYPE_BINARY:	return "application/octet-stream";
		case MIME_TYPE_OGG:		return "audio/ogg";
		case MIME_TYPE_MP3:		return "audio/mpeg";
	}
	return NULL;
} // GetMIMEString


tbool CXloader::SetSpecificVerb(EVerbType eVerb)
{
	if (mbIsFailed) {
		//SetError("Previous error");
		return false;
	}
	
	if (!mbIsInitialized) {
		SetError("Not initialized");
		return false;
	}
	
	if (mbIsTransfering) {
		SetError("We can't change http-verb as we've already begun transfer");
		return false;
	}
	
	tbool bIsUploader = (mpfileToUpload != NULL);
	
	switch (eVerb) {
		case VERB_DEFAULT:
			meSpecificVerb = eVerb;
			return true;
			
		case VERB_GET:
		{
			// Only IDownloader may use GET
			if (!bIsUploader)
				return true;
		}
			break;
			
		case VERB_POST:
			meSpecificVerb = eVerb;
			return true;
			
		case VERB_PUT:
		{
			// Only IUploader may use PUT
			if (bIsUploader)
				return true;
		}
			break;
	}
	
	// Fall-through to error
	return false;
} // SetSpecificVerb


EVerbType CXloader::GetVerb(EVerbType eVerbDefault)
{
	switch (meSpecificVerb) {
		case VERB_GET:
		case VERB_POST:
		case VERB_PUT:
			return meSpecificVerb;
	}
	
	return eVerbDefault;
} // GetVerb


const tchar* CXloader::GetVerbString(EVerbType eVerbDefault)
{
	switch (GetVerb(eVerbDefault)) {
		case VERB_GET:	return "GET";
		case VERB_POST:	return "POST";
		case VERB_PUT:	return "PUT";
	}
	
	// What?!
	return "";
} // GetVerbString


tbool CXloader::AddParam(const tchar* pszParamName, const tchar* pcParamData, tint32 iParamDataLen)
{
	CAutoLock Lock(mMutex_ForParams);
	
	if (mbIsFailed) {
		//SetError("Previous error");
		return false;
	}
	
	if (!mbIsInitialized) {
		SetError("Not initialized");
		return false;
	}
	
	if (mbIsTransfering) {
		SetError("We can't add more parameters as we've already begun transfer");
		return false;
	}
	
	// Verify sanity of param name
	{
		if ((pszParamName == NULL) || (*pszParamName == '\0')) {
			SetError("No param name");
			return false;
		}
		for (tchar* pc = (tchar*)pszParamName; *pc; pc++) {
			tchar c = *pc;
			if (
				((c < 'A') || (c > 'Z'))
				&&
				((c < 'a') || (c > 'z'))
				&&
				((c < '0') || (c > '9'))
				&&
				((c != '_') && (c != '[') && (c != ']'))
				) {
				SetError("Not valid paramname");
				return false;
			}
		}
	}
	
	// Convert param data to URL-encoded safe string
	tchar* pszUrlEncoded = NULL;
	tint32 iUrlEncodedLen = 0;
	{
		if (iParamDataLen < 0) {
			iParamDataLen = (pcParamData == NULL) ? 0 : strlen(pcParamData);
		}
		iUrlEncodedLen = (iParamDataLen == 0) ? 0 : IINetUtil::CalcURLEncodedLen(pcParamData, iParamDataLen);
		if (iUrlEncodedLen > 0) {
			pszUrlEncoded = new tchar[iUrlEncodedLen + 1];
			if (pszUrlEncoded == NULL) {
				SetError("Out of memory");
				return false;
			}
			tint32 iActuallyEncoded = IINetUtil::URLEncode(pcParamData, iParamDataLen, pszUrlEncoded);
			if (iActuallyEncoded != iUrlEncodedLen) {
				SetError("Insane output length");
				delete[] pszUrlEncoded;
				return false;
			}
		}
	}
	
	// Add param name and encoded value to lists
	try {
		mlist_sParamNames.push_back(pszParamName);
		mlist_iParamDataLen.push_back(iUrlEncodedLen);
		mlist_pszParamDataUrlEncoded.push_back(pszUrlEncoded);
	}
	catch (...) {
		// Unspecified error - probably out of memory
		delete[] pszUrlEncoded;
		return false;
	}
	
	// Success
	return true;
} // AddParam


tbool CXloader::AssembleParams()
{
	CAutoLock Lock(mMutex_ForParams);
	
	if (!mbIsInitialized) {
		SetError("Not initialized");
		return false;
	}
	
	if (mpszParamsAssembled) {
		SetError("Already assembled");
		return false;
	}
	
	// Verify sanity of lists
	tint32 iNames = mlist_sParamNames.size();
	tint32 iLens = mlist_iParamDataLen.size();
	tint32 iDatas = mlist_pszParamDataUrlEncoded.size();
	if ((iNames != iLens) || (iLens != iDatas)) {
		SetError("List lengths aren't all the same");
		return false;
	}
	
	// Calculate space needed
	{
		miParamsAssembledLen = 0;
		
		// First accumulate lengths
		std::list<std::string>::iterator itName = mlist_sParamNames.begin();
		std::list<tint32>::iterator itDataLen = mlist_iParamDataLen.begin();
		for ( ; itName != mlist_sParamNames.end(); itName++, itDataLen++) {
			std::string& rsName = *itName;
			tint32 iLen = *itDataLen;
			
			// First a parameter delimiter (for GET verb only)
			if (meSpecificVerb == VERB_GET) {
				// For verb GET we add the parameters as part of URI.
				// Therefore all will be prepend with a delimiter char:
				// - First parameter will be delimited from page part by a '?'
				// - Rest of the parameters will be delimited by a '&'
				miParamsAssembledLen++;
			}
			
			// Then param name
			miParamsAssembledLen += rsName.length();
			
			// Then maybe param data
			if (iLen > 0) {
				// An equation-sign and then data
				miParamsAssembledLen += 1 + iLen;
			}
			
			// Lastly a new-line sequence (always DOS style due to w3c spec)
			if (meSpecificVerb == VERB_GET) {
				// There's no new-line for verb GET, as the parameters are part of the URI
				// Do nothing  here...
			}
			else {
				// Using verb POST: Two new-line chars, CR + LF
				miParamsAssembledLen += 2;
			}
		}
	}
	
	// Attempt to allocate space for params + traling zero
	mpszParamsAssembled = new tchar[miParamsAssembledLen + 1];
	if (mpszParamsAssembled == NULL) {
		SetError("Out of memory");
		return false;
	}
	
	// And action! Assemble the parameter string
	{
		tchar* pszDst = mpszParamsAssembled;
		tchar cNameDelimiter = '?';
		std::list<std::string>::iterator itName = mlist_sParamNames.begin();
		std::list<tint32>::iterator itDataLen = mlist_iParamDataLen.begin();
		std::list<tchar*>::iterator itParamData = mlist_pszParamDataUrlEncoded.begin();
		while (itName != mlist_sParamNames.end()) {
			// Get elements
			std::string& rsName = *itName;
			tint32 iNameLen = rsName.length();
			tchar* pcData = *itParamData;
			tint32 iDataLen = *itDataLen;
			
			// For verb GET we must prepend parameter names with a delimiter
			if (meSpecificVerb == VERB_GET) {
				*pszDst++ = cNameDelimiter;
			}
			
			// Copy param name
			memcpy(pszDst, rsName.c_str(), iNameLen);
			pszDst += iNameLen;
			
			// Maybe copy param data
			if (iDataLen > 0) {
				*pszDst++ = '=';
				memcpy(pszDst, pcData, iDataLen);
				pszDst += iDataLen;
			}
			
			// Newline sequence for verb POST only
			if (meSpecificVerb != VERB_GET) {
				memcpy(pszDst, "\r\n", 2);
				pszDst += 2;
			}
			
			// Advance to next
			itName++;
			itDataLen++;
			itParamData++;
			//cNameDelimiter = '&';
		}
		// Append zero-termination
		*pszDst = '\0';
	}
	
	return true;
} // AssembleParams


void CXloader::WipeParams()
{
	CAutoLock Lock(mMutex_ForParams);
	
	mlist_sParamNames.clear();
	
	std::list<tchar*>::iterator it = mlist_pszParamDataUrlEncoded.begin();
	while (it != mlist_pszParamDataUrlEncoded.end()) {
		tchar* psz = *it;
		if (psz) delete[] psz;
		mlist_pszParamDataUrlEncoded.erase(it);
		it = mlist_pszParamDataUrlEncoded.begin();
	}
	
	mlist_iParamDataLen.clear();
	
	if (mpszParamsAssembled) {
		delete[] mpszParamsAssembled;
		mpszParamsAssembled = NULL;
	}
} // WipeParams


tbool CXloader::OpenConnection()
{
	return OpenConnection_OSSpecific();
} // OpenConnection


void CXloader::CloseConnection()
{
	CloseConnection_OSSpecific();
	CloseFile_IgnoreError();
} // CloseConnection


tbool CXloader::DownloadPortion(tchar* pszBuffer, tint32 iBufferSize, tint32* piPortionSize, tuint64* puiTotalSize)
{
	return UploadPortion(NULL, pszBuffer, iBufferSize, piPortionSize, puiTotalSize);
} // DownloadPortion


tbool CXloader::UploadPortion(tuint64* puiUploadProgress, tchar* pszReplyBuffer, tint32 iReplyBufferSize, tint32* piReplyPortionSize, tuint64* puiReplyTotalSize)
{
	*piReplyPortionSize = 0;
	
	if (IsFailed()) {
		//SetError("Previous error");
		return false;
	}
	
	if (!IsInitialized()) {
		SetError("Not initialized");
		return false;
	}
	
	tbool bFirstTime = (!IsTransfering());
	SetIsTransfering();
	if (bFirstTime) {
		CAutoLock Lock(mMutex_Connection);
		
		CloseConnection();
		if (!AssembleParams()) return false;
		if (!OpenConnection()) {
			CloseConnection();
			return false;
		}
		// We're alive
		RefreshAlive();
	}
	
	return DoPortion_OSSpecific(puiUploadProgress, pszReplyBuffer, iReplyBufferSize, piReplyPortionSize, puiReplyTotalSize);
} // UploadPortion


tbool CXloader::Abort()
{
	if (IsTransfering()) {
		CAutoLock Lock(mMutex_Connection);
		
		CloseConnection();
		SetIsDone();
	}
	
	WipeParams();
	
	CloseFile_IgnoreError();	
	
	return true;
} // Abort


void CXloader::CloseFile_IgnoreError()
{
	if (mpfileToUpload) {
		//mpfileToUpload->Destroy(); - will be Destroy'ed from calling app
		mpfileToUpload = NULL;
	}
} // CloseFile_IgnoreError


void CXloader::SetIsUninitialized()
{
	mbIsInitialized = mbIsTransfering = mbIsDone = mbIsFailed = false;
} // SetIsUninitialized


void CXloader::SetIsInitialized()
{
	mbIsInitialized = true;
} // SetIsInitialized


void CXloader::SetIsTransfering()
{
	mbIsFailed = mbIsDone = false;
	mbIsTransfering = true;
} // SetIsTransfering


void CXloader::SetIsDone()
{
	mbIsDone = true;
	mbIsTransfering = false;
} // SetIsDone


void CXloader::SetIsFailed()
{
	mbIsFailed = true;
	mbIsTransfering = false;
} // SetIsFailed


tbool CXloader::IsTransfering()
{
	return mbIsTransfering;
} // IsTransfering


tbool CXloader::IsInitialized()
{
	return mbIsInitialized;
} // IsInitialized


tbool CXloader::IsDone()
{
	return mbIsDone;
} // IsDone


tbool CXloader::IsFailed()
{
	return mbIsFailed;
} // IsFailed


void CXloader::RefreshAlive()
{
	muiAliveMs = ITime::GetTimeMS();
} // RefreshAlive


tbool CXloader::IsAlive()
{
	// (lasse) Are we really needing this? Anyway not used now
	
	if ((!mbIsInitialized) || (!mbIsTransfering)) {
		SetError("Not initialized or not downloading");
		return false;
	}
	
	if (mbIsFailed) {
		//SetError("Previously failed");
		return false;
	}
	
	tuint32 uiElapsedMs = ITime::GetTimeMS() - muiAliveMs;
	
	if (muiTimeOutSecs == 0) {
		// Special case for zero time-out: Actually use 500 ms
		return (uiElapsedMs < 500);
	}
	
	tuint32 uiSecsElapsed = uiElapsedMs / 1000;
	return (uiSecsElapsed <= muiTimeOutSecs);
} // IsAlive


void CXloader::SetError(const tchar* pszError)
{
	CAutoLock Lock(mMutex_ForErrors);
	
	msLastError = pszError;
	mbIsFailed = true;
}


tbool CXloader::GetLatestError(tchar* pszErrBuff, tint32 iErrBuffSize)
{
	CAutoLock Lock(mMutex_ForErrors);
	
	if ((pszErrBuff == NULL) || (iErrBuffSize <= 0)) return false;
	
	if (!mbIsFailed) {
		// No error
		*pszErrBuff = '\0';
		return true;
	}
	
	tchar* pszMsg = (tchar*)(msLastError.c_str());
	tint32 iLenMsg = msLastError.length();
	if (iLenMsg == 0) {
		// No error description available
		pszMsg = "Unspecified error";
		iLenMsg = strlen(pszMsg);
	}
	tbool bRoomEnough = iLenMsg < iErrBuffSize;
	if (!bRoomEnough) {
		// Cut off
		iLenMsg = iErrBuffSize - 1;
	}
	memcpy(pszErrBuff, pszMsg, iLenMsg);
	pszErrBuff[iLenMsg] = '\0';
	
	return bRoomEnough;
}