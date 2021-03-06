


#include "KSOS.h"

CKSFile_Controller::CKSFile_Controller()
{
	msProject_Name = "";
	
//	mpMIDI_Sequencer = new(CMIDI_Sequencer);
	
//	char pszInFile[1024];
//	char pszOutFile[1024];
	
	
//	mpMIDI_Sequencer->Init( pszInFile, pszOutFile);
}

CKSFile_Controller::~CKSFile_Controller()
{
	
}

tbool CKSFile_Controller::Open_Project()
{
/*	
	// take care of not owerwriting a project withour  warning
	if (Save_Before_Close() == giUser_Canceld_Save) {
		return giUser_Canceld_Save;
	}
*/	
	
	// msExtendedError = "";
	CAutoDelete<ge::IOpenDialog> pDlg(ge::IOpenDialog::Create());
	
	// Default Name
	tchar pszDefaultFolder[1024];
	tchar pszXML_Path[1024];
	gpApplication->GetDefaultProjectFolder(pszDefaultFolder);
	
	pDlg->DoDialog(pszXML_Path, pszDefaultFolder, "*.xml", "KS Project (*.xml)", "New Project.xml");
	
	if (pszXML_Path[0] == 0) {
		// Exit with no error
		gpApplication->Start_Timer();
		return true;
	}
	// loading project
	gpApplication->PlaybackStop();
	gpApplication->Stop_Timer();
	// Avoid repeated GUI updates on load
	gpApplication->SetGUIsReady(false);
	/*
	// delete project name
	Project_Name("");
	
	// delete online project name
	Online_Project_Name("");
	*/
	// clean project
	gpApplication->Clean_Project(0);
	
	CAutoLock Lock( gpApplication->GetMutex() );
	
	// find project name
	std::string sProject_Name			= pszXML_Path;
	tint32 iPosColon					= sProject_Name.find_last_of(':');
	sProject_Name.erase(0, iPosColon + 1);
	
	// remove extencion

	tint32 iPosDot					= sProject_Name.find_last_of('.');
	sProject_Name.erase(iPosDot, sProject_Name.size() );
	// store project name
	Project_Name(sProject_Name);
	
	// find and store project folder
	std::string sProject_Folder			= pszXML_Path;
	iPosColon							= sProject_Folder.find_last_of(':');
	sProject_Folder.erase(iPosColon+1, sProject_Folder.size() );
	Project_Folder(sProject_Folder);
	
	// read the project file
	gpApplication->Read_Project_From_Disk(Project_Folder() +  Project_Name() + ".xml" );
	
	// gui is readdy
	gpApplication->SetGUIsReady(true);
	
	// set windows
	gpApplication->Update_Windows();
	
	// force update now
	gpApplication->Stack_Tracks();
	gpApplication->Start_Timer();
	
	
	return true;
}

tint32 CKSFile_Controller::New_Project()
{
/*
	// take care of not owerwriting a project withour  warning
	if (Save_Before_Close() == giUser_Canceld_Save) {
		
	//	gpApplication->Start_Timer();
		return giUser_Canceld_Save;
	}
*/			

	// get default folder
	tchar pszDefault_Folder[1024];
	gpApplication->GetDefaultProjectFolder(pszDefault_Folder);

	// open new project dialog
	tchar pszProject_Folder[1024];
	CAutoDelete<ge::ISaveAsDialog> pDlg(ge::ISaveAsDialog::Create());
	pDlg->DoDialog(pszProject_Folder, pszDefault_Folder, "", "", "New Project");
	std::string sProject_Folder = pszProject_Folder;

	
	// user canceled operation
	if (pszProject_Folder[0] == 0)
		return giUser_Canceld_Create_New_Project;
		
	// clean project
	gpApplication->Clean_Project(giDefault_Number_Of_Tracks);
	
	
	
	// update path's and names for files and folders
	Update_Project_Name(sProject_Folder);
	
	// create new folders for the project
	Create_Folders();
	
	// create a new project file
	Create_Project_File();
	
	// save project so name in file isn't lost
	Save_Project();
	
//	gpApplication->Start_Timer();
	
	return giUser_Created_A_New_Project;
		
	
	
}

tint32 CKSFile_Controller::Save_Before_Close()
{
	// If a project is alreaddy loaded
	if (gpApplication->Project_Name().size()) {
		

		// Warning dialog
		tchar pszMsg[1024];
		sprintf( pszMsg, "Save changes to '%s before closing'\n", Project_Name().c_str() );
		ge::IWindow::EMsgBoxReturn eRet;
		eRet = ge::IWindow::ShowMessageBox(pszMsg, "!Warning", ge::IWindow::MsgBoxYesNoCancel);
		
			
		// return value
		switch (eRet) {
				
			case ge::IWindow::MsgBoxRetYes:{
				Save_Project();
				return giUser_Saved_Project;
			}
			case ge::IWindow::MsgBoxRetNo:
				return giUser_Didn_Save;
				
			case ge::IWindow::MsgBoxRetCancel: 
				return giUser_Canceld_Save; 
				
		}
	}
	return giUser_No_Project_To_Save;
}

tbool CKSFile_Controller::Overwrite_Project()
{
	/*
	// If a project is alreaddy loaded
	if (gpApplication->Project_Name().size()) {
		
		// Warning dialog
		tchar pszMsg[1024];
		sprintf( pszMsg, "Overwriting project '%s '\n", Project_Name().c_str() );
		ge::IWindow::EMsgBoxReturn eRet;
		eRet = ge::IWindow::ShowMessageBox(pszMsg, "!Warning", ge::IWindow::MsgBoxOKCancel);
		
		// return value
		switch (eRet) {
				
			case ge::IWindow::MsgBoxRetOK:
				return true;

			case ge::IWindow::MsgBoxRetCancel: 
				return false; 
				
		}
	}
	 */
	return true;
}

tbool CKSFile_Controller::Save_Project()
{
//	printf("Save_Project()\n");
	gpApplication->Save_Project_As_XML_File_To_Disk();
	return true;
	
}

tbool CKSFile_Controller::Save_As()
{
	// store copy from locations
	msCopy_From_Project_Folder	=	gpApplication->Project_Folder();
	msCopy_From_Project_Name	=	msProject_Name;
	
	// default folder
	tchar pszDefault_Folder[1024];
	gpApplication->GetDefaultProjectFolder(pszDefault_Folder);
	
	// open new project dialog
	tchar pszProject_Folder[1024];
	std::string sName_Copy	=	gpApplication->Project_Name();
	sName_Copy				+= " Copy";
	
	CAutoDelete<ge::ISaveAsDialog> pDlg(ge::ISaveAsDialog::Create());
	pDlg->DoDialog(pszProject_Folder, pszDefault_Folder, "", "", sName_Copy.c_str());
	
	// user canceled operation
	if (pszProject_Folder[0] == 0)  return false;
	
	// update project name and path's
	Update_Project_Name(pszProject_Folder);

	// create folders for audio midi etc...
	Create_Folders();
	
	// Save settings in project file
	gpApplication->Save_Project_As_XML_File_To_Disk();
	
	// copy samples and files from old project in to the new project folder
	Copy_Project();
	
	return true;

}


tbool CKSFile_Controller::Copy_Project()
{
	if(gpApplication->Project_Is_Imported()) {
		
		Copy_KSProject_Waves();
	}
	else{
		
		Copy_Samples();
	}
		
	return true;
	
}

tbool CKSFile_Controller::Close_Project()
{
	/*
	// take care of not owerwriting a project withour  warning
	if (Save_Before_Close() == giUser_Canceld_Save) 
		return giUser_Canceld_Save;
	*/
	// clean project
	gpApplication->Clean_Project(0);
	
	// delete project name
	gpApplication->Project_Name("");
	
	// delete online project name
	gpApplication->Online_Project_Name("");
	
	gpApplication->Clear_Project_UUID();
	
	return true;
}

tbool CKSFile_Controller::Copy_KSProject_Waves()
{
	/*
	CAutoDelete<IFileSearch> pSearch(IFileSearch::Create());
	
	
	// Old project folder
	std::string sCopy_From_Folder = gpApplication->GetProjDir();
	sCopy_From_Folder += "Contents:Audio:Clips:Decompressed:";
	
	
	std::string sCopy_To_Folder = gpApplication->Project_Folder() + "Wave Files:";
	pSearch->Init2((sCopy_From_Folder + "*.wav").c_str());
	
	
	tchar pszFile_Name[1024];
	tbool bDir;
	
	
	while (pSearch->GetNext(pszFile_Name, bDir)) {
		
		std::string sFile_Name = pszFile_Name;
		IFile::CopyFile(sCopy_To_Folder.c_str(), sCopy_From_Folder.c_str(), sFile_Name.c_str());
		
	}
	*/
	return true;
}


tbool CKSFile_Controller::Create_Project_Folder()
{
	IFile::CreateDirectory(gpApplication->Project_Folder().c_str());
	// make check here
	return true;
}

//! create a new sample folder inside the project folder
tbool CKSFile_Controller::Create_Audio_Folders()
{
	std::string sFolder = gpApplication->Project_Folder() + "Wave Files";
	IFile::CreateDirectory(sFolder.c_str());
	// make check here
	return true;
}

//! create a new midi folder inside the project folder
tbool CKSFile_Controller::Create_Midi_Folder()
{
	std::string sFolder = gpApplication->Project_Folder() + "MIDI FIles";
	IFile::CreateDirectory(gpApplication->Project_Folder().c_str());
	// make check here
	return true;
}

//! create a new plug-in data folder inside the project folder
tbool CKSFile_Controller::Create_Plugin_Data_Folder()
{
	std::string sFolder = gpApplication->Project_Folder() + "Plug-In's Settings";
	IFile::CreateDirectory(sFolder.c_str());
	// make check here
	return true;
}

//! create a new download folder inside the project folder
tbool CKSFile_Controller::Create_OGG_Folder()
{
	std::string sFolder = gpApplication->OGG_File_Folder();
	IFile::CreateDirectory(sFolder.c_str());
	// make check here
	return true;
}

//! create a new uplaod folder inside the project folder
tbool CKSFile_Controller::Create_MP3_Folder()
{
	std::string sFolder = gpApplication->MP3_File_Folder();
	IFile::CreateDirectory(sFolder.c_str());
	// make check here
	return true;
}

//! create a new uplaod folder inside the project folder
tbool CKSFile_Controller::Create_Wave_Picts_Folder()
{
	std::string sFolder = gpApplication->Project_Folder() + "Wave Picts";
	IFile::CreateDirectory(sFolder.c_str());
	// make check here
	return true;
}




//! create a new project file
tbool CKSFile_Controller::Create_Project_File()
{
	std::string sProject_Name	=	gpApplication->Project_Name();
	std::string sProject_Folder =	gpApplication->Project_Folder();
	std::string sProject		=  sProject_Folder + sProject_Name + ".xml";
	
	
	CAutoDelete<IFile> pfProject_File(IFile::Create());
	pfProject_File->Open(sProject.c_str(), IFile::FileCreate);
	
	
	if (pfProject_File) {
		return true;
	}
	return false;
}




tbool CKSFile_Controller::Copy_Samples()
{
	CAutoDelete<IFileSearch> pSearch(IFileSearch::Create());
	
	
	// copy from project folder
	std::string sCopy_From_Folder = msCopy_From_Project_Folder + "Wave Files:";
	pSearch->Init2((sCopy_From_Folder + "*.wav").c_str());
	
	
	std::string sCopy_To_Folder = gpApplication->Project_Folder() + "Wave Files:";
	
	
	
	tchar pszFile_Name[1024];
	tbool bDir;
	while (pSearch->GetNext(pszFile_Name, bDir)) {
		
		std::string sFile_Name = pszFile_Name;
		IFile::CopyFile(sCopy_To_Folder.c_str(), sCopy_From_Folder.c_str(), sFile_Name.c_str());
		
	}
	
	return true;
}

tbool CKSFile_Controller::Create_Folders()
{

	// create a new project folder
	if(Create_Project_Folder() == false ) return false;
	
	// create a new project folder
	if(Create_Audio_Folders() == false ) return false;
	
	// create a midi project folder
	if(Create_Midi_Folder() == false ) return false;
	
	// create a plug-in data project folder
	if(Create_Plugin_Data_Folder() == false ) return false;
	
	// create a new OGG folder
	if(Create_OGG_Folder() == false ) return false;
	
	// create a new MP3 folder
	if(Create_MP3_Folder() == false ) return false;
	
	// create a new wave pict folder
	if(Create_Wave_Picts_Folder() == false ) return false;
	
	// create a new project file
//	if( Create_Project_File() == false ) return false;

	return true;

}

void CKSFile_Controller::Update_Project_Name(std::string sNew_Name)
{
	// set project folder
	gpApplication->Project_Folder(sNew_Name + ":");
	
	// set project name
	std::string sProject_Name = sNew_Name;
	tint iPos = sProject_Name.find_last_of(':');
	sProject_Name = sProject_Name.substr(iPos + 1, sProject_Name.size());
	gpApplication->Project_Name(sProject_Name);
	gpApplication->Online_Project_Name(gpApplication->Temp_Online_Project_Name() );
	
//	printf( "Update_Project_Name : %s \n", sProject_Name.c_str() );
	

	
}

std::string  CKSFile_Controller::Get_Extencion(std::string sFile)
{
	tint iPos	=	sFile.find_last_of('.');
	sFile		=	sFile.substr(iPos, sFile.size());
	return sFile;
	
}

tbool CKSFile_Controller::Readable_Audio(std::string sFile_Path, tbool* pbIsStereo /*= NULL*/)
{

	CAutoDelete<IFile> pFile(IFile::Create());
	
	
	if (pFile->Open(sFile_Path.c_str(), IFile::FileRead)) {
		
		ac::IDecoder* pDec = ac::IDecoder::Create(pFile);
		
		if (pDec) {
			
			tbool bReturn = pDec->TestFile(pFile);
			if (pbIsStereo) *pbIsStereo = pDec->miChannels >= 2;
			pDec->Destroy();
			return  bReturn;
			//return true;
		}
	}
	return false;
}


tbool CKSFile_Controller::Is_A_File(std::string sFile_Path)
{
	
	CAutoDelete<IFile> pFile(IFile::Create());
	
	if (pFile->Open(sFile_Path.c_str(), IFile::FileRead)) {
		return true;
	}
	return false;
}












