
#include "KSOS.h"


CProgressTaskList::~CProgressTaskList()
{
	DeleteList();
} // destructor


void CProgressTaskList::DeleteList()
{
	CAutoLock Lock(mMutex);

	while (listCProgressTask.size()) {
		std::list<CProgressTask*>::iterator it = listCProgressTask.begin();
		CProgressTask* pTask = *it;
		pTask->Destroy();
		listCProgressTask.erase(listCProgressTask.begin());
	}
} // DeleteList


tbool CProgressTaskList::DoWork()
{
	CAutoLock Lock(mMutex);

	if (!IsInProgress_NoLock()) {
		// That's bad - why are we here?
		return false;
	}

	if (mbAbort) {
		// Stop what-ever were doing
		tint32 iNumberOfActions = listCProgressTask.size();
		DeleteList();
		mbAbort = false;
		if (!mbAbort_DontTell) {
			
			tchar pszMsg[1024];
			sprintf( pszMsg, "You aborted %d queued task%s", iNumberOfActions, (iNumberOfActions > 1) ? "s" : "");
			ge::IWindow::ShowMessageBox( std::string("Warning").c_str(),  pszMsg,  ge::IWindow::MsgBoxOK);
			/*
			tchar pszMsg[1024];
			sprintf( pszMsg, "You aborted %d queued task%s", iNumberOfActions, (iNumberOfActions > 1) ? "s" : "");
				gpApplication->ShowMessageBox_NonModal(pszMsg, "Progress aborted");
			 */
		}
		return true;
	}

	tbool bSuccess = false;

	std::list<CProgressTask*>::iterator it = listCProgressTask.begin();
	CProgressTask* pProgressTask = *it;
	bSuccess = pProgressTask->DoWork();
	if (bSuccess) {
		if (pProgressTask->IsDone()) {
			// Advance to next task - if any
			pProgressTask->Destroy();
			it++;
			listCProgressTask.erase(listCProgressTask.begin(), it);
		}
	}

	return bSuccess;
} // DoWork


tbool CProgressTaskList::IsDone()
{
	CAutoLock Lock(mMutex);

	return IsDone_NoLock();
} // IsDone


tbool CProgressTaskList::IsDone_NoLock()
{
	if (!IsInProgress_NoLock()) {
		// No tasks
		return true;
	}

	CProgressTask* pInfo = *(listCProgressTask.begin());
	return pInfo->IsDone();
} // IsDone_NoLock
