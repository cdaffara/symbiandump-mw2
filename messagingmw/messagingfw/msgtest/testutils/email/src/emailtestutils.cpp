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

#include "emailtestutils.h"

#include <c32comm.h>
#include <e32test.h>
#include <f32fsys.h>
#include <s32file.h>
#include <e32uid.h>
#include <txtrich.h>
#include <txtfmlyr.h>
#include <msvids.h>
#include <msvuids.h>
#include <msvruids.h>
#include <e32std.h>
#include <mtclbase.h>
#include <mtclreg.h>
#include <msvapi.h>
#include <mtmuids.h>
#include <smuthdr.h>
#include <miutset.h>
#include <es_sock.h>
#include <in_sock.h>
#include <commdbconnpref.h>
#include <cemailaccounts.h>
#include <iapprefs.h>

#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>
#include <centralrepository.h>

_LIT(KComputerNamePath, "c:\\msgtest\\MachineName.txt");

EXPORT_C CEmailTestUtils* CEmailTestUtils::NewL(RTest& aRTest, TUint aCreationFlags)
	{
	CEmailTestUtils* self = NewLC(aRTest, aCreationFlags);
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CEmailTestUtils* CEmailTestUtils::NewLC(RTest& aRTest, TUint aCreationFlags)
	{
	CEmailTestUtils* self = new (ELeave) CEmailTestUtils(aRTest);
	CleanupStack::PushL(self);
	self->ConstructL(aCreationFlags);
	return self;
	}

EXPORT_C void CEmailTestUtils::InstantiateImapClientMtmL()
	{
	// client side
	iImapClientMtm = (CImap4ClientMtm*) InstantiateClientMtmL(KUidMsgTypeIMAP4, iImapServiceId);
	}

EXPORT_C void CEmailTestUtils::InstantiatePopClientMtmL()
	{
	iPopClientMtm = (CPop3ClientMtm*) InstantiateClientMtmL(KUidMsgTypePOP3, iPopServiceId);
	}

EXPORT_C void CEmailTestUtils::InstantiateSmtpClientMtmL()
	{
	iSmtpClientMtm = (CSmtpClientMtm*) InstantiateClientMtmL(KUidMsgTypeSMTP, iSmtpServiceId);
	}

EXPORT_C void CEmailTestUtils::InstantiateImapServerMtmL()
	{
	iImapServerMtm = (CBaseServerMtm*) InstantiateServerMtmL(KUidMsgTypeIMAP4, iImapServiceId);
	}

EXPORT_C void CEmailTestUtils::InstantiatePopServerMtmL()
	{
	iPopServerMtm = (CBaseServerMtm*) InstantiateServerMtmL(KUidMsgTypePOP3, iPopServiceId);
	}

EXPORT_C void CEmailTestUtils::InstantiateSmtpServerMtmL()
	{
	iSmtpServerMtm = (CBaseServerMtm*) InstantiateServerMtmL(KUidMsgTypeSMTP, iSmtpServiceId);
	}

EXPORT_C void CEmailTestUtils::CreateImapServerMtmRegL()
	{
	CreateServerMtmRegL(KUidMsgTypeIMAP4, _L("IMAP4"), KImap4ServerMtmFileName(), TMsvTestDllInfo(KImap4ClientMtmFileName, 3), KImap4DataLayerFileName(), KImap4DataLayerFileName(), KUidMsvTechnologyGroupEmail, imap4DatFile);
	}

EXPORT_C void CEmailTestUtils::CreateSmtpServerMtmRegL()
	{
	CreateServerMtmRegL(KUidMsgTypeSMTP, _L("SMTP"), KSmtpServerMtmFileName(), TMsvTestDllInfo(KSmtpClientMtmFileName,2), KSmtpDataLayerFileName(), KSmtpDataLayerFileName(), KUidMsvTechnologyGroupEmail, smtpDatFile);
	}

EXPORT_C void CEmailTestUtils::CreatePopServerMtmRegL()
	{
	CreateServerMtmRegL(KUidMsgTypePOP3, _L("POP3"), KPop3ServerMtmFileName(), KPop3ClientMtmFileName(), KPop3DataLayerFileName(), KPop3DataLayerFileName(), KUidMsvTechnologyGroupEmail, pop3DatFile);
	}

EXPORT_C TMsvId CEmailTestUtils::CreateImapServiceL()
	{
	TInt err = KErrNone;
	iImapServiceId = 0;
	TRAP(err, ServiceIdL(KUidMsgTypeIMAP4, iImapServiceId));

	if (err)
		{
		CEmailAccounts* account = CEmailAccounts::NewLC();

		CImImap4Settings* imapSettings = new(ELeave) CImImap4Settings();
		CleanupStack::PushL(imapSettings);

		CImIAPPreferences* imapIAP = CImIAPPreferences::NewLC();
				
		account->PopulateDefaultImapSettingsL(*imapSettings, *imapIAP);

		_LIT(KImapServer, "imap.dial.pipex.com");
		imapSettings->SetServerAddressL(KImapServer);
		_LIT8(KImapLoginName, "abf75");
		imapSettings->SetLoginNameL(KImapLoginName);
		_LIT8(KImapPassword, "oodifama");
		imapSettings->SetPasswordL(KImapPassword);
		imapSettings->SetPort(143);
		
		_LIT(KTxtAccountName, "ImapAccount");
		TImapAccount imapAccount = account->CreateImapAccountL(KTxtAccountName, *imapSettings, *imapIAP, EFalse);

		CleanupStack::PopAndDestroy(3, account); // imapIAP, imapSettings, account

		ServiceIdL(KUidMsgTypeIMAP4, iImapServiceId);
		}
	else
		{
		Printf(_L("Imap Service Already Exists. You should delete the existing service first!\n"));
		}


	return iImapServiceId;
	}

EXPORT_C TMsvId CEmailTestUtils::CreateSmtpServiceL()
	{
	TInt err = KErrNone;
	iSmtpServiceId = 0;
	TRAP(err, ServiceIdL(KUidMsgTypeSMTP, iSmtpServiceId));

	if (err)
		{
		CEmailAccounts* account = CEmailAccounts::NewLC();

		CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
		CleanupStack::PushL(smtpSettings);
		
		CImIAPPreferences* smtpIAP = CImIAPPreferences::NewLC();
				
		account->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpIAP);

		_LIT(KSmtpServer, "smtp.dial.pipex.com");
		smtpSettings->SetServerAddressL(KSmtpServer);
		smtpSettings->SetEmailAliasL(_L("Messaging Test1"));
		_LIT(KSmtpEmailAddress, "msgtest1@internet-test.plc.psion.com");
		smtpSettings->SetEmailAddressL(KSmtpEmailAddress);
		smtpSettings->SetReplyToAddressL(KSmtpEmailAddress);
		smtpSettings->SetBodyEncoding(EMsgOutboxMIME);
		smtpSettings->SetReceiptAddressL(KSmtpEmailAddress);
		smtpSettings->SetPort(25);
		
		_LIT(KTxtAccountName, "SmtpAccount");
		TSmtpAccount smtpAccount = account->CreateSmtpAccountL(KTxtAccountName, *smtpSettings, *smtpIAP, EFalse);

		CleanupStack::PopAndDestroy(3, account); // smtpIAP, smtpSettings, account

		ServiceIdL(KUidMsgTypeSMTP, iSmtpServiceId);
		}
	else
		{
		Printf(_L("Smtp Service Already Exists\n"));	
		}

	return iSmtpServiceId;
	}

EXPORT_C TMsvId CEmailTestUtils::CreatePopServiceL()
	{
	TInt err = KErrNone;
	iPopServiceId = 0;
	TRAP(err, ServiceIdL(KUidMsgTypePOP3, iPopServiceId));

	if (err)
		{
		CEmailAccounts* account = CEmailAccounts::NewLC();

		CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
		CleanupStack::PushL(popSettings);

		CImIAPPreferences* popIAP = CImIAPPreferences::NewLC();
				
		account->PopulateDefaultPopSettingsL(*popSettings, *popIAP);
		
		_LIT(KTxtAccountName, "PopAccount");
		TPopAccount popAccount = account->CreatePopAccountL(KTxtAccountName, *popSettings, *popIAP, EFalse);

		CleanupStack::PopAndDestroy(3, account); // popIAP,popSettings, account

		ServiceIdL(KUidMsgTypePOP3, iPopServiceId);
		}
	else
		{
		Printf(_L("Pop Service Already Exists. You should delete the existing service first!\n"));
		}


	return iPopServiceId;
	}

EXPORT_C void CEmailTestUtils::DeleteImapServiceL()
	{
	DeleteServiceL(KUidMsgTypeIMAP4);
	}

EXPORT_C void CEmailTestUtils::DeleteSmtpServiceL()
	{
	DeleteServiceL(KUidMsgTypeSMTP);
	}

EXPORT_C void CEmailTestUtils::DeleteScriptFilesFromLogsDirL()
	{
	// Deletes any *.scr file from C:\logs\email directory 
	// to enable dialup connection

	TBuf<5> buf(_L("*.scr"));
	TFileName deleteFile; 
	deleteFile.Append(KEmailDataDir);
	deleteFile.Append(buf);
	
	TFindFile finder(iFs);
	CDir* dirs = NULL;
	TInt err = finder.FindWildByDir(deleteFile, KEmailDataDir, dirs);

	__ASSERT_ALWAYS((err == KErrNone) || (err == KErrNotFound), Panic(KErrGeneral));
	
	CFileMan* fileMan = CFileMan::NewL(iFs);
	CleanupStack::PushL(fileMan);
	fileMan->Delete(deleteFile);
	
	CleanupStack::PopAndDestroy(); // fileMan
	delete dirs;
	}

EXPORT_C void CEmailTestUtils::DeletePopServiceL()
	{
	DeleteServiceL(KUidMsgTypePOP3);
	}

EXPORT_C TMsvId CEmailTestUtils::FindImapServiceL()
	{
	TMsvId firstId = 0;
	ServiceIdL(KUidMsgTypeIMAP4, firstId);
	return firstId;
	}

EXPORT_C TMsvId CEmailTestUtils::FindSmtpServiceL()
	{
	TMsvId firstId = 0;
	ServiceIdL(KUidMsgTypeSMTP, firstId);
	return firstId;
	}

EXPORT_C TMsvId CEmailTestUtils::FindPopServiceL()
	{
	TMsvId firstId = 0;
	ServiceIdL(KUidMsgTypePOP3, firstId);
	return firstId;
	}

EXPORT_C void CEmailTestUtils::Reset()
	{
	DeleteCurrentMtm();

	CMsvTestUtils::Reset();
	}

EXPORT_C void CEmailTestUtils::ClearEmailAccountsL()
	{
	TBool isServerSide = EFalse;
	if( iClientServer == EServerSide )
		{
		isServerSide = ETrue;
		GoClientSideL();
		}

	CRepository* popRepository = CRepository::NewL(KUidMsgTypePOP3);
	popRepository->Reset();
	delete popRepository;
	CRepository* imapRepository = CRepository::NewL(KUidMsgTypeIMAP4);
	imapRepository->Reset();
	delete imapRepository;
	CRepository* smtpRepository = CRepository::NewL(KUidMsgTypeSMTP);
	smtpRepository->Reset();
	delete smtpRepository;
	
	if( isServerSide )
		{
		GoServerSideL();
		}
	}

void CEmailTestUtils::DeleteCurrentMtm()
	{
	if (iImapClientMtm)
		{
		delete iImapClientMtm;
		iImapClientMtm = NULL;
		}
	if (iPopClientMtm)
		{
		delete iPopClientMtm;
		iPopClientMtm = NULL;
		}
	if (iSmtpClientMtm)
		{
		delete iSmtpClientMtm;
		iSmtpClientMtm = NULL;
		}
	if (iImapServerMtm)
		{
		delete iImapServerMtm;
		iImapServerMtm = NULL;
		iServerEntry=NULL;
		}
	if (iPopServerMtm)
		{
		delete iPopServerMtm;
		iPopServerMtm = NULL;
		iServerEntry=NULL;
		}
	if (iSmtpServerMtm)
		{
		delete iSmtpServerMtm;
		iSmtpServerMtm = NULL;
		iServerEntry=NULL;
		}
	}

EXPORT_C const TDesC8& CEmailTestUtils::MachineName()
    {
    return *iMachineName;
    }
    
EXPORT_C CEmailTestUtils::~CEmailTestUtils()
	{
	Reset();
	delete iMachineName;
	}

CEmailTestUtils::CEmailTestUtils(RTest& aRTest)
: CMsvTestUtils(aRTest), iSmtpServiceId(KMsvNullIndexEntryId)
	{
	StartC32();
	}

void CEmailTestUtils::ConstructL(TUint aCreationFlags)
	{
	CMsvTestUtils::ConstructL(aCreationFlags);
	CMsvTestUtils::SpecifyLogsDir(KEmailDataDir);

    // Read the machine name and assign to iMachineName.	
	RFile file;
	User::LeaveIfError(file.Open(iFs, KComputerNamePath, EFileRead));
	CleanupClosePushL(file);

    TInt  fileSize;
	User::LeaveIfError(file.Size(fileSize));
	iMachineName = HBufC8::NewL(fileSize);
	TPtr8 bufferPtr = iMachineName->Des();
	User::LeaveIfError(file.Read(bufferPtr));

	CleanupStack::PopAndDestroy(&file);
	}

void CEmailTestUtils::Panic(TInt aPanic)
	{
	User::Panic(EmailTEST_PANIC, aPanic);
	}

void CEmailTestUtils::Panic(TEmailTestUtilsPanic aPanic)
	{
	Panic((TInt) aPanic);
	}

EXPORT_C void CEmailTestUtils::InstallSmtpMtmGroupL()
	{
	InstallMtmGroupL(smtpDatFile);
	}

EXPORT_C void CEmailTestUtils::InstallPopMtmGroupL()
	{
	InstallMtmGroupL(pop3DatFile);
	}

EXPORT_C void CEmailTestUtils::InstallImapMtmGroupL()
	{
	InstallMtmGroupL(imap4DatFile);
	}

void CEmailTestUtils::SetFolderType(TMsvEntry& aMsvEntry, TPtrC& type)
	{
	TMsvEmailEntry& entry = (TMsvEmailEntry&)aMsvEntry;

	if(entry.iType == KUidMsvRootEntry)
		type.Set(_L("ROOT"));
	else if(entry.iType == KUidMsvServiceEntry)
		type.Set(_L("SERVICE"));
	else if(entry.iType == KUidMsvFolderEntry)
		{
		if (entry.MessageFolderType() == EFolderTypeAlternative)
			type.Set(_L("FOLDER_ALT"));
		else if (entry.MessageFolderType() == EFolderTypeMixed)
			type.Set(_L("FOLDER_MIX"));
		else if (entry.MessageFolderType() == EFolderTypeRelated)
			type.Set(_L("FOLDER_REL"));
		else if (entry.MessageFolderType() == EFolderTypeRFC822)
			type.Set(_L("FOLDER_RFC"));
		else if (entry.MessageFolderType() == EFolderTypeUnknown)
			type.Set(_L("FOLDER_UNKWN"));
		else if (entry.MessageFolderType() == EFolderTypeParallel)
			type.Set(_L("FOLDER_PLL"));
		else if (entry.MessageFolderType() == EFolderTypeDigest)
			type.Set(_L("FOLDER_DGST"));
		else if (entry.MessageFolderType() == EFolderTypePartial)
			type.Set(_L("FOLDER_PARTL"));
		else if (entry.MessageFolderType() == EFolderTypeDirectory)
			type.Set(_L("FOLDER_DIR"));
		else if (entry.MessageFolderType() == EFolderTypeExternal)
			type.Set(_L("FOLDER_EXT"));
		else
			type.Set(_L("FOLDER"));
		}
	else if(entry.iType == KUidMsvMessageEntry)
		type.Set(_L("MESSAGE"));
	else if(entry.iType == KUidMsvEmailTextEntry)
		type.Set(_L("TEXT"));
	else if(entry.iType == KUidMsvEmailHtmlEntry)
		type.Set(_L("HTML"));
	else if(entry.iType == KUidMsvAttachmentEntry)
		type.Set(_L("ATTACHMENT"));
	else
		type.Set(_L("UNKNOWN"));
	}

EXPORT_C void CEmailTestUtils::ListChildrenL(RFile& aFile, CDir& rfcFileList, TInt& aCount, TInt& aEntryCount, TBool aReplace, TBool aOtherFiles)
	{
	// Check details of attachments on current context entry


	CMsvEntrySelection* msvSelection = NULL;
	TMsvEmailEntry entry;

	if (iClientServer==EClientSide)
		{
		entry = (TMsvEmailEntry&) iMsvEntry->Entry(); 
		msvSelection = iMsvEntry->ChildrenL();
		}
	else
		{
		entry = (TMsvEmailEntry&) iServerEntry->Entry(); 
		msvSelection = new (ELeave) CMsvEntrySelection;
		iServerEntry->GetChildren(*msvSelection);
		}
	CleanupStack::PushL(msvSelection);
	
	TInt attachCount = msvSelection->Count(); 	// For each child ..
	TInt bodySize=0;

	TPtrC type;

	SetFolderType(entry, type);

	TPtrC priority;

	TMsvPriority temp=entry.Priority();
	if(temp==EMsvHighPriority)
		priority.Set(_L("High  "));
	else if(temp==EMsvMediumPriority)
		priority.Set(_L("Medium"));
	else if(temp==EMsvLowPriority)
		priority.Set(_L("Low   "));
	else
		priority.Set(_L("None  "));


	TBuf<6> streams=_L("------");

	if ((iClientServer==EClientSide && iMsvEntry->HasStoreL()) ||
		(iClientServer==EServerSide && iServerEntry->HasStoreL()))
		{
		CMsvStore* fileStore;
		fileStore = (iClientServer==EClientSide) ? iMsvEntry->ReadStoreL() : iServerEntry->ReadStoreL();
		
		if(fileStore->IsPresentL(KUidMsgFileIMailHeader))
			{
			if (aOtherFiles)
				DisplayHeaderL(entry.Id(), *fileStore, iLogsDir, aReplace);
			streams.Replace(0, 1, _L("H"));
			}
		if(fileStore->IsPresentL(KUidMsgFileMimeHeader))
			{
			if (aOtherFiles)
				DisplayMimeHeaderL(entry.Id(), *fileStore, iLogsDir, aReplace);
			streams.Replace(1, 1, _L("M"));
			}
		if(fileStore->HasBodyTextL())
			{
			if (aOtherFiles)
				WriteBodyDataL(entry.Id(), iLogsDir, *fileStore, aReplace);
			streams.Replace(2, 1, _L("B"));
			CParaFormatLayer* paraLayer = CParaFormatLayer::NewL();
			CCharFormatLayer* charLayer = CCharFormatLayer::NewL();
			CRichText* body = CRichText::NewL(paraLayer,charLayer);
			fileStore->RestoreBodyTextL(*body);
			bodySize=body->DocumentLength();
			delete paraLayer;
			delete charLayer;
			delete body;
			}
		delete fileStore;
		}

	if((entry.iType==KUidMsvAttachmentEntry || entry.iType==KUidMsvEmailHtmlEntry))
		{
		TFileName filepath;

		TBool storePresent;
		if(iClientServer==EClientSide)
			{
			storePresent=iMsvEntry->HasStoreL();
			}
		else
			{
			storePresent=iServerEntry->HasStoreL();
			}
			
		if(storePresent)
			{			
			CMsvStore* store;
			if (iClientServer==EClientSide)
				{
				store = iMsvEntry->ReadStoreL();
				CleanupStack::PushL(store);
				}
			else
				{
				store = iServerEntry->ReadStoreL();
				CleanupStack::PushL(store);
				}
				
			MMsvAttachmentManager& manager = store->AttachmentManagerL();
			if( manager.AttachmentCount()>0 )
				{
				// there should only be one attachment per entry
				CMsvAttachment* attachmentInfo = manager.GetAttachmentInfoL(0);
				filepath = attachmentInfo->FilePath();
				delete attachmentInfo;
				}
				
			CleanupStack::PopAndDestroy(store);
			}
		
		if(storePresent)
		if (aOtherFiles)
			WriteFileDataL(entry.Id(), filepath, iLogsDir, aReplace);
		}

	if(entry.Attachment())
		streams.Replace(3, 1, _L("A"));
	if(entry.Receipt())
		streams.Replace(4, 1, _L("R"));

	TBuf<16> other=_L("------ ------");
	if(entry.VCalendar())
		other.Replace(7, 2, _L("CAL"));
	if(entry.ICalendar())
		other.Replace(7, 2, _L("ICS"));
	if(entry.VCard())
		other.Replace(10, 2, _L("CAR"));

	if (entry.New())
		other.Replace(0, 1, _L("N"));
	if (entry.Unread())
		other.Replace(1, 1, _L("U"));
	if (entry.Complete())
		other.Replace(2, 1, _L("C"));
	if (entry.BodyTextComplete())
		other.Replace(3, 1, _L("B"));
	if (entry.MHTMLEmail())
		other.Replace(4, 1, _L("H"));
	if (entry.Visible())
		other.Replace(5, 1, _L("V"));

	HBufC* outputL = HBufC::NewLC(600);
	TPtr outputLine = outputL->Des();

	// Put details into output string buffer
	TMsvId current=entry.Id();
	TMsvEntry parent;
	if (entry.Parent() > 0)
		{
		if (iClientServer==EClientSide) 
			{
			iMsvEntry->SetEntryL(entry.Parent());
			parent=iMsvEntry->Entry();
			}
		else
			{
			iServerEntry->SetEntry(entry.Parent());
			parent=iServerEntry->Entry();
			}
		}
	
	if((parent.iType==KUidMsvFolderEntry)&&(parent.iDetails.Compare(_L("Outbox"))>=0)
										&& &rfcFileList && iFileCount < rfcFileList.Count())
		{
		TFileName filename = (rfcFileList)[iFileCount].iName;
		filename.SetLength(8);
		outputLine.Append(filename);
		iFileCount++;
		}
	else
		outputLine.AppendFormat(_L("        "));

	if (iClientServer==EClientSide)
		{
		iMsvEntry->SetEntryL(current);
		entry = iMsvEntry->Entry();
		}
	else
		{
		iServerEntry->SetEntry(current);
		entry = iServerEntry->Entry();
		}
	
	for(TInt i=0; i < aCount; i++)
		outputLine.AppendFormat(_L("  "));
		
	if (entry.iType == KUidMsvEmailTextEntry)
		outputLine.AppendFormat(TRefByValue<const TDesC>_L("%S, 00%x, Children=%d, Size=%d, Store=%S, P=%S, Other=%S, Det:%S, Desc:%S, TxtSize:%d\r\n"),
							&type, 
							entry.Id(),
							attachCount, 
							entry.iSize,
							&streams,
							&priority,
							&other,
							&entry.iDetails,
							&entry.iDescription,
							bodySize);
	else
		outputLine.AppendFormat(TRefByValue<const TDesC>_L("%S, 00%x, Children=%d, Size=%d, Store=%S, P=%S, Other=%S, Det:%S, Desc:%S\r\n"),
							&type, 
							entry.Id(),
							attachCount, 
							entry.iSize,
							&streams,
							&priority,
							&other,
							&entry.iDetails,
							&entry.iDescription);
	
	HBufC8* pOutputLine8 = HBufC8::NewLC(outputLine.Length());
	pOutputLine8->Des().Copy(outputLine);
	aFile.Write(pOutputLine8->Des());
	CleanupStack::PopAndDestroy(); // pBuf16

	for(TInt j=0; j<attachCount; j++)
		{
		// Get Context of current message
		if (iClientServer==EClientSide) 
			iMsvEntry->SetEntryL((*msvSelection)[j]);
		else
			iServerEntry->SetEntry((*msvSelection)[j]);
		
		aEntryCount++;
		aCount++;
		ListChildrenL(aFile, rfcFileList, aCount, aEntryCount, EFalse, aOtherFiles);
		aCount--;
		}	

	CleanupStack::PopAndDestroy(2); // msvSelection, outputL
	}

void CEmailTestUtils::DisplayMimeHeaderL(TMsvId aId, CMsvStore& fileStore, const TFileName& aFilepath, TBool aReplace)
	{
	TFileName filename(aFilepath);
	filename.Append(KFileNameMimeHeaders);

	RFile file;

	TInt err = KErrNone;
	if (aReplace)
		err = file.Replace(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	else
		err = file.Open(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	if(err==KErrNotFound) // file does not exist - create it
		err=file.Create(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	
	TInt offset=0;
	iRTest(file.Seek(ESeekEnd, offset)==KErrNone);

	TBuf8<600> buf;
	buf.Zero();
	buf.AppendFormat(_L8("*** 00%x ************* Mime Header ******************\n"), aId);

	CImMimeHeader* mimeHeader = CImMimeHeader::NewL();
	mimeHeader->RestoreL(fileStore);
	
	TBuf<300> buf2;
	
	buf.AppendFormat(_L8("Version: %d\r\n"),mimeHeader->Version());
	file.Write(buf);

	buf.Zero();
	buf.AppendFormat(_L8("MimeCharset: %d\r\n"),mimeHeader->MimeCharset());
	file.Write(buf);
	if(mimeHeader->ContentType().Length())
		{
		buf2.Copy(mimeHeader->ContentType());
		DoBuf(buf, _L("Content-Type: "), buf2);
		}
	else buf.Zero();
	file.Write(buf);

	if(mimeHeader->ContentSubType().Length())
		{
		buf2.Copy(mimeHeader->ContentSubType());
		DoBuf(buf, _L("Content-Type,SubType: "), buf2);
		}
	else buf.Zero(); 
	file.Write(buf);

	if(mimeHeader->ContentDisposition().Length())
		{
		buf2.Copy(mimeHeader->ContentDisposition());
		DoBuf(buf, _L("Content-Disposition: "), buf2);
		}
	else buf.Zero();
	file.Write(buf);

	if(mimeHeader->ContentDescription().Length())
		{
		buf2.Copy(mimeHeader->ContentDescription());
		DoBuf(buf, _L("Content-Description: "), buf2);
		}
	else buf.Zero();
	file.Write(buf);

	if(mimeHeader->ContentBase().Length())
		{
		buf2.Copy(mimeHeader->ContentBase());
		DoBuf(buf, _L("Content-Base: "), buf2);
		}
	else buf.Zero();
	file.Write(buf);

	if(mimeHeader->ContentID().Length())
		{
		buf2.Copy(mimeHeader->ContentID());
		DoBuf(buf, _L("Content-ID: "), buf2);
		}
	else buf.Zero();
	file.Write(buf);

	if(mimeHeader->ContentLocation().Length())
		{
		buf2.Copy(mimeHeader->ContentLocation());
		DoBuf(buf, _L("Content-Location: "), buf2);
		}
	else buf.Zero();
	file.Write(buf);

	if(mimeHeader->ContentTransferEncoding())
		{
		buf.AppendFormat(_L8("Content-Transfer-Encoding: %d\r\n"),mimeHeader->ContentTransferEncoding());
		file.Write(buf);
		}

	PrintRecipients( mimeHeader->ContentTypeParams(), _L("ContentTypeParams:"), file);
	PrintRecipients( mimeHeader->ContentDispositionParams(), _L("ContentDispositionParams:"), file);

	file.Write(_L8("\n******************** end of MIME Header ****************\n"));
	file.Close();
	delete mimeHeader;
	}

void CEmailTestUtils::PrintRecipients(CDesC8Array& list, const TDesC& label, RFile file)
	{
	TBuf8<300> buf;
	
	for(int i=0; i < list.Count(); i++)
		{
		buf.Zero();
		buf.Copy(label); 
		buf.Append(list[i]);
		buf.Append(_L("\r\n"));
		file.Write(buf);
		}
	}

void CEmailTestUtils::PrintRecipientsL(CDesC16Array& list, const TDesC& label, RFile file)
	{
	TBuf8<300> buf;
	
	for(int i=0; i < list.Count(); i++)
		{
		buf.Zero();
		buf.Copy(label); 

		HBufC8* liststr=HBufC8::NewLC(list[i].Length());
		liststr->Des().Copy(list[i]);

		buf.Append(*liststr);
		buf.Append(_L("\r\n"));
		file.Write(buf);

		CleanupStack::PopAndDestroy();
		}
	}


void CEmailTestUtils::DisplayHeaderL(TMsvId aId, CMsvStore& fileStore, const TFileName& aFilepath, TBool aReplace)
	{
	TFileName filename(aFilepath);
	filename.Append(KFileNameHeaders);

	RFile file;
	
	TInt err = KErrNone;
	
	if (aReplace)
		err = file.Replace(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	else
		err = file.Open(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	if(err==KErrNotFound) // file does not exist - create it
		err=file.Create(iFs, filename, EFileShareAny | EFileStreamText | EFileWrite);
	
	TInt offset=0;
	iRTest(file.Seek(ESeekEnd, offset)==KErrNone);
	
	TBuf8<300> buf;
	buf.Zero();
	buf.AppendFormat(_L8("*** 00%x ************* Rfc822 Header ****************\n"), aId);
	file.Write(buf);

	CImHeader* header = CImHeader::NewLC();	// PushL(header)
	CleanupStack::Pop();					// header
	
	TUint emptySize = header->DataSize();
	header->RestoreL(fileStore);

	TUint size = header->DataSize()-emptySize;
	buf.Zero();
	buf.AppendFormat(_L8("Size >>> %d\n"), size);
	file.Write(buf);

	if(header->Subject().Length())
		DoBuf(buf, _L("Subject: "), header->Subject());
	else buf.Zero();
	file.Write(buf);

	if(header->From().Length())
		DoBuf(buf, _L("From: "), header->From());
	else buf.Zero();
	file.Write(buf);

	if(header->ReplyTo().Length())
		DoBuf(buf, _L("Return-Path: "), header->ReplyTo());
	else buf.Zero();
	file.Write(buf);

	if(header->ImMsgId().Length())
		DoBuf(buf, _L("ID: "), header->ImMsgId());
	else buf.Zero();
	file.Write(buf);

	if(header->ReceiptAddress().Length())
		DoBuf(buf, _L("Return-Receipt-To: "), header->ReceiptAddress());
	else buf.Zero();
	file.Write(buf);

	PrintRecipientsL(header->ToRecipients(), _L("To:"), file);
	PrintRecipientsL(header->CcRecipients(), _L("Cc:"), file);
	PrintRecipientsL(header->BccRecipients(), _L("Bcc:"), file);
	
	file.Write(_L8("\n******************** end of RFC822 header **************\n"));
	file.Close();
	delete header;
	}


EXPORT_C void CEmailTestUtils::CopyScriptFileL(const TDesC& aSourceFile, const TDesC& aDestPath, const TDesC& aPortNumber)
	{
	TFileName destPath = aDestPath;
	// Make the destination file name.
	destPath.Append(_L("imsk"));
	destPath.Append(aPortNumber);
	destPath.Append(_L(".scr"));

	// Delete the existing script file if it is there.
	TInt err = iFs.Delete(destPath);
	__ASSERT_ALWAYS((err == KErrNone) || (err == KErrNotFound), Panic(KErrGeneral));

	// Copy the file.
	CFileMan* fileMan = CFileMan::NewL(iFs);
	CleanupStack::PushL(fileMan);
	User::LeaveIfError(fileMan->Copy(aSourceFile, destPath));
	CleanupStack::PopAndDestroy(); // fileMan
	}

EXPORT_C void CEmailTestUtils::CopyScriptFileL(const TDesC& aSourceFile, const TDesC& aPortNumber)
	{
	CopyScriptFileL(aSourceFile, _L("c:\\logs\\email\\"), aPortNumber);
	}


EXPORT_C void CEmailTestUtils::CreateMessageFilesL(TMsvId aFinalServiceId, TMsvId aCreateInFolderId)
	{
	CreateMessageFilesL(aFinalServiceId,aCreateInFolderId,KRfc822Dir);
	}

EXPORT_C void CEmailTestUtils::CreateMessageFilesL(TMsvId aFinalServiceId, TMsvId aCreateInFolderId,const TDesC& aPath)
	{
	iServerEntry->SetEntry(aFinalServiceId);
	TInt counter=0;
	TInt numberOfFiles=0;

	// Call this function to convert each text file found in \rfc822\ directory into
	// message files in \mail\ directory

	iFs.SetSessionPath(aPath);

	iRTest.Printf(_L("Generating files :"));

	CDir* rfcFileList;
	// Loads the any test files into an EntryList
	TInt error=iFs.GetDir(_L("RFC822*.TXT"), KEntryAttNormal, ESortByName, rfcFileList);
	if(!error)
		{
		numberOfFiles = rfcFileList->Count();
		if(numberOfFiles)
			{
			do	{
				iRTest.Printf(_L("."));
				TFileName filename = (*rfcFileList)[counter].iName;
				CreateMessageL(filename, aFinalServiceId, aCreateInFolderId);
				counter++;
				}
			while(counter<numberOfFiles);
			}
		else
			iRTest.Printf(_L("WARNING - There are no RFC files in test file source directory\n"));
		}
	else
		iRTest.Printf(TRefByValue<const TDesC>_L("\r\nFile error %d"), error);

	delete rfcFileList;
	}

EXPORT_C void CEmailTestUtils::CreateMessageL(const TDesC& filename, TMsvId aFinalServiceId, TMsvId aCreateInFolderId)
	{
	User::LeaveIfError(iServerEntry->SetEntry(aFinalServiceId));
	CImRecvConvert* recvConvert = CImRecvConvert::NewLC(iFs, iServerEntry, iServerEntry->Entry().iMtm, aFinalServiceId);
	recvConvert->SetMsvId(aCreateInFolderId); //Set the TMsvServerEntry to correct context 

	TRAPD(error,ReadDataL(recvConvert, filename)); // Do the stuff
	CleanupStack::PopAndDestroy();  //recvConvert
	}

void CEmailTestUtils::ReadDataL(CImRecvConvert* recvConvert, TFileName fileName)
	{
	// open the file
	RFile file;

	TInt anError = file.Open(iFs, fileName, EFileShareAny);
	if(anError != KErrNone)
		{
		iRTest.Printf(TRefByValue<const TDesC>_L("\r\nFile open error %d"), anError);
		iRTest.Getch();
		User::Leave(KErrNotFound);
		}

	// read the file into the conversion object
	TBuf8<1024> line;
	TBuf8<1> aChar;
	TBool finished = FALSE;

	recvConvert->ResetL();
	// supply a new attachment path since previous one is discarded by call to Reset()

	do {
		line.FillZ();
		line.SetLength(0);
		// compile a line one char at a time
		do {
			file.Read(aChar, 1);
			if(aChar.Length())
				line.Append(aChar);
			else
				finished = TRUE;
			// stop at the end of line or no more data
			} while(aChar.Length() && aChar[0] != 0x0A);
			if(!line.Length())
				break;
			/***********************************************/
			/* -- This is where we actually do the work -- */
			/***********************************************/
			recvConvert->ParseNextField(line); // This function doesnt actually leave, any leaves are relayed to the MessageCompleteL() function
		} while(!finished);

	TRAPD(err, recvConvert->MessageCompleteL());
	if(err!=KErrNone)
		{
		// At this point the message should be deleted and CImRecvConvert should be Reset()
		iRTest.Printf(TRefByValue<const TDesC>_L("\r\nError %d occured during the conversion of Message %d"), err, recvConvert->EntryId());
		iRTest.Getch();
		}
	
	file.Close();
	}


EXPORT_C void CEmailTestUtils::CreateAllTestDirectories()
	{
	CMsvTestUtils::CreateAllTestDirectories();

	iFs.SetSessionPath(_L("c:\\"));

	if (!iFs.MkDir(KMailtestPath))
		{
		Printf(_L("Created c:\\mailtest\\ directory\n"));
		}

	TUint AttValue;

	if (iFs.Att(KImcmResourceLocation, AttValue))
		{
		if (iFs.Att(_L("z:\\resource\\messaging\\imcm.rsc"),AttValue))
			{
			Printf(_L("WARNING - IMCM.RSC cannot be found\n"));
			}			
		}

	if (!iFs.MkDir(KEmailDataDir))
		{
		Printf(_L("Created c:\\logs\\email directory\n"));
		}
	
	if (!iFs.MkDir(KRfc822Dir))
		{
		Printf(_L("Created empty c:\\mailtest\\rfc822 directory. Nb. No messages to send\n"));
		Printf(_L("Put RFC822*.TXT files into c:\\mailtest\\rfc822 directory to run test harness\n"));
		}
	}


EXPORT_C void CEmailTestUtils::InstantiateClientMtmsL()
	{
	InstantiateImapClientMtmL();
	InstantiatePopClientMtmL();
	InstantiateSmtpClientMtmL();
	}

EXPORT_C void CEmailTestUtils::InstantiateServerMtmsL()
	{
	InstantiateImapServerMtmL();
	InstantiatePopServerMtmL();
	InstantiateSmtpServerMtmL();
	}

EXPORT_C void CEmailTestUtils::DeleteServicesL()
	{
	DeleteImapServiceL();
	DeleteSmtpServiceL();
	DeletePopServiceL();
	}

EXPORT_C void CEmailTestUtils::CreateServicesL()
	{
	CreateImapServiceL();
	CreatePopServiceL();
	CreateSmtpServiceL();	
	}

EXPORT_C void CEmailTestUtils::FindExistingServicesL()
	{
	TRAPD(err, iImapServiceId = FindImapServiceL());

	if (err)
		Printf(_L("Imap Service Not Found\n"));

	TRAP(err, iSmtpServiceId = FindSmtpServiceL());

	if (err)
		Printf(_L("Smtp Service Not Found\n"));

	TRAP(err, iPopServiceId = FindPopServiceL());

	if (err)
		Printf(_L("Pop Service Not Found\n"));
	}

EXPORT_C void CEmailTestUtils::InstallMtmGroupsL()
	{
	InstallSmtpMtmGroupL();
	InstallPopMtmGroupL();
	InstallImapMtmGroupL();
	}

EXPORT_C void CEmailTestUtils::CreateServerMtmRegsL()
	{
	CreateImapServerMtmRegL();
	CreateSmtpServerMtmRegL();
	CreatePopServerMtmRegL();
	}


EXPORT_C void CEmailTestUtils::StartConnectionL(TUint32 aIAP)
	{
	RSocketServ serv;
	User::LeaveIfError(serv.Connect());
	RConnection conn;
	User::LeaveIfError(conn.Open(serv));
	TCommDbConnPref prefs;
	prefs.SetDirection(ECommDbConnectionDirectionOutgoing);
	prefs.SetBearerSet(KCommDbBearerWcdma | KCommDbBearerCSD);
	prefs.SetIapId(aIAP);
	prefs.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
	TRequestStatus status;
	conn.Start(prefs, status);
	User::WaitForRequest(status);
	
	if (status.Int() != KErrNone && status.Int() != KErrAlreadyExists && status.Int() != KErrInUse)
		User::Leave(status.Int());
	}

