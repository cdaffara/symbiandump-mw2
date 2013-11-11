// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __EMAILSMOKECOMMANDS_H
#define __EMAILSMOKECOMMANDS_H
#include "emailtestutils.h"
#include "emailsmoke.h"

const TInt KMaxTestBodyTxt = 200;

//
//
// CCleanMessageFolder
//

class CCleanMessageFolder : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCleanMessageFolder(CEmailTestUtils& aTestUtils);

protected:
	CEmailTestUtils& iTestUtils;
	};


//
//
// CDumpMailStore
//

class CDumpMailStore : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CDumpMailStore(CEmailTestUtils& aTestUtils, const TDesC& aPath);

protected:
	CEmailTestUtils& iTestUtils;
	TFileName iPath;
	};


//
//
// CCompareFiles
//

class CCompareFiles : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCompareFiles(CMsvTestUtils& aTestUtils, const TDesC& aPath1, const TDesC& aPath2);

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

protected:
	CMsvTestUtils& iTestUtils;
	TFileName iPath1;
	TFileName iPath2;
	};


//
//
// CDumpToFile
//

class CDumpToFile : public CBase
	{
protected:
	void WriteToFileL(RFile& aFile, const TDesC& aInputLine);
	};


//
//
// CDumpBodyText
//

class CDumpBodyText : public CDumpToFile, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CDumpBodyText(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils, const TDesC& aPath);

private:
	void RemoveRichtextFormating(TDes& aSourceLine);
	void DumpBodyTextToFileL(RFile& aFile, TMsvId& aEntryId);

protected:
	CMsvEntrySelection& iSelection;
	CMsvTestUtils& iTestUtils;
	TFileName iPath;
	};
	

//
//
// CGetAttachmentNames
//

class CGetAttachmentNames : public CDumpToFile, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CGetAttachmentNames(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils, const TDesC& aPath);

private:
	void DumpAttachmentNamesToFileL(RFile& aFile, TMsvId& aEntryId);

protected:
	CMsvEntrySelection& iSelection;
	CMsvTestUtils& iTestUtils;
	TFileName iPath;
	};
	
	
//
//
// CDumpHeaderFields
//

class CDumpHeaderFields : public CDumpToFile, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CDumpHeaderFields(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils, const TDesC& aPath);

private:
	void DumpHeaderFieldsToFileL(RFile& aFile, TMsvId& aEntryId);

protected:
	CMsvEntrySelection& iSelection;
	CMsvTestUtils& iTestUtils;
	TFileName iPath;
	};
	

//`//
//
// CChangeCharset
//

class CChangeCharset : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CChangeCharset(TUint aNewCharset, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils); 

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	TUint iNewCharset;
	};

//
//
// CAddComment
//

class CAddComment : public CBase, public MBaseTestState
	{
public:
	static CAddComment* NewL(const CDesCArrayFlat& aComment, CMsvTestUtils& aTestUtils);
	void StartL(TRequestStatus& aStatus);
	~CAddComment();

private:
	CAddComment(CMsvTestUtils& aTestUtils);
	void ConstructL(const CDesCArrayFlat& aComment);

protected:
	CMsvTestUtils& iTestUtils;
	HBufC* iComment;
	};

//
//
// CCountChildren
//

class CCountChildren : public CBase, public MBaseTestState
	{
public:
	CCountChildren(TMsvId& aParentFolder, CEmailClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

protected:
	CEmailClientTest& iTestHarness;
	TMsvId& iParentFolder;
	};

//
//
// CCheckService
//

class CCheckService : public CBase, public MBaseTestState
	{
public:
	CCheckService(CEmailClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

protected:
	CEmailClientTest& iTestHarness;
	};


//
//
// CGoClientSide
//

class CGoClientSide : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CGoClientSide(CMsvTestUtils& aTestUtils);

protected:
	CMsvTestUtils& iTestUtils;
	};



//
//
// CGoServerSide
//

class CGoServerSide : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CGoServerSide(CMsvTestUtils& aTestUtils);

protected:
	CMsvTestUtils& iTestUtils;
	};



//
//
// CResetMessageServer
//

class CResetMessageServer : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CResetMessageServer(CMsvTestUtils& aTestUtils);

protected:
	CMsvTestUtils& iTestUtils;
	};




//
//
// CCheckMessageFolder
//

class CCheckMessageFolder : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCheckMessageFolder(const TDesC& aFileName, CMsvTestUtils& aTestUtils);

protected:
	CMsvTestUtils& iTestUtils;
	TFileName iFileName;
	};



//
//
// CChangeMsvDrive
//

class CChangeMsvDrive : public CActive, public MBaseTestState
	{
public:
	CChangeMsvDrive(CMsvClientTest& aParentTestHarness, TInt aDrive);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CChangeMsvDrive();

private:
	CMsvClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	TInt iDrive;
	};
	

//
//
// CRemoveFileOrDir
//

class CRemoveFileOrDir : public CBase, public MBaseTestState
	{
public:
	CRemoveFileOrDir(CMsvTestUtils& aTestUtils, const TDesC& aPath);
	void StartL(TRequestStatus& aStatus);

private:
	CMsvTestUtils& iTestUtils;
	TFileName iPath;
	};
	

//
//
// CSelectEntry
//

class CSelectEntry : public CBase, public MBaseTestState
	{
public:
	static CSelectEntry* NewL(const TDesC& aName, const TMsvId& aCurrentFolderId, TMsvId* aEntryId, CMsvTestUtils& aTestUtils);
	static CSelectEntry* NewL(const TDesC& aName, const TMsvId& aCurrentFolderId, CMsvEntrySelection* aSelection, CMsvTestUtils& aTestUtils);
	void StartL(TRequestStatus& aStatus);
	~CSelectEntry();

private:
	CSelectEntry(const TMsvId& aCurrentFolderId, TMsvId* aEntryId, CMsvTestUtils& aTestUtils);
	CSelectEntry(const TMsvId& aCurrentFolderId, CMsvEntrySelection* aSelection, CMsvTestUtils& aTestUtils);
	void ConstructL(const TDesC& aName);

private:
	CMsvTestUtils& iTestUtils;
	const TMsvId& iCurrentFolderId;
	TMsvId* iEntryId;
	CMsvEntrySelection* iEntrySelection;
	HBufC* iEntryName;
	};

/*
class CSelectEntry : public CBase, public MBaseTestState
	{
public:
	static CSelectEntry* NewL(const TDesC& aName, const TMsvId& aCurrentFolderId, TMsvId& aEntryId, CMsvTestUtils& aTestUtils);
	void StartL(TRequestStatus& aStatus);
	~CSelectEntry();

private:
	CSelectEntry(const TMsvId& aCurrentFolderId, TMsvId& aEntryId, CMsvTestUtils& aTestUtils);
	void ConstructL(const TDesC& aName);

private:
	CMsvTestUtils& iTestUtils;
	const TMsvId& iCurrentFolderId;
	TMsvId& iEntryId;
	HBufC* iEntryName;
	};
*/

//`//
//
// CCheckSelectionCount
//

class CCheckSelectionCount : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCheckSelectionCount(TInt aCount, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils); 

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	TInt iTestCount;
	};




//`//
//
// CCheckNewFlag
//

class CCheckNewFlag : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCheckNewFlag(TInt aSelectedIndex, TBool aTestNewStatus, 
				  CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils); 

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	TInt iSelectedIndex;
	TBool iTestNewStatus;
	};



//`//
//
// CCheckPriority
//

class CCheckPriority : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCheckPriority(TInt aSelectedIndex, TInt aTestPriority, 
				   CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils); 

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	TInt iSelectedIndex;
	TBool iTestPriority;
	};


//`//
//
// CCheckAttachment
//

class CCheckAttachment : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCheckAttachment(TInt aSelectedIndex, TBool aTestAttachment, 
					 CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils); 

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	TInt iSelectedIndex;
	TBool iTestAttachment;
	};


//`//
//
// CCheckChildren
//

class CCheckChildren : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCheckChildren(TInt aSelectedIndex, TBool aTestAttachment, 
					 CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils); 

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	TInt iSelectedIndex;
	TBool iTestChildren;
	};

//`//
//
// CCheckBodyText
//

class CCheckBodyText : public CBase, public MBaseTestState
	{
public:
	static CCheckBodyText* NewL(TInt aSelectedIndex, TDesC& aTestBodyText, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils);
	
	~CCheckBodyText();

	void StartL(TRequestStatus& aStatus);
	
private:
	void ConstructL(TDesC& aTestBodyText);
	CCheckBodyText(TInt aSelectedIndex, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils); 

	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	TInt iSelectedIndex;
	HBufC* iTestBodyText;
	};

//`//
//
// CCheckRemoteFolderSize
//

class CCheckRemoteFolderSize : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCheckRemoteFolderSize(TInt aCount, CMsvTestUtils& aTestUtils); 

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CMsvTestUtils& iTestUtils;
	TInt iTestCount;
	};

//`//
//
// CSelectPopulatedInSelection
//

class CSelectPopulatedInSelection : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CSelectPopulatedInSelection(CMsvEntrySelection& aSelection,CMsvTestUtils& aTestUtils); 

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	};

//`//
//
// CSelectLastInSelection
//

class CSelectLastInSelection : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CSelectLastInSelection(TInt aCount, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils); 

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	TInt iTestCount;
	};

//`//
//
// CSelectFirstInSelection
//

class CSelectFirstInSelection : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CSelectFirstInSelection(TInt aCount, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils); 

private:
	CMsvTestUtils& iTestUtils;
	CMsvEntrySelection& iSelection;
	TInt iTestCount;
	};

//
//
// CSelectEntryById
//

class CSelectEntryById : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CSelectEntryById(TMsvId aId, TMsvId& aEntryId, CMsvTestUtils& aTestUtils);

private:
	CMsvTestUtils& iTestUtils;
	TMsvId iIdToSelect;
	TMsvId& iEntryId;
	};

//
//
// CMsvTestEntry
//

class CMsvTestEntry : public CBase, public MBaseTestState
	{
public:
	CMsvTestEntry(CMsvClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	
	void TestVisible(TBool aVisibleValue);
	void TestComplete(TBool aCompleteValue);

protected:
// What to test
	TBool iTestVisible;
	TBool iTestComplete;

// Expected values
	TBool iVisibleValue;
	TBool iCompleteValue;

protected:
	CMsvClientTest& iParentTestHarness;
	};

//
//
// CSelectAllMessages
//

class CSelectAllMessages : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CSelectAllMessages(TMsvId& aParentFolder, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils);

private:
	CMsvTestUtils& iTestUtils;
	TMsvId& iParentFolder;
	CMsvEntrySelection& iSelection;
	};



//
//
// CLongWait
//

class CLongWait : public CActive, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CLongWait();
	CLongWait(TInt aWaitTime);
	~CLongWait();
	void RunL();
	void DoCancel();

private:
	CTestTimer* iTimer;
	TRequestStatus* iReportStatus;
	TInt iWaitTime;
	};




//
//
// CCreateEmailMessage
//

class CCreateEmailMessage : public CActive, public MBaseTestState
	{
public:
	CCreateEmailMessage(const TMsvId& iFolderId, const TMsvId& iServiceId, const TDesC& aFileName, CEmailTestUtils& aTestUtils,TInt aHowMany=1);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CCreateEmailMessage();

private:
	enum TCreateEmailState
		{
		ECreateEmailWait1,
		ECreateEmailWait2
		};

	TCreateEmailState iState;
	const TMsvId& iCurrentFolderId;
	const TMsvId& iServiceId;
	TFileName iFileName;
	CEmailTestUtils& iTestUtils;
	CTestTimer* iTimer;
	TRequestStatus* iReportStatus;
	TInt iHowMany;
	};

//
//
// CCopySelection
//

class CCopySelection : public CActive, public MBaseTestState
	{
public:
	CCopySelection(CMsvClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CCopySelection();

private:
	CMsvClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};


//
//
// CMoveSelection
//

class CMoveSelection : public CActive, public MBaseTestState
	{
public:
	CMoveSelection(CMsvClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CMoveSelection();

private:
	CMsvClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};



//
//
// CDeleteSelection
//

class CDeleteSelection : public CActive, public MBaseTestState
	{
public:
	CDeleteSelection(CMsvClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CDeleteSelection();

private:
	CMsvClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};

//
//
// CDeleteChildren
//

class CDeleteChildren : public CActive, public MBaseTestState
	{
public:
	CDeleteChildren(CMsvClientTest& aParentTestHarness, TInt aSelectedIndex);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CDeleteChildren();

private:
	CMsvClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	TInt iSelectedIndex;
	};


//
//
// CCreateEmailService
//

class CCreateEmailService : public CBase, public MBaseTestState
	{
public:
	void StartL();
	virtual ~CCreateEmailService();
	TMsvEntry& Entry();
	void SetDetailsStringL(const TDesC& aDetails);

protected:
	CCreateEmailService(CEmailClientTest& aTestHarness);

protected:
	CEmailClientTest& iTestHarness;
	TMsvEntry iEntry;
	HBufC* iDetails;
	};


//
//
// CSetIAPs
//

// eh xxxx, this code is now obsolete.
// The IAP preferences should be set by the CCreateEmailService class.

#ifndef _NO_IAP_PREFS

class CImIAPPreferences;

class CSetIAPs : public CBase, public MBaseTestState
	{
public:
	static CSetIAPs* NewL(CMsvClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	CImIAPPreferences& IAPPreferences();
	~CSetIAPs();

private:
	void ConstructL();
	CSetIAPs(CMsvClientTest& aParentTestHarness);

private:
	CMsvClientTest& iParentTestHarness;
	CImIAPPreferences* iIAPPreferences;
	};

#endif // _NO_IAP_PREFS

#endif
