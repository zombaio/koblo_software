// CDownloader MS-Windows only code

#include "ineInternalOS.h"


void CDownloader::Constructor_OSSpecific()
{
	// Make sure we only close what's been opened
	mhInitialize = mhConnection = mhFile = NULL;
} // Constructor_OSSpecific


tbool CDownloader::OpenConnection()
{
	// First verify that computer is connected to the Internet
	DWORD dwConnectedState = 0;
	if (
		(!::InternetGetConnectedState(&dwConnectedState, 0))
		||
		(dwConnectedState & INTERNET_CONNECTION_OFFLINE)
	) {
		SetError("No Internet connection");
		return false;
	}

	mhInitialize = ::InternetOpen("Koblo INet Engine 1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (mhInitialize == NULL) {
		SetError("InternetOpen(..) failed");
		return false;
	}
	// 3 seconds of *connecting* should be quite enough - even for very slow analogue modems...
	tint32 iConnectTimeOutMs = 3000; 
	if (!InternetSetOption(Initialize, INTERNET_OPTION_CONNECT_TIMEOUT, &iConnectTimeOutMs, sizeof(iConnectTimeOutMs))) {
		SetError("Unable to set connection time-out");
		return false;
	}

	// Create pseudo-unique context identifier for the connection
	mdwContextID = ++dwContextID_Seed;
	// Connect to server
	mhConnection = InternetConnect(
		mhInitialize,
		msHost.c_str(), miPort,	msUser.c_str(), msPassword.c_str(),
		INTERNET_SERVICE_HTTP, 0, &mdwContextID);
	if (mhConnection == NULL) {
		SetError("InternetConnect(..) failed");
		return false;
	}

	// Prepere media-type
	tchar* pszMediaType = NULL;
	switch (meDesiredMIMEType) {
		case DESIRED_TYPE_HTML:		pszMediaType = "text/html";		break;
		case DESIRED_TYPE_XML:		pszMediaType = "text/xml";		break;
		case DESIRED_TYPE_BINARY:	pszMediaType = "application/octet-stream"; break;
		case DESIRED_TYPE_OGG:		pszMediaType = "audio/ogg";		break;
		case DESIRED_TYPE_MP3:		pszMediaType = "audio/mpeg";	break;
		
		default:
		case DESIRED_TYPE_TEXT:		pszMediaType = "text/*";		break
	}
	// Open document
	mhFile = ::HttpOpenRequest(
		mhConnection,
		(miParamsAssembledLen > 0) ? "POST" : "GET",
		msPage.c_str(),
		NULL, NULL, pszMediaType,
		INTERNET_FLAG_NO_CACHE_WRITE|INTERNET_FLAG_RELOAD,
		&mdwContextID);

	if (mhFile == NULL) {
		SetError("HttpOpenRequest(..) failed");
		return false;
	}

	tchar* pszAdditionalHeaders = NULL;
	tint32 iAdditionHeadersLen = 0;
	if (miParamsAssembledLen > 0) {
		pszAdditionalHeaders = "Content-Type: application/x-www-form-urlencoded";
		iAdditionHeadersLen = strlen(pszAdditionalHeaders);
	}
	if (!::HttpSendRequest(mhFile,
		pszAdditionalHeaders, iAdditionHeadersLen,
		mpszParamsAssembled, miParamsAssembledLen)
	){
		SetError("HttpSendRequest(..) failed");
		return false;
	}

	muiTotalSize = 0;
	DWORD dwBufferSize = sizeof(muiTotalSize);
	DWORD dwHeaderIndex = 0;
	tbool bQueryOK = ::HttpQueryInfo(
		mhFile,
		HTTP_QUERY_CONTENT_LENGTH, &muiTotalSize, &dwBufferSize,
		&dwHeaderIndex);
	if (!bQueryOK) {
		// Fall back to largest possible value
		muiTotalSize = ^((tuint64)0);
	}

	return true;
} // OpenConnection


void CDownloader::CloseConnection()
{
	if (mhFile) {
		::InternetCloseHandle(mhFile);
		mhFile = NULL;
	}
	if (mhConnection) {
		::InternetCloseHandle(mhConnection);
		mhConnection = NULL;
	}
	if (mhInitialize) {
		::InternetCloseHandle(mhInitialize);
		mhInitialize = NULL;
	}
} // CloseConnection


tbool CDownloader::DownloadPortion(tchar* pszBuffer, tint32 iBufferSize, tint32* piPortionSize, tuint64* puiTotalSize)
{
	if (!mbIsInitialized) {
		SetError("Not initialized");
		return false;
	}

	if (mbIsFailed) {
		//SetError("Previous error");
		return false;
	}

	tbool bFirstTime = (!mbIsDownloading);
	mbIsDownloading = true;
	if (bFirstTime) {
		if (!AssembleParams()) return false;
		if (!OpenConnection()) {
			CloseConnection();
			return false;
		}
		// We're alive
		RefreshAlive();
	}

	return false;
} // DownloadPortion


tbool CDownloader::Abort()
{
	if (mbIsDownloading) {
		// TODO: Add functionality...
		
		CloseConnection();
		mbIsDownloading = false;
	}

	WipeParams();

	return true;
} // Abort


