// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "t_converterwrapper.h"
// Utils includes
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>
#include <messagingtestutility2.h>
#include <f32file.h> 
#include <smuthdr.h>
#include <txtrich.h>
#include <msvuids.h>
#include<msvapi.h>
#include <Smut.h>
#include <sqldb.h>
// Enumerations
_LIT(KDBFileName,"\\messaging.db");
_LIT(KDelimiter, " ;");
//_LIT(KInboxId, " 4098");
_LIT(KInboxId, " 4101");
_LIT(KEMsvMessageStoreNotSupported,			"EMsvMessageStoreNotSupported");
_LIT(KEMsvMessageStoreCorrupt,				"EMsvMessageStoreCorrupt");
_LIT(KEMsvMediaChanged,						"EMsvMediaChanged");
_LIT(KEMsvRefreshMessageView,				"EMsvRefreshMessageView");
_LIT(KEMsvDiskNotAvailable,					"EMsvDiskNotAvailable");
_LIT(KEMsvMisMatch,							"Event does not match!");

// Commands
//_LIT(KImapAccountName,"ImapAccountName");
_LIT(KFolderName,"FolderName");
_LIT(KToAddress,"ToAddress%d%d");
_LIT(KCcAddress,"CcAddress%d%d");
_LIT(KBccAddress,"BccAddress%d%d");
_LIT(KFromAddress,"FromAddress%d");
_LIT(KSubject,"Subject%d");

_LIT(KContentType,"ContentType%d");
_LIT(KContentSubType,"ContentSubType%d");

//Converter Commands
_LIT(KNew,									"New");
_LIT(KConversion,							"DoConversion");
_LIT(KOnlyConversion,						"DoOnlyConversion");
_LIT(KGetDriveList,							"GetDriveList");
_LIT(KGetConversionStatus,					"GetConversionStatus");
_LIT(KRestartConversion,					"RestartConversion");
_LIT(KCancelConversion,						"CancelConversion");
_LIT(KMultipleConversionRequests,			"MultipleConversionRequests");
_LIT(KOutofMemory,							"OutofMemory");
_LIT(KMsvServerPattern, 					"!MsvServer*");

// Command parameters
_LIT(KObjectValue,							"object_value");
_LIT(KDrive,								"drive");
_LIT(KExpDrivesCount,				 		"exp_drives_count");
_LIT(KEventType,						    "event_type");
_LIT(KConverterMail2Folder, "c:\\msgtest\\converter\\Mail2");		


/**
Purpose: Constructor of CT_ConverterWrapper class

@internalComponent
*/
CT_ConverterWrapper::CT_ConverterWrapper()
:	iObject(NULL),
	iSession(NULL),
	iObserver(NULL),iActiveCallback(NULL)
	{
	}

/**
Purpose: Destructor of CT_ConverterWrapper class

@internalComponent
*/
CT_ConverterWrapper::~CT_ConverterWrapper()
	{
	delete iObject;
	iObject = NULL;
	// Close Message Server
	if (iSession)
		{
		iSession->CloseMessageServer();
		delete iObject;
		iObject=NULL;
		delete iObserver;
		iObserver = NULL;
		delete iSession;
		iSession = NULL;
		delete iActiveCallback;
		iActiveCallback = NULL;
		TFindProcess find(KMsvServerPattern);
		TFullName name;
		if (find.Next(name) == KErrNone)
			{
			TRequestStatus status;
			RProcess process;
			User::LeaveIfError(process.Open(name));
			process.Logon(status);
			User::WaitForRequest(status);
			User::After(9000000);	
			}
		}
		DoCleanup();	
	}
/**
Purpose: Command fuction of CT_ConverterWrapper class

@internalComponent
*/
CT_ConverterWrapper* CT_ConverterWrapper::NewL()
	{
	CT_ConverterWrapper*	ret = new (ELeave) CT_ConverterWrapper();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}
/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
void CT_ConverterWrapper::ConstructL()
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
	// create the directories
	fs.MkDirAll(_L("D:\\private\\1000484b\\mail2\\"));
	fs.MkDirAll(_L("E:\\private\\1000484b\\mail2\\"));
	fs.MkDirAll(_L("F:\\private\\1000484b\\mail2\\"));
	_LIT(KMail2FolderPathD, "d:\\private\\1000484b\\Mail2");
	_LIT(KMail2FolderPathE, "e:\\private\\1000484b\\Mail2");
	_LIT(KMail2FolderPathF, "f:\\private\\1000484b\\Mail2");
	TRAPD(errfile, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathD,2));
	TRAP(errfile, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathE,2));
	TRAP(errfile, fileMngr->Copy(KConverterMail2Folder,KMail2FolderPathF,2));
	
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();	
	
	iObject			= new (ELeave) TInt;
	iObserver		= new(ELeave)CT_MediaSessionObserver();
	iSession		= CMsvSession::OpenSyncL(*iObserver);
	iActiveCallback  = CActiveCallback::NewL(*this);
	}

/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
TBool CT_ConverterWrapper::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool ret = ETrue;

	// Print out the parameters for debugging
	INFO_PRINTF2( _L("<font size=2 color=990000><b>aCommand = %S</b></font>"), &aCommand );
	INFO_PRINTF2( _L("aSection = %S"), &aSection );
	INFO_PRINTF2( _L("aAsyncErrorIndex = %D"), aAsyncErrorIndex );
	
	if(KNew() == aCommand)
		{
		DoCmdNewL(aSection);
		}
	else if(KOnlyConversion() == aCommand)
		{
		doConversion(aSection);
		}
	else if(KConversion() == aCommand)
		{
		DoCmdDoConversion(aSection);
		}
	else if(KGetDriveList() == aCommand)
		{
		DoCmdGetDriveList(aSection);
		}
	else if(KGetConversionStatus() == aCommand)
		{
		DoCmdGetConversionStatus(aSection);
		}
	else if(KRestartConversion() == aCommand)
		{
		DoCmdCancelConversion(aSection);
		}
	else if(KCancelConversion() == aCommand)
		{
		DoCmdCancelConversion(aSection);
		}
	else if(KMultipleConversionRequests() == aCommand)
		{
		DoCmdHandleMultipleConversionRequests();
		}
	else if(KOutofMemory() == aCommand)
		{
		DoCmdCheckOutofMemory(aSection);
		}
	else
		{
		ret = EFalse;
		}
	return ret;
	}

/**
Purpose: To verify the mime headers of the message
@internalComponent
@param  aSection Current ini file command section
*/
TBool CT_ConverterWrapper::VerifyMimeHeaderPartsforImapL(const TDesC& aSection,CMsvEntrySelection* aEntrySelection)
	{
	TBool status = EPass;
	
	for(int index = 0; index < aEntrySelection->Count(); ++index)
		{
		CMsvEntry* entry = iSession->GetEntryL(aEntrySelection->At(index));
		CleanupStack::PushL(entry);
		CMsvStore* store = entry->ReadStoreL();
		CleanupStack::PushL(store);
		if (store->IsPresentL(KUidMsgFileMimeHeader))
			{
			CImMimeHeader* mimeHeader = CImMimeHeader::NewLC();
			mimeHeader->RestoreL(*store);
	
			TBuf<256> param;
			TPtrC contentType;
			param.Format(KContentType, index);
			if(GetStringFromConfig(aSection, param, contentType))
				{
				TPtrC8 content8 = mimeHeader->ContentType();
				HBufC* content16 = HBufC::NewLC(content8.Length());
				content16->Des().Copy(content8);
				if(contentType.Compare(content16->Des()))
					{
					ERR_PRINTF1(_L("Content Type is not matched"));
					status = EFail;
					}
				CleanupStack::PopAndDestroy(content16);
				}
			TPtrC contentSubType;
			param.Format(KContentSubType, index);
			if(GetStringFromConfig( aSection, param, contentSubType))
				{
				TPtrC8 content8 = mimeHeader->ContentSubType();
				HBufC* content16 = HBufC::NewLC(content8.Length());
				content16->Des().Copy(content8);
				if(contentSubType.Compare(content16->Des()))
					{
					ERR_PRINTF1(_L("Content Sub Type is not matched"));
					status = EFail;
					}
				CleanupStack::PopAndDestroy(content16);
				}
			CleanupStack::PopAndDestroy(mimeHeader);
			}
		CleanupStack::PopAndDestroy(store);
		CleanupStack::PopAndDestroy(entry);
		}
	return status;
	}
	
/**	
Purpose: To verify the headers of the message
@internalComponent
@param  aSection Current ini file command section
*/ 

TBool CT_ConverterWrapper::VerifyImHeaderPartsforImapL(const TDesC& aSection, CMsvEntrySelection* aEntrySelection)
	{
	TBool status = EPass;

	for(int index = 0; index < aEntrySelection->Count(); ++index)
		{
		CMsvEntry* entry = iSession->GetEntryL(aEntrySelection->At(index));
		CleanupStack::PushL(entry);
		CMsvStore* store = entry->ReadStoreL();
		CleanupStack::PushL(store);
		CImHeader* header = CImHeader::NewLC();
		// Retrieve the email message header part from the message store
		header->RestoreL(*store);

		// Validate the header fields
		TBuf<256> param;

		for(int indexTo = 0; indexTo < header->ToRecipients().Count(); ++indexTo)
			{
			TPtrC toAddress;
			param.Format(KToAddress, index, indexTo);
			if(GetStringFromConfig( aSection, param, toAddress))
				{
				TPtrC toAdd = header->ToRecipients()[indexTo];
				if(toAddress.Compare(header->ToRecipients()[indexTo]))
					{
					ERR_PRINTF1(_L(" To Address is not matched"));
					status = EFail;
					}
				}
			}
		for(int indexCc = 0; indexCc < header->CcRecipients().Count(); ++indexCc)
			{
			TPtrC ccAddress;
			param.Format(KCcAddress, index, indexCc);
			if(GetStringFromConfig( aSection, param, ccAddress))
				{
				TPtrC ccAdd = header->CcRecipients()[indexCc];
				if(ccAddress.Compare(header->CcRecipients()[indexCc]))
					{
					ERR_PRINTF1(_L("CC Address is not matched"));
					status = EFail;
					}
				}
			}
		for(int indexBcc = 0; indexBcc < header->BccRecipients().Count(); ++indexBcc)
			{
			TPtrC bccAddress;
			param.Format(KBccAddress, index, indexBcc);
			if(GetStringFromConfig( aSection, param, bccAddress))
				{
				TPtrC bccAdd = header->BccRecipients()[indexBcc];
				if(bccAddress.Compare(header->BccRecipients()[indexBcc]))
					{
					ERR_PRINTF1(_L("BCC Address is not matched"));
					status = EFail;
					}
				}
			}
		TPtrC fromAddress;
		param.Format(KFromAddress, index);
		if(GetStringFromConfig( aSection, param, fromAddress))
			{
			TPtrC fromAdd = header->From();
			if(fromAddress.Compare(header->From()))
				{
				ERR_PRINTF1(_L("From Address is not matched"));
				status = EFail;
				}
			}
		TPtrC subject;
		param.Format(KSubject, index);
		if(GetStringFromConfig( aSection, param, subject))
			{
			TPtrC tsub = header->Subject();
			if(subject.Compare(header->Subject()))
				{
				ERR_PRINTF1(_L("Subject is not matched"));
				status = EFail;
				}
			}
		CleanupStack::PopAndDestroy(header);
		CleanupStack::PopAndDestroy(store);
		CleanupStack::PopAndDestroy(entry);
		}
	return status;
	}
	

	
	
	
/**	
Purpose: Handle the session event 

@internalComponent
@param  aEvent event type of session
@param  aArg1  NA 
@param  aArg2  NA
@param  NA 
*/

void CT_MediaSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*)
	{
	if (aEvent)
		iType = aEvent;
	if (aArg1)
		iArg1 = *(TInt*)aArg1;
	if (aArg2)
		iArg2 = *(TInt*)aArg2;
	}

TBool CT_ConverterWrapper::GetVerdictFromConfig(const TDesC& aSection, const TDesC& aParameterName, MMsvSessionObserver::TMsvSessionEvent& aEvent)
	{
	// Read drives mapping operation name from INI file
	TPtrC eventStr;
	TBool ret = GetStringFromConfig(aSection, aParameterName, eventStr);

	if (ret)
		{
		if (eventStr == KEMsvMessageStoreNotSupported())
			{
			aEvent = MMsvSessionObserver::EMsvMessageStoreNotSupported;
			}
		else if (eventStr == KEMsvMessageStoreCorrupt())
			{
			aEvent = MMsvSessionObserver::EMsvMessageStoreCorrupt;
			}
	 	else if (eventStr == KEMsvMediaChanged())
			{
			aEvent = MMsvSessionObserver::EMsvMediaChanged;
			}
		else if (eventStr == KEMsvRefreshMessageView())
			{
			aEvent = MMsvSessionObserver::EMsvRefreshMessageView;
			}
		else if (eventStr == KEMsvDiskNotAvailable())
			{
			aEvent = MMsvSessionObserver::EMsvDiskNotAvailable;
			}
		}
	return ret;
	}
	
/**
Purpose: Capture the notification

@internalComponent
@param  aSection Current ini file command section
*/ 
TBool CT_ConverterWrapper::CaptureEvent(const TDesC& aSection)
	{
	CActiveScheduler::Start();
	MMsvSessionObserver::TMsvSessionEvent event;
	
	if (!GetVerdictFromConfig(aSection, KEventType, event))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KEventType());
		return EFalse;
		}

	const TDesC& expStr = ConvertToStr(event);
	INFO_PRINTF2( _L("<font color=990000>Expected event = %S</font>"), &expStr );

	const TDesC& actualStr = ConvertToStr(iObserver->iType);
	INFO_PRINTF2( _L("<font color=990000>Actual event = %S</font>"), &actualStr );
	
	if ( iObserver->iType != event )
		{
		ERR_PRINTF3(_L("<font color=FF0000>Expected event: %S is not equal to Actual event: %S </font>"), &expStr, &actualStr);
		return EFalse;
		}
	return ETrue;
	}


const TDesC& CT_ConverterWrapper::ConvertToStr(MMsvSessionObserver::TMsvSessionEvent& aEvent ) 
	{
	if( aEvent == MMsvSessionObserver::EMsvMessageStoreNotSupported)
		return KEMsvMessageStoreNotSupported();
	else if ( aEvent == MMsvSessionObserver::EMsvMessageStoreCorrupt)
		return KEMsvMessageStoreCorrupt();
	else if ( aEvent == MMsvSessionObserver::EMsvMediaChanged)
		return KEMsvMediaChanged();
	else if ( aEvent == MMsvSessionObserver::EMsvRefreshMessageView)
		return KEMsvRefreshMessageView();
	else if ( aEvent == MMsvSessionObserver::EMsvDiskNotAvailable)
		return KEMsvDiskNotAvailable();
	else 
		return KEMsvMisMatch();
	}

/**
Purpose: To create a new object of the CTEFTest type through the API.

Ini file options:
	iniData - The data from the ini file at the section provided.

@internalComponent
@param  aSection Current ini file command section
*/
void CT_ConverterWrapper::DoCmdNewL(const TDesC& aSection)
	{
	TInt objectValue = 0;
	if (!GetIntFromConfig(aSection, KObjectValue(), objectValue))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KObjectValue());
		SetBlockResult(EFail);
		}
	else
		{
		delete iObject;
		iObject = new (ELeave) TInt(objectValue);
		}
	}

/**
Purpose: Perform the conversion operation

@internalComponent
@param  aSection Current ini file command sectio
*/
void CT_ConverterWrapper::DoCmdDoConversion(const TDesC& aSection)
	{
	TPtrC folderName;
	TPtrC accountName;
	TPtrC popAccountName;
	TInt drive;
	
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}	

	// Start the conversion 
	TDriveNumber driveArray = (TDriveNumber)drive;
	TUint32 startTest =  User::NTickCount();
	TRAPD(err1, iSession->ConvertMessageStore(driveArray,iActiveCallback->iStatus));
	iActiveCallback->Activate(ETrue);

	if (!CaptureEvent(aSection))
		{
		SetBlockResult( EFail );
		}
	else
		{
		SetBlockResult( EPass );
		}
	if(err1 == KErrNone)
		{
		SetBlockResult( EPass );
		}
	else
		{
		SetBlockResult( EFail );
		}

	TUint32 endTest =  User::NTickCount();
	INFO_PRINTF2(_L("Time take for conversion is %d milliseconds"),(endTest-startTest));
	TInt priority = 1;
	TRAPD( err2, iSession->UpdateDrivePriorityL(driveArray,(TUint&)priority));	
	if(err2==KErrNone)
		{
		SetBlockResult( EPass );
		}
	else
		{
		SetBlockResult( EFail );
		}

	TMsvId serviceId;
	
	// The drive would have been attached.Remove the drive so that it is possible to open it 	 
	//iSession->RemoveDriveL(TDriveUnit(driveArray));
	TParse parse;
	TPtrC drive1(TDriveUnit(driveArray).Name());
	parse.Set(KDBFileName, &drive1, NULL);
	TFileName dbFile = parse.FullName();

	RSqlDatabase tempDB;
	CleanupClosePushL(tempDB);
	tempDB.OpenL(dbFile);
	TSqlScalarFullSelectQuery query(tempDB);	
	//Get the service id of the imap  account 
	_LIT(KStandardEntryExistsQuery1, "SELECT id FROM IndexEntry WHERE details like 'ImapAccountA' and mtmid = 268439594 ");
	 serviceId = query.SelectIntL(KStandardEntryExistsQuery1);
	_LIT(KStandardEntryExistsQuery2, "SELECT id FROM IndexEntry WHERE   details like 'Inbox' and serviceId= ");
	RBuf16 queryBuf;
	TInt len =  KStandardEntryExistsQuery2().Length() + 15;
	queryBuf.Create(len);
	CleanupClosePushL(queryBuf);
	queryBuf.Append(KStandardEntryExistsQuery2);
	queryBuf.AppendNum(serviceId);
	queryBuf.Append(KDelimiter);
	RSqlStatement getStmt;
	CleanupClosePushL(getStmt);
	User::LeaveIfError(getStmt.Prepare(tempDB, queryBuf));
	TInt inboxId;

	TInt columnIndex = getStmt.ColumnIndex(_L("id"));
	inboxId=query.SelectIntL(queryBuf);	
	CMsvEntrySelection* childrenSelection = CT_MsgUtils::CreateChildrenSelectionL(iSession,inboxId);
	CleanupStack::PushL(childrenSelection);

	// Verify the header parts
	TBool status = VerifyImHeaderPartsforImapL(aSection,childrenSelection);
	if (status == EFail )
		{
			SetBlockResult(EFail);
		}
	// Verify mime header parts
	status = VerifyMimeHeaderPartsforImapL(aSection,childrenSelection);
	if (status == EFail )
		{
			SetBlockResult(EFail);
		}
	CleanupStack::PopAndDestroy(childrenSelection);

	// Get the children under inbox and fetch the index entry of the message
	_LIT8(KGetEntryQuery1, "SELECT * from IndexEntry where parentId = ");
	_LIT(KGetEntryQuery2," Limit 1");
	RBuf8 queryBuf1;
	len = KGetEntryQuery1().Length()+KGetEntryQuery2().Length()  + 15;
	queryBuf1.Create(len);
	CleanupClosePushL(queryBuf1);
	queryBuf1.Append(KGetEntryQuery1);
	queryBuf1.AppendNum(inboxId);
	queryBuf1.Append(KGetEntryQuery2);
	queryBuf1.Append(KDelimiter);
	RSqlStatement getStmt1;
	CleanupClosePushL(getStmt1);
	User::LeaveIfError(getStmt1.Prepare(tempDB, queryBuf1));
	// Fetch the index entries 	
	RBuf16  buf;
	CleanupClosePushL(buf);
	buf.Create(10000);
	TInt i;
	TInt err;
	while((err = getStmt1.Next()) == KSqlAtRow)   
	 {  
	 for(  i=0;i<17;i++)
	 	{
	 		buf.AppendNum(getStmt1.ColumnInt(i));
	 	}
	 for(;i<19;i++)
	 	{
	 		buf.Append(getStmt1.ColumnTextL(i));
	 	}
	  }
	
	CleanupStack::PopAndDestroy(4); 

	// Start for the POP account
	_LIT(KStandardEntryPopExistsQuery1, "SELECT id FROM IndexEntry WHERE details like 'PopAcnt001' and mtmid = 268439592 ");

	serviceId = query.SelectIntL(KStandardEntryPopExistsQuery1);
	
	// Got the child for a particular service under the standard folder
	
	_LIT(KStandardEntryPopExistsQuery2, "SELECT * FROM IndexEntry WHERE serviceId= ");
	_LIT(KStandardEntryPopExistsQuery3, " AND parentid= ");
	_LIT(KStandardEntryPopExistsQuery4," Limit 1");
	RBuf16 queryBufpop;
	len =  KStandardEntryPopExistsQuery2().Length()+KStandardEntryPopExistsQuery3().Length() +KStandardEntryPopExistsQuery4().Length()  + 15;
	queryBufpop.Create(len);
	CleanupClosePushL(queryBufpop);
	queryBufpop.Append(KStandardEntryPopExistsQuery2);
	queryBufpop.AppendNum(serviceId);
	queryBufpop.Append(KStandardEntryPopExistsQuery3);
	queryBufpop.Append(KInboxId);
	queryBufpop.Append(KStandardEntryPopExistsQuery4);
	queryBufpop.Append(KDelimiter);

	RSqlStatement getStmtpop;
	CleanupClosePushL(getStmtpop);
	User::LeaveIfError(getStmtpop.Prepare(tempDB, queryBufpop));
	
	// Verification for POP messages
	
	columnIndex = getStmtpop.ColumnIndex(_L("id"));
	inboxId=0;
	RBuf16  aPop;
	CleanupClosePushL(aPop);
	aPop.Create(10000);
	
	while((err = getStmtpop.Next()) == KSqlAtRow)   
		 {    
		 for(i=0;i<17;i++)
		 	{
		 	aPop.AppendNum(getStmtpop.ColumnInt(i));
 		 	}
	 
		for(;i<19;i++)
		 	{
		  	aPop.Append(getStmtpop.ColumnTextL(i));
		 	}
		 }
	 CleanupStack::PopAndDestroy(2);
	 CleanupStack::PopAndDestroy(3);  
	 
	
	// Change the drive to c: 
	TRAP( err2, iSession->UpdateDrivePriorityL(TDriveNumber(EDriveC),(TUint&)priority));
	if(err2==KErrNone)
		{
		SetBlockResult( EPass );
		}
	else
		{
		SetBlockResult( EFail);
		}
	iSession->RemoveDriveL(driveArray);
	}

/**
Purpose: RunL function for the active requests

@internalComponent
@param  aSection Current ini file command section
*/
void CT_ConverterWrapper:: RunL(CActive* aActive, TInt aIndex)	
	{
	aActive=aActive;
	TInt err = iActiveCallback->iStatus.Int();
	SetAsyncError(aIndex,err);
	CActiveScheduler::Stop();	
	}

/**
Purpose: Test for getconvertible drive

@internalComponent
@param  aSection Current ini file command section
*/

void CT_ConverterWrapper::DoCmdGetDriveList(const TDesC& aSection)
	{
	
	TPtrC exp_drives_count;
	
	if(!GetStringFromConfig( aSection, KExpDrivesCount, exp_drives_count))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KFolderName());
		SetBlockResult(EFail);
		}
	RArray<TDriveNumber>aDriveList;
	TRAPD(err, iSession->GetConvertibleDriveListL(aDriveList));
	
	if(err==KErrNone)
		{
		TInt count;
		TInt supportedDriveCount = 0;
		count=aDriveList.Count();
		
		for(int i=0;i<count;i++)
			{
			//Check whether the drive returned by the API is supported or not
			if(MessageServer::IsMessageStoreSupported(aDriveList[i]))
				{
				supportedDriveCount++;
				}
			}
		if(supportedDriveCount)
			{
			SetBlockResult(EFail);
			}
		else
			{
			SetBlockResult(EPass);
			}
		}
	else
		{
		SetBlockResult(EFail);
		}
	}	

/**
Purpose: Cancel the conversion operation 

@internalComponent
@param  aSection Current ini file command section
*/ 
void CT_ConverterWrapper::DoCmdCancelConversion(const TDesC& aSection)
	{
	TInt drive;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		}

	//	CancelConversionL
	TDriveNumber driveArray = (TDriveNumber)drive;
	TRAPD(err, iSession->CancelConversion(driveArray));


	//if db exists fail the test
	_LIT(KTestDbFileName, "d:\\messaging.db");
	RFs fsSession;
	RFile file;
	User::LeaveIfError(fsSession.Connect()); 
	CleanupClosePushL(fsSession);

	TInt err1=file.Open(fsSession,KTestDbFileName,EFileRead);
	if(err1==KErrNone)
		{
		SetBlockResult(EFail);
		}
	else
		{
		SetBlockResult(EPass);
		}
	CleanupStack::PopAndDestroy();
	}

/**
Purpose: Issue multiple conversion requests and check whether the conversion requests are queued

@internalComponent
@param  aSection Current ini file command section
*/
void CT_ConverterWrapper::DoCmdHandleMultipleConversionRequests()
	{
	//TInt drive;
	_LIT(KTestDbFileName, "d:\\messaging.db");
	_LIT(KTestDbFileName2, "e:\\messaging.db");
	RFs fsSession;
	RFile file;
	User::LeaveIfError(fsSession.Connect()); 
	CleanupClosePushL(fsSession);
	TInt err1=file.Open(fsSession,KTestDbFileName,EFileRead);
	if(err1==KErrNone)
		{
		SetBlockResult(EFail);
		}
		else
		{
		SetBlockResult(EPass);
		}

	User::LeaveIfError(fsSession.Connect()); 
	err1=file.Open(fsSession,KTestDbFileName2,EFileRead);
	if(err1==KErrNone)
		{
		SetBlockResult(EFail);
		}
		else
		{
		SetBlockResult(EPass);
		}
	CleanupStack::PopAndDestroy();
	}


/**
Purpose:Check for the memory leaks during conversion
@internalComponent
@param  aSection Current ini file command section
*/
void CT_ConverterWrapper::DoCmdCheckOutofMemory(const TDesC& aSection)
	{
	TInt drive;

	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}
	TDriveNumber driveArray = (TDriveNumber)drive;
	TInt err = KErrNone;
	__UHEAP_MARK;
	for (TInt k=2;;++k)
		{
		RDebug::Print(_L("loop number - %d"),k);
		__UHEAP_SETFAIL(RHeap::EDeterministic,k);
		__UHEAP_MARK;
		TRAPD(err1, iSession->ConvertMessageStore(driveArray,iActiveCallback->iStatus));
		iActiveCallback->Activate(ETrue);
		CActiveScheduler::Start();
		if(err1==KErrNone)
			{
			SetBlockResult(EFail);
			}
		else
			{
			SetBlockResult(EPass);
			}
		__UHEAP_MARKEND;
		User::Heap().Check();
		if (err==KErrNone)
			break;
		}
		__UHEAP_MARKEND;
		__UHEAP_RESET;
		User::Heap().Check();
		__UHEAP_SETFAIL(RHeap::ENone,0);
		SetBlockResult( EPass );
	}

/**
Purpose:Check whether GetConversionStatus returns appropriate status
@internalComponent
@param  aSection Current ini file command section
*/
void CT_ConverterWrapper::DoCmdGetConversionStatus(const TDesC& aSection)
	{
	TInt drive;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}
		
	TDriveNumber driveArray = (TDriveNumber)drive;
	TUint32 startTest =  User::NTickCount();
	TRAPD(err1, iSession->GetConversionStatusL(driveArray));
	if(err1==KErrNotFound)
		{
		SetBlockResult( EPass );
		}
	else
		{
		SetBlockResult( EFail );
		}
	}

/**
doConversion()
Purpose:Clean up the contents after the tests are completed.
@internalComponent
@param None
*/
void CT_ConverterWrapper::doConversion(const TDesC& aSection)
	{
	TInt drive;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
		}
	if (!CaptureEvent(aSection))
		{
		SetBlockResult( EFail );
		}
	else
		{		
		SetBlockResult( EPass );
		}

	TDriveNumber driveArray = (TDriveNumber)drive;
	TUint32 startTest =  User::NTickCount();
	TRAPD(err1, iSession->ConvertMessageStore(driveArray,iActiveCallback->iStatus));
	iActiveCallback->Activate(ETrue);
	CActiveScheduler::Start();
	if (!CaptureEvent(aSection))
		{
		SetBlockResult( EFail );
		}
	else
		{
		SetBlockResult( EPass );
		}
	if(err1 == KErrNone)
		{
			SetBlockResult( EPass );
		}
	else
		{
			SetBlockResult( EFail );
		}
	}

/**
DoCleanup()
Purpose:Clean up the contents after the tests are completed.
@internalComponent
@param None
*/
void CT_ConverterWrapper::DoCleanup()
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMngr = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMngr);
	
 	TInt err = fileMngr->RmDir(_L("D:\\private"));
	err = fileMngr->RmDir(_L("E:\\private"));	
	err = fileMngr->RmDir(_L("F:\\private"));
	_LIT(KDbFileNameD, "D:\\messaging.db");
	_LIT(KDbFileNameE, "E:\\messaging.db");
	_LIT(KDbFileNameF, "F:\\messaging.db");
	_LIT(KCenrepCre, "c:\\private\\10202be9\\persists\\10286a26.cre");
	err = fileMngr->Delete(KDbFileNameD);
	err = fileMngr->Delete(KDbFileNameE);
	err = fileMngr->Delete(KDbFileNameF);
	err = fileMngr->Delete(KCenrepCre);
	CleanupStack::PopAndDestroy(fileMngr);
	fs.Close();
	}

