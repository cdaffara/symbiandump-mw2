// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "commands.h"
#include <biodb.h>
#include <bitsids.h>
#include <biouids.h>
#include <miutset.h>
#include <pop3set.h>
#include <imapset.h>
#include <smtpset.h>
#include <smutset.h>
#include <mtmdef.h>
#include <regpsdll.h>

#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>
#include <cemailaccounts.h>

#ifdef SYMBIAN_BOOKMARK_DATABASE
#include <bookmarkdatabase.h>
#include <bookmark.h>
_LIT(KTxtBookmarkExtension, ".ebm");
#endif // SYMBIAN_BOOKMARK_DATABASE

#include <commsdat.h>
#include <utf.h>
#include <cdblen.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "tmsvbioinfo.h"
#endif

//
// CDoCmdGeneral
// Handles the running of the genral commands

//
// For ECmdGeneralComment
CDoCmdGeneral* CDoCmdGeneral::NewL(CClientTestHarness& aTestHarness, const CDesCArrayFlat& aComment, TCmdGeneral aCmd)
{
	CDoCmdGeneral* self = NULL;

	switch(aCmd)
	{
		case(ECmdGeneralComment):
		{
			self = new (ELeave) CDoCmdGeneral(aTestHarness, ECmdGeneralComment);
			CleanupStack::PushL(self);
			self->ConstructL(aComment);
			CleanupStack::Pop(self);
		}
		break;

		case(ECmdGeneralCheckUsedBif):
		{
			self = new (ELeave) CDoCmdGeneral(aTestHarness, ECmdGeneralCheckUsedBif);
			CleanupStack::PushL(self);
			self->ConstructL(aComment);
			CleanupStack::Pop(self);
		}
		break;

		case(ECmdGeneralCheckFileWithExtExists):
		{
			self = new (ELeave) CDoCmdGeneral(aTestHarness, ECmdGeneralCheckFileWithExtExists);
			
		}
		break;

		case(ECmdGeneralPrettifyLog):
		{
			self = new (ELeave) CDoCmdGeneral(aTestHarness, ECmdGeneralPrettifyLog);
		}
		break;

		case(ECmdGeneralDumpMsg):
		{
			self = new (ELeave) CDoCmdGeneral(aTestHarness, ECmdGeneralDumpMsg);
		}
		break;

		case(ECmdGeneralGenerateFromFile):
		{
			self = new (ELeave) CDoCmdGeneral(aTestHarness, ECmdGeneralGenerateFromFile);
			CleanupStack::PushL(self);
			self->ConstructL(aComment);
			CleanupStack::Pop(self);
		}
		break;

		default:
			User::Leave(KErrNotFound);
	}
	
	return self;
}

//
// For ECmdGeneralComment
void CDoCmdGeneral::ConstructL(const CDesCArrayFlat& aComment)
{
	switch(iCmd)
	{
		case(ECmdGeneralComment):
		{
			TInt commentSize = 0;
			for(TInt i = 0; i < aComment.Count(); i++)
				commentSize += ((aComment[i]).Length() + _L(" ").Size());

			iComment = HBufC::NewL(commentSize);
			TPtr commentPtr = iComment->Des();

			for(TInt j = 0; j < aComment.Count(); j++)
			{
				commentPtr.Append((aComment)[j]);
				commentPtr.Append(_L(" "));
			}
		}
		break;

		case(ECmdGeneralGenerateFromFile):
		case(ECmdGeneralCheckUsedBif):
		{
			TBuf<KMaxFileName> mFilename;
			mFilename = aComment[0];
			iComment=mFilename.AllocL();
		}
		break;

		default:
			break;
	}
}

//
// For ECmdGenerateFromSection
CDoCmdGeneral* CDoCmdGeneral::NewL(CClientTestHarness& aTestHarness, const HBufC*& aBuffer, const TBIOMessageType& aMsgType, TCmdGeneral aCmd)
{
	CDoCmdGeneral* self = new (ELeave) CDoCmdGeneral(aTestHarness, aCmd);
	CleanupStack::PushL(self);
	self->ConstructL(aBuffer, aMsgType);
	CleanupStack::Pop(self);
	return self;
}

void CDoCmdGeneral::ConstructL(const HBufC*& aBuffer, const TBIOMessageType& aMsgType)
{
	//
	// Must make local copy of buffer;
	iBuffer = HBufC::NewMaxL(aBuffer->Length());
	*iBuffer = *aBuffer;
	iMsgType = aMsgType;
}

//
// For others
CDoCmdGeneral* CDoCmdGeneral::NewL(CClientTestHarness& aTestHarness, TCmdGeneral aCmd)
{
	CDoCmdGeneral* self = new (ELeave) CDoCmdGeneral(aTestHarness, aCmd);
	return self;
}

void CDoCmdGeneral::StartL(TRequestStatus& aStatus)
{
	switch(iCmd)
	{
		case ECmdGeneralPrettifyLog:
			PrettifyLogL();
			break;
		case ECmdGeneralDumpMsg:
			DumpCurrentMessageL();
			break;
		case ECmdGeneralClean:
			{
			iTestHarness.TestUtils().CleanMessageFolderL();
			}
			break;
		case ECmdGeneralComment:
		{
			//
			// Now we have to add the debugging line number which we only know at run time...
			TBuf<7> commentString;
			commentString.Format(_L("[%4d] "), iDebugInfo.LineNumber());

			//
			// Create a pointer to the HBufC, realloc size and then insert new data...
			iComment = iComment->ReAllocL(iComment->Size() + commentString.Size());
			TPtr commentPtr = iComment->Des();
			commentPtr.Insert(0, commentString);
			iTestHarness.TestUtils().WriteComment(*iComment);
			break;
		}
		case ECmdGeneralGenerateFromFile:
			{
			GenerateBIOMessageFromFileL(*iComment);
			}
			break;
		case ECmdGeneralGenerateFromSection:
			GenerateBIOMessageFromSectionL();
			break;
		case ECmdGeneralCheckUsedBif:
			CheckBifFileUsedL(*iComment);
			break;
		case ECmdGeneralCheckFileWithExtExists:
			CheckDataFileCreatedL();
			break;
		case ECmdGeneralCleanAllBioMsgs:
			CleanAllBioMsgsL();
			break;

		default:
			User::Leave(KErrNotFound);
	}

	//
	// We leave on error, so always say KErrNone
	TRequestStatus* req = &aStatus;
	User::RequestComplete(req, KErrNone);

}

CDoCmdGeneral::~CDoCmdGeneral()
{
	delete iBuffer;
	delete iComment;
}


void CDoCmdGeneral::PrettifyLogL()
{
	TParse loglocation;
	TFileName logfile;
	User::LeaveIfError(iTestHarness.TestUtils().ResolveLogFile(KNullDesC, loglocation));
	logfile.Copy(loglocation.FullName());
	logfile.Delete(logfile.Length()-1,1);
	iTestHarness.TestUtils().DoAppendVariantName(logfile);

	RFile Lfile;
	Lfile.Open(g_rfs,logfile,EFileRead|EFileShareAny);

	_LIT(KHTMLExtn,".html");
	logfile.Append(KHTMLExtn);
	RFile Hfile;
	User::LeaveIfError(Hfile.Replace(g_rfs,logfile,EFileWrite|EFileShareAny));

	_LIT8(KHTMLHeader,"<HTML><HEAD><TITLE>TBIO_T Test Logfile</TITLE></HEAD><BODY><PRE>\n\r");
	_LIT8(KHTMLFooter,"</PRE></BODY>");
	_LIT8(KHTMLRedLineStart,"<FONT COLOR=red>");
	_LIT8(KHTMLGreenLineStart,"<FONT COLOR=green>");
	_LIT8(KHTMLBlackLineStart,"<FONT COLOR=c0c0c0>");
	_LIT8(KHTMLBlueLineStart,"<FONT COLOR=blue>");
	_LIT8(KHTMLLineEnd,"</FONT><br>");
			
	Hfile.Write(KHTMLHeader);

	TBuf8<258> mReadBuf; //258 = KMaxLogLineLength + '\n\r'
	
	TInt p=0;
	Lfile.Read(mReadBuf);
	while(mReadBuf.Length()>0)
	{
		TInt eol = mReadBuf.Locate('\n');
		p+=eol+1;  //skip the \r
		Lfile.Seek(ESeekStart,p);
		if (eol>0)
		{
			mReadBuf.SetLength(eol); //skip the \n

			_LIT8(KDesOK,"ok");
			_LIT8(KDesError,"error");
			_LIT8(KDesFailed,"failed");
			_LIT8(KDesWarning,"warning");

			TBuf8<20> linestart;
			
			linestart = KHTMLBlackLineStart;		

			if (mReadBuf.FindF(KDesOK())>0)
			{
				linestart=KHTMLGreenLineStart;
			}
			else if (mReadBuf.FindF(KDesError())>0)
			{
				linestart=KHTMLRedLineStart;
			}
			else if (mReadBuf.FindF(KDesFailed())>0)
			{
				linestart=KHTMLRedLineStart;
			}
			else if (mReadBuf.FindF(KDesWarning())>0)
			{
				linestart=KHTMLBlueLineStart;
			}

			Hfile.Write(linestart);
			Hfile.Write(mReadBuf);
			Hfile.Write(KHTMLLineEnd);
		}

		Lfile.Read(mReadBuf);
	}

	Hfile.Write(KHTMLFooter);
	
	Hfile.Close();
	Lfile.Close();
}

void CDoCmdGeneral::CheckBifFileUsedL(TDesC& aExpectedName)
{
	// Checks that the Message Type Uid is correct
	// NB - this will change to check the BIF Name when the
	// base source code has been updated to support it.
	iTestHarness.InstantiateClientMtmL();

	const TMsvId& mMsg = iTestHarness.CurrentBioMsg();

	iTestHarness.TestUtils().iMsvEntry->SetEntryL(mMsg);

	TMsvEntry tm = iTestHarness.TestUtils().iMsvEntry->Entry();
	TInt mBioMsgInt = tm.iBioType;
	TUid mBioMsgUID = TUid::Uid(mBioMsgInt);

	TBuf<32> description;
	TUid mMessageTypeUid;
	TInt index;
	
	CBIODatabase* bioDB = CBIODatabase::NewLC(g_rfs);
	bioDB->GetBioIndexWithMsgIDL(mBioMsgUID, index);

	description.Copy(bioDB->BifReader(index).Description());
	mMessageTypeUid=bioDB->BifReader(index).MessageTypeUid();

	CleanupStack::PopAndDestroy();	// bioDB

	TBuf<32> mUidHexString;
	_LIT(KHexFormatString,"%x");
	mUidHexString.Format(KHexFormatString,mMessageTypeUid);

	if (mUidHexString.CompareF(aExpectedName)!=0)
	{
		_LIT(KTmp,"ERROR - checking BIF MessageTypeUID - expected \"%S\" read \"%S\"");
		HBufC* td = HBufC::NewLC(KTmp().Length()+105);
		td->Des().Format(KTmp,&aExpectedName,&mUidHexString);
		iTestHarness.TestUtils().WriteComment(*td);
		CleanupStack::PopAndDestroy(td);

		User::Leave(KErrGeneral); //Test framework requires us to leave if test fails
	}
	else
	{
		_LIT(KOK,"OK Checked BIF FileTypeUid - correct");
		iTestHarness.TestUtils().WriteComment(KOK);
	}
}

#ifdef SYMBIAN_BOOKMARK_DATABASE
void CDoCmdGeneral::CheckBookmarksDbL() const
	{
	iTestHarness.TestUtils().WriteComment(_L("-- BookmarkDb Details --"));
	RBkDatabase bookmarkDb;
	bookmarkDb.OpenL();
	CleanupClosePushL(bookmarkDb);
	RBkFolder rootFolder = bookmarkDb.OpenRootL();
	CleanupClosePushL(rootFolder);
	RBkNode bookmarkItem;
	TBool foundBookmark = EFalse;
	const TInt bookmarkCount = rootFolder.Count();
	for( TInt ii=0; ii<bookmarkCount; ++ii )
		{
		bookmarkItem = rootFolder.OpenItemL(ii);
		CleanupClosePushL(bookmarkItem);
		if( bookmarkItem.Type() == Bookmark::ETypeBookmark )
			{
			foundBookmark = ETrue;
			Bookmark::TItemId bookmarkId = bookmarkItem.Id();
			CleanupStack::PopAndDestroy(&bookmarkItem);
			
			// bookmark found, display bookmark infomation
			RBkBookmark bookmark = bookmarkDb.OpenBookmarkL(bookmarkId);
			CleanupClosePushL(bookmark);
			TBuf<Bookmark::KMaxUriLength> uri;
			uri.Copy(bookmark.Uri());
			iTestHarness.TestUtils().WriteComment(bookmark.Title());
			iTestHarness.TestUtils().WriteComment(uri);
			CleanupStack::PopAndDestroy(&bookmark);
			iTestHarness.TestUtils().WriteComment(_L("-----"));
			}
		else
			{
			CleanupStack::PopAndDestroy(&bookmarkItem);
			}
		}
		
	CleanupStack::PopAndDestroy(2, &bookmarkDb);	// rootFolder, bookmarkDb
	
	if( !foundBookmark )
		{
		_LIT(KError,"ERROR - No bookmarks found in root folder of BookmarkDb.");
		iTestHarness.TestUtils().WriteComment(KError);
		User::Leave(KErrNotFound);
		}
	}
#endif // SYMBIAN_BOOKMARK_DATABASE

void CDoCmdGeneral::CheckDataFileCreatedL()
{
	iTestHarness.InstantiateClientMtmL();

	const TMsvId& messageId = iTestHarness.CurrentBioMsg();

	iTestHarness.TestUtils().iMsvEntry->SetEntryL(messageId);

	TInt index;

	TMsvEntry tm = iTestHarness.TestUtils().iMsvEntry->Entry();
	TInt mBioMsgInt = tm.iBioType;
	TUid mBioMsgUID = TUid::Uid(mBioMsgInt);
	
	CBIODatabase* bioDB = CBIODatabase::NewLC(g_rfs);
	bioDB->GetBioIndexWithMsgIDL(mBioMsgUID, index);

	TBuf<KMaxFileName> mExtn = bioDB->BifReader(index).FileExtension();

	// The following section is necessary as semi-supported iacp functions
	// (e.g. WWWHotlist) just create a file, but the bif file doesn't have
	// a file extension field set.  DPS
	_LIT(KNullString,"");

	if (mExtn==KNullString)
	{
		_LIT(KNullExtensionWarning,"Warning: file extension read from BIF file is NULL!");
		iTestHarness.TestUtils().WriteComment(KNullExtensionWarning);

		_LIT(KWWWHotlistExtn,".eBM");

		if (bioDB->BifReader(index).MessageTypeUid().iUid==KUidBIOInternetAccessPointMsg)
		{
			mExtn = KWWWHotlistExtn;
			
			_LIT(KNullFixupWarning,"Warning: As MessageTypeUid indicates IACP msg, guessed .eBM");
			iTestHarness.TestUtils().WriteComment(KNullFixupWarning);
		}

		// The same if ture for WAPP bookmarks. See comment above.
		else if (bioDB->BifReader(index).MessageTypeUid().iUid == KUidBIOWAPAccessPointMsg)
		{
			mExtn = KWWWHotlistExtn;
			
			_LIT(KNullFixupWarning,"Warning: As MessageTypeUid indicates WAPP msg, guessed .eBM");
			iTestHarness.TestUtils().WriteComment(KNullFixupWarning);
		}
	}

	CleanupStack::PopAndDestroy(bioDB);

	// a <message id>.ebm attachment should exist
	CMsvStore* store = iTestHarness.TestUtils().iMsvEntry->ReadStoreL();
	CleanupStack::PushL(store);
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	TBool found = EFalse;
	TBuf<KMaxFileName> ebmFilename;
	ebmFilename.Format(_L("%x%S"), messageId,&mExtn);
	for( TInt ii=0; ii<manager.AttachmentCount(); ++ii )
		{
		CMsvAttachment* attachment = manager.GetAttachmentInfoL(ii);
		if( attachment->AttachmentName().CompareF(ebmFilename) == 0 )
			{
			found = ETrue;
			}
		delete attachment;
		
		if( found )
			{
			break;
			}
		}
	CleanupStack::PopAndDestroy(store);
	
	TBuf<KMaxFileName+40> mOut;
	if( found )
		{
		_LIT(KOK,"OK File Exists (%S)");
		mOut.Format(KOK, &ebmFilename);
		iTestHarness.TestUtils().WriteComment(mOut);
		
#ifdef SYMBIAN_BOOKMARK_DATABASE
		// File is found, ensure that bookmarks exists is database if it
		// is a bookmark file
		if( mExtn.CompareF(KTxtBookmarkExtension) == 0 )
			{
			CheckBookmarksDbL();
			}
#endif // SYMBIAN_BOOKMARK_DATABASE		
		}
	else
		{
		_LIT(KError,"ERROR: File [%S] not found!");
		mOut.Format(KError, &ebmFilename);
		iTestHarness.TestUtils().WriteComment(mOut);

		User::Leave(KErrNotFound);
		}

}


void CDoCmdGeneral::DumpCurrentMessageL()
{
	_LIT(KAboutToDump,"Dump of Parsed Fields:");
	iTestHarness.TestUtils().WriteComment(KAboutToDump);

	iTestHarness.InstantiateClientMtmL();

	const TMsvId& messageId = iTestHarness.CurrentBioMsg();
	iTestHarness.TestUtils().iMsvEntry->SetEntryL(messageId);

	CMsvStore* mStore = iTestHarness.TestUtils().iMsvEntry->ReadStoreL();
	CleanupStack::PushL(mStore);

	RMsvReadStream msgInStream;
	msgInStream.OpenLC(*mStore, KUidMsvBIODataStream);
	
	CParsedField* parsedField = new(ELeave) CParsedField();
	CleanupStack::PushL(parsedField);
	
	TInt numOfFields = msgInStream.ReadUint8L();
	
	for (TInt i = 0; i < numOfFields; i++)
		{
		parsedField->InternalizeL(msgInStream);

		TPtrC mFieldName = parsedField->FieldName();
		TPtrC mFieldVal = parsedField->FieldValue();

		HBufC* td = HBufC::NewLC(10+mFieldName.Length()+mFieldVal.Length());
				
		_LIT(KTmp,"-- %S:\"%S\"");
		td->Des().Format(KTmp,&mFieldName,&mFieldVal);

		//NB this shouldn't really be needed as msgs should
		//be shorter than 160 chars, but some of the test data
		//aren't!!
		if (td->Length()>220) {td->Des().SetLength(220);}
		

		iTestHarness.TestUtils().WriteComment(*td);

		CleanupStack::PopAndDestroy(td);
		}

	CleanupStack::PopAndDestroy(parsedField);
	CleanupStack::PopAndDestroy();	// msgInStream.OpenLC ??? not sure what got created...
	CleanupStack::PopAndDestroy(mStore);
}


void CDoCmdGeneral::GenerateBIOMessageFromSectionL()
{
	iTestHarness.InstantiateClientMtmL();
	TMsvId newEntryId = iTestHarness.TestUtils().CreateBIOEntryL(*iBuffer, iMsgType);
	_LIT(KMessageGend,"Msg Generated from section with ID: (%d)");

	TBuf<KMaxFileName*2> mComment;
	mComment.Format(KMessageGend, newEntryId);
	iTestHarness.TestUtils().WriteComment(mComment);		

	//
	// Set the context Biomsg to the
	// entry we've just added to the
	// message store
	iTestHarness.SetCurrentBioMsg(newEntryId);				
}


void CDoCmdGeneral::GenerateBIOMessageFromFileL(const TDesC& aFilename)
{
	//
	// Generates the BIO message we store as a member variable from a
	// file.  Initially assumes file name only given (looks in default
	// BIO msg directory).  If not found tries to find file based on
	// aFilename being a fully qualified filename incl. path.
	//
	//
 	TBuf<KMaxFileName> currentFile;
	TMsvId messageId;
	TBIOMessageType currentMsgType;

	currentFile = KBioTMsgDirectory;
	currentFile.Append(aFilename);

	TEntry mEntry;

	//
	// Check that file exists
	TInt err=g_rfs.Entry(currentFile,mEntry);

	//
	// If not, try the alternate path
	if(err != KErrNone)
	{
		currentFile = aFilename;
		TInt err=g_rfs.Entry(currentFile,mEntry);
		if (err!=KErrNone) User::Leave(err); //Leave (not found)
	}
	
	//
	// Set up for server side testing	
//	iTestHarness.InstantiateServerMtmL();
	iTestHarness.InstantiateClientMtmL();

	//
	// Pull the file name off
	TInt nPos = currentFile.LocateReverse('\\');
	TPtrC current(currentFile.Right(currentFile.Length() - nPos - 1));

	//
	// Get utils to set type based on file name
	currentMsgType = iTestHarness.TestUtils().SetMessageType(current);

	if(currentMsgType!=ENoMessage) // skip dodgy filenames
	{ 
		messageId = iTestHarness.TestUtils().CreateBIOEntryFromFileL( currentFile,currentMsgType);

		_LIT(KMessageGend,"Msg Generated from [%S] with ID: (%d)");

		TBuf<KMaxFileName*2> mComment;
		mComment.Format(KMessageGend,&aFilename, messageId);
		iTestHarness.TestUtils().WriteComment(mComment);		

		//
		// Set the context Biomsg to the
		// entry we've just added to the
		// message store
		iTestHarness.SetCurrentBioMsg(messageId);										  
	}
	else
	{
		_LIT(KBadFileName,"Error [%S] isn't a BIO Msg Filename.");
		TBuf<KMaxFileName+40> mComment;
		mComment.Format(KBadFileName,&aFilename);
		iTestHarness.TestUtils().WriteComment(mComment);
		User::Leave(KErrNotFound);
	}
}


void CDoCmdGeneral::CleanAllBioMsgsL()
{
	// Clean the BIO message entires only, marked by iMtmUid == KUidBIOMessageTypeMtm.
	
	// Go client side.
	iTestHarness.InstantiateClientMtmL();

	// Access the context.
	CMsvEntry& entry = *(iTestHarness.TestUtils().iMsvEntry);

	// Change context to root folder.
	entry.SetEntryL(KMsvRootIndexEntryId);

	// Set the msg selection ordering.
	TMsvSelectionOrdering ordering;
	ordering.SetShowInvisibleEntries(ETrue);
	entry.SetSortTypeL(ordering);

	// Delete all email accounts from CenRep
	CEmailAccounts* accounts = CEmailAccounts::NewLC();
	
	// delete POP accounts
	RArray<TPopAccount> popAccounts;
	CleanupClosePushL(popAccounts);
	accounts->GetPopAccountsL(popAccounts);
	TInt count = popAccounts.Count();
	for(TInt ii=0; ii<count; ++ii)
		{
		accounts->DeletePopAccountL(popAccounts[ii]);
		}
	CleanupStack::PopAndDestroy(&popAccounts);
	
	// delete IMAP accounts
	RArray<TImapAccount> imapAccounts;
	CleanupClosePushL(imapAccounts);
	accounts->GetImapAccountsL(imapAccounts);
	count = imapAccounts.Count();
	for(TInt ii=0; ii<count; ++ii)
		{
		accounts->DeleteImapAccountL(imapAccounts[ii]);
		}
	CleanupStack::PopAndDestroy(&imapAccounts);
	
	// delete SMTP accounts
	RArray<TSmtpAccount> smtpAccounts;
	CleanupClosePushL(smtpAccounts);
	accounts->GetSmtpAccountsL(smtpAccounts);
	count = smtpAccounts.Count();
	for(TInt ii=0; ii<count; ++ii)
		{
		accounts->DeleteSmtpAccountL(smtpAccounts[ii]);
		}
	CleanupStack::PopAndDestroy(&smtpAccounts);
	
	CleanupStack::PopAndDestroy(accounts);
	
	entry.SetEntryL(KMsvGlobalInBoxIndexEntryId);
	entry.SetEntryL(KMsvRootIndexEntryId);

	// Check each service.
	CMsvEntrySelection* services = entry.ChildrenWithTypeL(KUidMsvServiceEntry);
	CleanupStack::PushL(services);

	TInt serviceCount = services->Count();
	
	if(serviceCount)
		{
		for(TInt i = 0; i < serviceCount; i++)
			{
			entry.SetEntryL(services->At(i));    

			// BIO services do not contain folders, so can be deleted.
			if (entry.Entry().iMtm == KUidBIOMessageTypeMtm)
				{
				entry.SetEntryL(KMsvRootIndexEntryId);
				entry.DeleteL(services->At(i));
				}

			// Check each folder within other services.
			else
				{
				CMsvEntrySelection* folders = entry.ChildrenWithTypeL(KUidMsvFolderEntry);
				CleanupStack::PushL(folders);

				TInt fldrCount = folders->Count();

				if (fldrCount)
					{
					for (TInt ii = 0; ii < fldrCount; ii++)
						{
						entry.SetEntryL(folders->At(ii));

						// Check each messaging in the folder.
						CMsvEntrySelection* msgs = entry.ChildrenWithTypeL(KUidMsvMessageEntry);
						CleanupStack::PushL(msgs);

						TInt msgCount = msgs->Count();

						if(msgCount)
							{
							for(TInt iii = 0; iii < msgCount; iii++)
								{
								// Delete if BIO message.
								TMsvEntry	msgEntry = entry.ChildDataL(msgs->At(iii));

								if (msgEntry.iMtm == KUidBIOMessageTypeMtm)
									entry.DeleteL(msgs->At(iii));
								}
							}
						CleanupStack::PopAndDestroy(msgs);
						}
					}
				CleanupStack::PopAndDestroy(folders);
				}
			}
		}
	CleanupStack::PopAndDestroy(services);

	// Reset context back to root folder.
	entry.SetEntryL(KMsvRootIndexEntryId);
}

//
// CDoCmdBioDB
// Handles the running of the genral commands

//
// Constants
const TUid KUidBioMsgTypeEmailNotification = {0x10005530};
const TUid KUidBioMsgTypeVCal = {0x10005533};

//
// For others
CDoCmdBioDB* CDoCmdBioDB::NewL(CClientTestHarness& aTestHarness, TCmdBioDB aCmd)
{
	CDoCmdBioDB* self = new (ELeave) CDoCmdBioDB(aTestHarness, aCmd);
	return self;
}

void CDoCmdBioDB::StartL(TRequestStatus& aStatus)
{
	switch(iCmd)
	{
		case ECmdBioDBDump:
			DoBioDBDumpL();
			break;			

		case ECmdBioDBTestAPI:
			DoBioDBTestAPIL();
			break;

		case ECmdBioDBDumpWapBif:
			DoBioDBDumpWapBifsL();
			break;

		case ECmdBioDBCheckBioness:
			DoBioDBCheckBionessL();
			break;

		case ECmdBioDBDefaultSendBearer:
			DoBioDBDefaultSendBearerL();
			break;

		default:
			User::Leave(KErrNotFound);
	}

	//
	// We leave on error, so always say KErrNone
	TRequestStatus* req = &aStatus;
	User::RequestComplete(req, KErrNone);
}

void CDoCmdBioDB::DoBioDBDumpL()
{
	_LIT(KTestStart, "Starting BIODB BIF Dump...");
	iTestHarness.TestUtils().WriteComment(KTestStart);

	HBufC* commentText = HBufC::NewLC(100);
	CBIODatabase* bioDB = CBIODatabase::NewL(g_rfs);
	CleanupStack::PushL(bioDB);

	(commentText->Des()).Format(_L("%D Bif files read\n"), bioDB->BIOCount());
	iTestHarness.TestUtils().WriteComment(*commentText);

	const CArrayFix<TBioMsgId>* ent = NULL;
	
	for (TInt i=0; i < bioDB->BIOCount(); i++)
	{
		const CBioInfoFileReader& bifReader = bioDB->BifReader(i);
		TPtrC desc;
		desc.Set(bifReader.Description());
		(commentText->Des()).Format(_L("%D: '%S'"), i, &desc);
		iTestHarness.TestUtils().WriteComment(*commentText);
		ent = bioDB->BIOEntryLC(i);
		CleanupStack::PopAndDestroy();		//ent
	}
	
	_LIT(KLookForWap, "\r\nLooking for all Wap Ports to Watch\n");
	iTestHarness.TestUtils().WriteComment(KLookForWap);
	TPtrC desc;
	TInt pos;
	ent = bioDB->BioEntryByTypeLC(CBIODatabase::EStart, EBioMsgIdWap, pos);
	
	while(pos < bioDB->BIOCount())
	{
		desc.Set(bioDB->BifReader(pos).Description());
		
		for (TInt i = 0; ent && i < ent->Count(); i++)
		{
			if ((*ent)[i].iType == EBioMsgIdWap)
			{
				(commentText->Des()).Format(_L("%D: '%S' Port#:%D"), i, &desc, (*ent)[i].iPort);
				iTestHarness.TestUtils().WriteComment(*commentText);
			}
		}
		if(ent != NULL)
			CleanupStack::PopAndDestroy();	// ent
		
		ent = bioDB->BioEntryByTypeLC(CBIODatabase::ENext, EBioMsgIdWap, pos);
	}
	if(ent != NULL)
		CleanupStack::PopAndDestroy();	// ent

	_LIT(KLookNBS, "\nLooking for all NBS Ports to Watch\n");
	iTestHarness.TestUtils().WriteComment(KLookNBS);
	
	ent = bioDB->BioEntryByTypeLC(CBIODatabase::EStart, EBioMsgIdNbs, pos);
	
	while(pos < bioDB->BIOCount())
	{
		desc.Set(bioDB->BifReader(pos).Description());
		
		for (TInt i = 0; ent && i < ent->Count(); i++)
		{
			if ((*ent)[i].iType == EBioMsgIdNbs)
			{
				(commentText->Des()).Format(_L("%D: '%S' String:%S"), i, &desc, &((*ent)[i].iText));
				iTestHarness.TestUtils().WriteComment(*commentText);
			}
		}
		if (ent != NULL)
			CleanupStack::PopAndDestroy();	// ent

		ent = bioDB->BioEntryByTypeLC(CBIODatabase::ENext, EBioMsgIdNbs, pos);
	}
	if (ent != NULL)
		CleanupStack::PopAndDestroy();	// ent
	
	CleanupStack::PopAndDestroy(bioDB);
	CleanupStack::PopAndDestroy(commentText);

	_LIT(KTestEnd, "Done BIODB BIF Dump");
	iTestHarness.TestUtils().WriteComment(KTestEnd);
}

void CDoCmdBioDB::DoBioDBTestAPIL()
{
	_LIT(KTestStart, "Starting BIODB TestAPI...");
	iTestHarness.TestUtils().WriteComment(KTestStart);

	TUid msgID;

	CBIODatabase* bioDB = CBIODatabase::NewL(g_rfs);
	CleanupStack::PushL(bioDB);

	while(bioDB->BIOCount())
	{
		bioDB->GetBioMsgID(0,msgID);
		bioDB->RemoveBifL(msgID);
	}

	_LIT(KTestEnd, "Done BIODB TestAPI");
	iTestHarness.TestUtils().WriteComment(KTestEnd);
	CleanupStack::PopAndDestroy(bioDB);
}

void CDoCmdBioDB::DoBioDBDumpWapBifsL()
{
	//
	// Gets a list of port numbers for WAP Port type
	TInt pos;
	HBufC* commentText = HBufC::NewLC(100);
	CBIODatabase* bioDB = CBIODatabase::NewL(g_rfs);
	CleanupStack::PushL( bioDB );
	
	_LIT(KTestStart, "Starting BIODB WAP BIF Dump...");
	iTestHarness.TestUtils().WriteComment(KTestStart);

	const CArrayFix<TBioMsgId>* bioMsgIDs = bioDB->BioEntryByTypeLC(
							CBIODatabase::EStart,
							EBioMsgIdWap, pos);
	while(bioMsgIDs != NULL)
	{
		TUid msgUID;

		bioDB->GetBioMsgID(pos, msgUID);

		_LIT(KMFormatString, "<%S>:\tBioUID %D\tParserName %S\tFile Extension %S");

		TFileName parserName(bioDB->GetBioParserNameL(msgUID));

		TPtrC ext;
		ext.Set(bioDB->GetFileExtL(msgUID));

		TPtrC desc;
		desc.Set(bioDB->BifReader(pos).Description());
		(commentText->Des()).Format(KMFormatString, &desc, msgUID, &parserName, &ext);
		iTestHarness.TestUtils().WriteComment(commentText->Des());

		for (TInt i = 0; i < bioMsgIDs->Count(); i++)
		{
			TBuf<100> bearerString;
			GetBearerText(bioMsgIDs->At(i).iType, bearerString);
			(commentText->Des()).Format(_L("\t\tType:%S\tConf:%D\tIANA:%S\tPort:%D\t"),
					&bearerString,
					bioMsgIDs->At(i).iConfidence,
					&(bioMsgIDs->At(i).iText),
					bioMsgIDs->At(i).iPort);
			
			iTestHarness.TestUtils().WriteComment(commentText->Des());
		}
		
		CleanupStack::PopAndDestroy();	// bioMsgID
		
		bioMsgIDs = bioDB->BioEntryByTypeLC(
							CBIODatabase::ENext,
							EBioMsgIdWap, pos);
	}


	TInt portNumber = 0;
	TRAP_IGNORE(bioDB->GetPortNumberL(KUidBioMsgTypeEmailNotification, EBioMsgIdWap, portNumber));
	

	_LIT(KFormat, "Email Notify is Wap Port %d");
	(commentText->Des()).Format(KFormat,portNumber);
	iTestHarness.TestUtils().WriteComment(commentText->Des());

	TBioMsgIdText ianaString;
	TRAP_IGNORE(bioDB->GetIdentifierTextL(KUidBioMsgTypeEmailNotification, EBioMsgIdIana, ianaString));
	
	_LIT(KFormat2, "Email Notify is IANA String %d");
	(commentText->Des()).Format(KFormat2, &ianaString);
	iTestHarness.TestUtils().WriteComment(commentText->Des());
	
	CleanupStack::PopAndDestroy(bioDB);
	CleanupStack::PopAndDestroy(commentText);

	_LIT(KTestEnd, "Done BIODB WAP BIF Dump");
	iTestHarness.TestUtils().WriteComment(KTestEnd);
}

void CDoCmdBioDB::DoBioDBCheckBionessL()
{
	CBIODatabase* bioDB = CBIODatabase::NewL(g_rfs);
	CleanupStack::PushL(bioDB);

	/*	Not finished, and not used by parser.
		Keeping, unsure of history, GL 29_05_03.
	TBioMsgId bioMsg = iTestHarness.CurrentBioMsg();
	TUid bioMsgId;

	_LIT(KIsBio, "This IS a BIO message");
	_LIT(KIsntBio, "This IS NOT a BIO message");

	if(bioDB->IsBioMessageL(bioMsg, bioMsgId) == KErrNone)
	{
		iTestHarness.TestUtils().WriteComment(KIsBio);
	}
	else
	{
		iTestHarness.TestUtils().WriteComment(KIsntBio);
	}
	*/
	CleanupStack::PopAndDestroy(bioDB);
}

void CDoCmdBioDB::DoBioDBDefaultSendBearerL()
{
	HBufC* commentText = HBufC::NewLC(100);
	TBuf<100> bearerString;
	CBIODatabase* bioDB = CBIODatabase::NewL(g_rfs);
	CleanupStack::PushL( bioDB );
	
	_LIT(KEmailFound, "Default Send Bearer for EmailNotifciation %S (%d)\n");
	_LIT(KEmailNotFound, "Default Send Bearer for EmailNotifciation cannot be found\n");
	_LIT(KUnexpected, "Unexepect Error %d");

	TBioMsgId aBioMsgIdentifier;
	TRAPD(err, bioDB->GetDefaultSendBearerL(KUidBioMsgTypeEmailNotification, aBioMsgIdentifier));
	if (!err)
	{
		GetBearerText(aBioMsgIdentifier.iType, bearerString);
		(commentText->Des()).Format(KEmailFound ,&bearerString, aBioMsgIdentifier.iType);
	}
	else if (err == KErrNotFound)
	{
		(commentText->Des()).Format(KEmailNotFound);
	}
	else 
	{
		(commentText->Des()).Format(KUnexpected, err);
	}

	iTestHarness.TestUtils().WriteComment(commentText->Des());


	_LIT(KVCalFound, "Default Send Bearer for KUidBioMsgTypeVCal %S (%d)\n");
	_LIT(KVCalNotFound, "Default Send Bearer for KUidBioMsgTypeVCal cannot be found\n");

	TBioMsgIdType aPortType = EBioMsgIdWap;
	TRAP( err, bioDB->GetDefaultSendBearerTypeL(KUidBioMsgTypeVCal, aPortType));
	if (!err)
	{
		GetBearerText(aPortType, bearerString);
		(commentText->Des()).Format(KVCalFound ,&bearerString, aPortType);;
	}
	else if (err == KErrNotFound)
	{
		(commentText->Des()).Format(KVCalNotFound);
	}
	else 
	{
		(commentText->Des()).Format(KUnexpected, err);
	}
	
	iTestHarness.TestUtils().WriteComment(commentText->Des());
	

	_LIT(KVCardNotFound, "Default Send Bearer for KUidBioMsgTypeVCard cannot be found\n");
	
	TRAP(err, bioDB->GetDefaultSendBearerByTypeL(KUidBioMsgTypeEmailNotification, EBioMsgIdWapSecure, aBioMsgIdentifier));
	if (!err) 
	{
		(commentText->Des()).Format(KVCardNotFound,aBioMsgIdentifier.iPort);
	
	}
	else if (err == KErrNotFound)
	{
		(commentText->Des()).Format(KVCardNotFound);
	}
	else
	{
		(commentText->Des()).Format(KUnexpected, err);
	}
		
	iTestHarness.TestUtils().WriteComment(commentText->Des());


	CleanupStack::PopAndDestroy();	// bioDB
	CleanupStack::PopAndDestroy();	// commentText
}

CDoCmdBioDB::~CDoCmdBioDB()
{
}

void CDoCmdBioDB::GetBearerText(TInt aBearer, TBuf<100>& rBearerString)
{
	switch (aBearer)
	{
	case EBioMsgIdIana:
		rBearerString.Copy(_L("Iana"));
		break;
	case EBioMsgIdNbs:
		rBearerString.Copy(_L("Nbs"));
		break;
	case EBioMsgIdWap:
		rBearerString.Copy(_L("Wap"));
		break;
	case EBioMsgIdWapSecure:
		rBearerString.Copy(_L("WapSecure"));
		break;
	case EBioMsgIdUnknown:
	default:
		rBearerString.Copy(_L("Unknown"));
		break;
	}
}

//
// CDoCmdCommDB
// Handles the running of the CommDb Commands

// For 3 argument commands
CDoCmdCommDB* CDoCmdCommDB::NewL(const CDesCArrayFlat& aArgs,CClientTestHarness& aTestHarness, TCmdCommDB aCmd)
{
	CDoCmdCommDB* self = new (ELeave) CDoCmdCommDB(aTestHarness, aCmd);
	CleanupStack::PushL(self);
	self->ConstructL(aArgs);
	CleanupStack::Pop(self);
	return self;
}

void CDoCmdCommDB::ConstructL(const CDesCArrayFlat& aArgs)
{
	iArgs = new(ELeave) CDesCArrayFlat(3);
	CleanupStack::PushL(iArgs);
	for(TInt m=0;m<aArgs.Count();m++)
		iArgs->InsertL(m,aArgs[m]);
	CleanupStack::Pop(iArgs);
	iDBSession = CMDBSession::NewL(CMDBSession::LatestVersion());
}

void CDoCmdCommDB::StartL(TRequestStatus& aStatus)
{
	switch(iCmd)
	{
		//
		// Command takes three arguments:  [Record#] [Field] [TableName]
		// [TableName] is appended by COMMAND PARSER!!!
		case ECmdCommDBReadText:		
			DoCmdCommDBReadTextL();
			break;
		case ECmdCommDBReadLongText:
			break;
		case ECmdCommDBReadUint:
			break;
		case ECmdCommDBReadBool:
			break;

		//
		// Command takes four arguments:  [Field] "Expected Value" [TableName]
		// The "'s are MANDATORY.  [TableName] is appended by COMMAND PARSER!!!
		case ECmdCommDBCheck:  
			DoCmdCommDBCheckL();
			break;		
	}

	TRequestStatus* req = &aStatus;
	User::RequestComplete(req, KErrNone);
}

void CDoCmdCommDB::DoCmdCommDBReadTextL()
	{
	TPtrC mTableName = (*iArgs)[2];
	TPtrC mFieldName = (*iArgs)[1];
	//
	// Count the records
	TInt nNumRecords = 0;

	CMDBRecordSetBase*	recordSet = NULL;
	CMDBRecordSet<CCDDialOutISPRecord>* dialOutispRecordSet = NULL;
	if(mTableName.CompareF(_L("dialoutisp")) == 0)
		{
		 dialOutispRecordSet = new(ELeave) CMDBRecordSet<CCDDialOutISPRecord>(KCDTIdDialOutISPRecord);
		recordSet = static_cast<CMDBRecordSetBase*>(dialOutispRecordSet);
		CleanupStack::PushL(dialOutispRecordSet);
		//cleanup
		TRAPD(err, ((CCDDialOutISPRecord*)recordSet)->LoadL(*iDBSession));
		if(err != KErrNone)
			{
			User::Leave(err);
			}
		nNumRecords = recordSet->iRecords.Count();	
		}

	//
	// Check record number
	TInt  nRecordNum = 0;;	

	_LIT(KFirst, "first");
	_LIT(KLast, "last");
	TPtrC rcNum((*iArgs)[0]);
	if(rcNum.CompareF(KFirst) == 0)
	{
		nRecordNum = 0;
	}
	else
	if(rcNum.CompareF(KLast) == 0)
	{
		nRecordNum = nNumRecords - 1;
	}
	else
	{
		TLex lex((*iArgs)[0]);
			
		lex.Val(nRecordNum);
	nRecordNum = nRecordNum - 1;
		//
		// Numeric record number, check its within range
		if(nRecordNum > nNumRecords - 1 || nRecordNum < 0)
			User::Leave(KErrNotFound);
	}

	TInt nSelectedRecord = nRecordNum;
	if(nSelectedRecord < 0)
		nSelectedRecord = 0;

	TBuf<KCommsDbSvrMaxFieldLength> mVal;
	if(nRecordNum <= nNumRecords)
		{
		if(mFieldName.CompareF(_L("IpNameServer1")) == 0)
			{
			mVal = ((CCDDialOutISPRecord*)recordSet->iRecords[nRecordNum])->iIpNameServer1;						
			}									
		else if(mFieldName.CompareF(_L("IpNameServer2")) == 0)
			{
			mVal = ((CCDDialOutISPRecord*)recordSet->iRecords[nRecordNum])->iIpNameServer2; 
			}			
		}	
	HBufC* mOut = HBufC::NewL(mVal.Length()+mTableName.Length()+mFieldName.Length()+50); //On the CleanupStack
	_LIT(KReadFormatString,"CommDB - Table='%S', Column='%S', Row=%d, Value='%S'");
	mOut->Des().Format(KReadFormatString, &mTableName, &mFieldName, nSelectedRecord, &mVal);
	iTestHarness.TestUtils().WriteComment(*mOut);
	delete mOut;

	if(dialOutispRecordSet)
		{
		CleanupStack::PopAndDestroy(dialOutispRecordSet);		
		}	
}



TBool CDoCmdCommDB::CheckBool(const TDesC& aValue)
	{
	TBool value= EFalse;
	if(aValue.CompareF(_L("false")) == 0)		
		value= EFalse;
	else
		value= ETrue;
	return value;
	}

TInt CDoCmdCommDB::GetIntValue(const TDesC& aValue)
	{
	TInt value = 0;	
	TLex lex(aValue);				
	lex.Val(value);
	return value;
	}
	
TBool CDoCmdCommDB::DoFindRecordExists(const TDesC& aTableNam, const TDesC& aData)
	{
	TInt equalPos = 0; 
	TInt commaPos = 0;
	TBool err = EFalse;
	
	TPtrC fieldData = aData;
	const TChar equal = '=';
	equalPos = fieldData.Locate(equal);
	TPtrC fieldName = fieldData.Left(equalPos);
	fieldData.Set(fieldData.Mid(equalPos+1));
	commaPos = fieldData.Locate(KCommaSeparator);
	TPtrC fieldValue;
	if(commaPos >= 0)
		{
		fieldValue.Set(fieldData.Left(commaPos));
		fieldData.Set(fieldData.Mid(commaPos+1));		
		}
	else
		fieldValue.Set(fieldData);
		
	if(aTableNam.CompareF(_L("DialOutIsp")) == 0)
		{
		CCDDialOutISPRecord* dialOutispRecord = static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
		CleanupStack::PushL(dialOutispRecord);
		do
			{
			equalPos = commaPos = 0;
			if(fieldName.CompareF(_L("Name")) == 0)
				{
				dialOutispRecord->iRecordName.SetMaxLengthL(fieldValue.Length());
				dialOutispRecord->iRecordName = fieldValue;		
				}				
			else if(fieldName.CompareF(_L("LoginName")) == 0)
				{
				dialOutispRecord->iLoginName.SetMaxLengthL(fieldValue.Length());
				dialOutispRecord->iLoginName = fieldValue;		
				}				
			else if(fieldName.CompareF(_L("IfAuthName")) == 0)
				{
				dialOutispRecord->iIfAuthName.SetMaxLengthL(fieldValue.Length());
				dialOutispRecord->iIfAuthName = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("DefaultTelNum")) == 0)
				{
				dialOutispRecord->iDefaultTelNum.SetMaxLengthL(fieldValue.Length());
				dialOutispRecord->iDefaultTelNum = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IpNameServer1")) == 0)
				{
				dialOutispRecord->iIpNameServer1.SetMaxLengthL(fieldValue.Length());
				dialOutispRecord->iIpNameServer1 = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IpNameServer2")) == 0)
				{
				dialOutispRecord->iIpNameServer2.SetMaxLengthL(fieldValue.Length());
				dialOutispRecord->iIpNameServer2 = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IpGateway")) == 0)
				{
				dialOutispRecord->iIpGateway.SetMaxLengthL(fieldValue.Length());
				dialOutispRecord->iIpGateway = fieldValue;	
				}							
			else if(fieldName.CompareF(_L("Description")) == 0)
				{
				dialOutispRecord->iDescription.SetMaxLengthL(fieldValue.Length());
				dialOutispRecord->iDescription = fieldValue;	
				}							
			else if(fieldName.CompareF(_L("UseLoginScript")) == 0)
				{
				dialOutispRecord->iUseLoginScript = CheckBool(fieldValue);	
				}				
			else if(fieldName.CompareF(_L("EnableSWComp")) == 0)
				{
				dialOutispRecord->iEnableSwComp = CheckBool(fieldValue);				
				}				
			else if(fieldName.CompareF(_L("BearerProtocol")) == 0)
				{
				dialOutispRecord->iBearerProtocol = (TUint32)GetIntValue(fieldValue);					
				}				
			else if(fieldName.CompareF(_L("BearerSpeed")) == 0)
				{
				dialOutispRecord->iBearerSpeed = GetIntValue(fieldValue);		
				}				
			else if(fieldName.CompareF(_L("InitString")) == 0)
				{
				TBuf8<100> tempPtr;
				const TBuf16<100> tmpField = fieldValue;			
				TInt val = CnvUtfConverter::ConvertFromUnicodeToUtf8(tempPtr, tmpField);						
				dialOutispRecord->iInitString.SetMaxLengthL(fieldValue.Length());					
				dialOutispRecord->iInitString = tempPtr;			
				}				
			else if(fieldName.CompareF(_L("PromptForLogin")) == 0)
				{
				dialOutispRecord->iPromptForLogin = CheckBool(fieldValue);	
				}				
			else if(fieldName.CompareF(_L("IpAddr")) == 0)
				{
				dialOutispRecord->iIpAddr.SetMaxLengthL(fieldValue.Length());	
				dialOutispRecord->iIpAddr = fieldValue;		
				}						
			else if(fieldName.CompareF(_L("IpNetmask")) == 0)
				{
				dialOutispRecord->iIpNetMask.SetMaxLengthL(fieldValue.Length());
				dialOutispRecord->iIpNetMask = fieldValue;	
				}
								
			equalPos = fieldData.Locate(equal);
			if(equalPos >=0 )
				{
				fieldName.Set(fieldData.Left(equalPos));
				fieldData.Set(fieldData.Mid(equalPos+1));		
				}
			else
				{
				fieldName.Set(fieldData);	
				}
				
			commaPos = fieldData.Locate(KCommaSeparator);
			if(commaPos >= 0 )
				{
				fieldValue.Set(fieldData.Left(commaPos));
				fieldData.Set(fieldData.Mid(commaPos+1));
				}
			else
				{
				fieldValue.Set(fieldData);				
				}
			}while(equalPos > 0);
		
		err = 	dialOutispRecord->FindL(*iDBSession);		
				
		CleanupStack::PopAndDestroy(dialOutispRecord);
		}
	else if(aTableNam.CompareF(_L("IAP")) == 0)
		{
		CCDIAPRecord* iapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
		CleanupStack::PushL(iapRecord);
		do
			{
			equalPos = commaPos = 0;
			if(fieldName.CompareF(_L("Name")) == 0)
				{
				iapRecord->iRecordName.SetMaxLengthL(fieldValue.Length());
				iapRecord->iRecordName = fieldValue;		
				}				
			else if(fieldName.CompareF(_L("IAPServiceType")) == 0)
				{
				iapRecord->iServiceType.SetMaxLengthL(fieldValue.Length());
				iapRecord->iServiceType = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IAPBearerType")) == 0)
				{
				iapRecord->iBearerType.SetMaxLengthL(fieldValue.Length());
				iapRecord->iBearerType = fieldValue;	
				}				
			
			equalPos = fieldData.Locate(equal);
			if(equalPos >=0 )
				{
				fieldName.Set(fieldData.Left(equalPos));
				fieldData.Set(fieldData.Mid(equalPos+1));		
				}
			else
				{
				fieldName.Set(fieldData);	
				}
				
			commaPos = fieldData.Locate(KCommaSeparator);
			if(commaPos >=0 )
				{
				fieldValue.Set(fieldData.Left(commaPos));
				fieldData.Set(fieldData.Mid(commaPos+1));
				}
			else
				{
				fieldValue.Set(fieldData);				
				}				
			}while(equalPos > 0);
		
		err = iapRecord->FindL(*iDBSession);		
				
		CleanupStack::PopAndDestroy(iapRecord);
		}
	else if(aTableNam.CompareF(_L("proxies")) == 0)
		{
		CCDProxiesRecord* proxiesRecord = static_cast<CCDProxiesRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdProxiesRecord));
		CleanupStack::PushL(proxiesRecord);
		do
			{
			equalPos = commaPos = 0;
			if(fieldName.CompareF(_L("ProxyServerName")) == 0)
				{
				proxiesRecord->iServerName.SetMaxLengthL(fieldValue.Length());
				proxiesRecord->iServerName = fieldValue;	
				}
					
			else if(fieldName.CompareF(_L("PortNumber")) == 0)
				{
				proxiesRecord->iPortNumber.SetMaxLengthL(fieldValue.Length());				
				proxiesRecord->iPortNumber = GetIntValue(fieldValue);	
				}
								
			else if(fieldName.CompareF(_L("Exceptions")) == 0)
				{
				TInt fieldLength = fieldValue.Length();
				if(fieldLength>50)
					{
					proxiesRecord->iExceptions.SetMaxLengthL(KCommsDbSvrMaxFieldLength);	
					}
				else
					{
					proxiesRecord->iExceptions.SetMaxLengthL(fieldValue.Length());
					proxiesRecord->iExceptions = fieldValue;	
					}									
				}				
			else if(fieldName.CompareF(_L("ProtocolName")) == 0)
				{
				proxiesRecord->iProtocolName.SetMaxLengthL(fieldValue.Length());
				proxiesRecord->iProtocolName = fieldValue;	
				}
							
			equalPos = fieldData.Locate(equal);
			if(equalPos >=0 )
				{
				fieldName.Set(fieldData.Left(equalPos));
				fieldData.Set(fieldData.Mid(equalPos+1));
				}
			else
				{				
				fieldName.Set(fieldData);
				}
				
			commaPos = fieldData.Locate(KCommaSeparator);
			if(commaPos >= 0)
				{
				fieldValue.Set(fieldData.Left(commaPos));
				fieldData.Set(fieldData.Mid(commaPos+1));
				}
			else
				{
				fieldValue.Set(fieldData);			
				}
			}while(equalPos > 0);
		
		err = 	proxiesRecord->FindL(*iDBSession);		
		CleanupStack::PopAndDestroy(proxiesRecord);
		}
	else if(aTableNam.CompareF(_L("outgoinggprs")) == 0)
		{
		CCDOutgoingGprsRecord* gprsRecord = static_cast<CCDOutgoingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdOutgoingGprsRecord));	
		CleanupStack::PushL(gprsRecord);
		do
			{
			equalPos = commaPos = 0;
			if(fieldName.CompareF(_L("Name")) == 0)
				{
				gprsRecord->iRecordName.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iRecordName = fieldValue;	
				}
			else if(fieldName.CompareF(_L("APN")) == 0)
				{
				gprsRecord->iGPRSAPN.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSAPN = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("PDPAddress")) == 0)
				{
				gprsRecord->iGPRSPDPAddress.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSPDPAddress = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IfAuthName")) == 0)
				{
				gprsRecord->iGPRSIfAuthName.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSIfAuthName = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IfAuthPass")) == 0)
				{
				gprsRecord->iGPRSIfAuthPass.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSIfAuthPass = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IpNameServer1")) == 0)
				{
				gprsRecord->iGPRSIPNameServer1.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSIPNameServer1 = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IpNameServer2")) == 0)
				{
				gprsRecord->iGPRSIPNameServer2.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSIPNameServer2 = fieldValue;	
				}
				
			equalPos = fieldData.Locate(equal);
			if(equalPos >= 0 )
				{
				fieldName.Set(fieldData.Left(equalPos));
				fieldData.Set(fieldData.Mid(equalPos+1));		
				}
			else
				{
				fieldName.Set(fieldData);	
				}
			commaPos = fieldData.Locate(KCommaSeparator);
			if(commaPos >= 0)
				{
				fieldValue.Set(fieldData.Left(commaPos));
				fieldData.Set(fieldData.Mid(commaPos+1));
				}
			else
				{
				fieldValue.Set(fieldData);		
				}						
			}while(equalPos > 0);
		
		err = gprsRecord->FindL(*iDBSession);		
		CleanupStack::PopAndDestroy(gprsRecord);
		}
	else if(aTableNam.CompareF(_L("IncomingGPRS")) == 0)
		{
		CCDIncomingGprsRecord* gprsRecord = static_cast<CCDIncomingGprsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIncomingGprsRecord));
		CleanupStack::PushL(gprsRecord);
		do
			{
			equalPos = commaPos = 0;
			if(fieldName.CompareF(_L("Name")) == 0)
				{
				gprsRecord->iRecordName.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iRecordName = fieldValue;	
				}
			else if(fieldName.CompareF(_L("APN")) == 0)
				{
				gprsRecord->iGPRSAPN.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSAPN = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("PDPAddress")) == 0)
				{
				gprsRecord->iGPRSPDPAddress.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSPDPAddress = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IfAuthName")) == 0)
				{
				gprsRecord->iGPRSIfAuthName.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSIfAuthName = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IfAuthPass")) == 0)
				{
				gprsRecord->iGPRSIfAuthPass.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSIfAuthPass = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IpNameServer1")) == 0)
				{
				gprsRecord->iGPRSIPNameServer1.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSIPNameServer1 = fieldValue;	
				}				
			else if(fieldName.CompareF(_L("IpNameServer2")) == 0)
				{
				gprsRecord->iGPRSIPNameServer2.SetMaxLengthL(fieldValue.Length());
				gprsRecord->iGPRSIPNameServer2 = fieldValue;	
				}
			equalPos = fieldData.Locate(equal);
			if(equalPos >=0 )
				{
				fieldName.Set(fieldData.Left(equalPos));
				fieldData.Set(fieldData.Mid(equalPos+1));		
				}
			else
				{
				fieldName.Set(fieldData);	
				}
			commaPos = fieldData.Locate(KCommaSeparator);
			if(commaPos >= 0)
				{
				fieldValue.Set(fieldData.Left(commaPos));
				fieldData.Set(fieldData.Mid(commaPos+1));
				}
			else
				{
				fieldValue.Set(fieldData);	
				}							
			}while(equalPos > 0);
		
		err = gprsRecord->FindL(*iDBSession);
		CleanupStack::PopAndDestroy(gprsRecord);	
		}
	else if(aTableNam.CompareF(_L("WAPAccessPoint")) == 0)
		{
		CCDWAPAccessPointRecord* wapAPRecord = static_cast<CCDWAPAccessPointRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPAccessPointRecord));
		CleanupStack::PushL(wapAPRecord);
		do
			{
			equalPos = commaPos = 0;
			if(fieldName.CompareF(_L("Name")) == 0)
				{
				wapAPRecord->iRecordName.SetMaxLengthL(fieldValue.Length());
				wapAPRecord->iRecordName = fieldValue;					
				}				
			else if(fieldName.CompareF(_L("StartPage")) == 0)
				{
				wapAPRecord->iWAPStartPage.SetMaxLengthL(fieldValue.Length());
				wapAPRecord->iWAPStartPage = fieldValue;					
				}				
			
			equalPos = fieldData.Locate(equal);
			if(equalPos >= 0 )
				{
				fieldName.Set(fieldData.Left(equalPos));
				fieldData.Set(fieldData.Mid(equalPos+1));		
				}
			else
				{
				fieldName.Set(fieldData);	
				}
				
			commaPos = fieldData.Locate(KCommaSeparator);
			if(commaPos >= 0 )
				{
				fieldValue.Set(fieldData.Left(commaPos));
				fieldData.Set(fieldData.Mid(commaPos+1));
				}
			else
				{
				fieldValue.Set(fieldData);		
				}						
			}while(equalPos > 0);
		
		err = wapAPRecord->FindL(*iDBSession);		
				
		CleanupStack::PopAndDestroy(wapAPRecord);
		}	
	else if(aTableNam.CompareF(_L("WAPIPBearer")) == 0)
		{
		CCDWAPIPBearerRecord* wapIPBearerRecord = static_cast<CCDWAPIPBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPIPBearerRecord));
		CleanupStack::PushL(wapIPBearerRecord);
		do
			{
			equalPos = commaPos = 0;
			if(fieldName.CompareF(_L("GatewayAddress")) == 0)
				{
				wapIPBearerRecord->iWAPGatewayAddress.SetMaxLengthL(fieldValue.Length());					
				wapIPBearerRecord->iWAPGatewayAddress = fieldValue;
				}				
			else if(fieldName.CompareF(_L("ProxyPortNumber")) == 0)
				{
				wapIPBearerRecord->iWAPProxyPort = GetIntValue(fieldValue);	
				}				
			else if(fieldName.CompareF(_L("WSPOption")) == 0)
				{
				wapIPBearerRecord->iWAPWSPOption = GetIntValue(fieldValue);		
				}							
			else if(fieldName.CompareF(_L("Security")) == 0)
				{
				wapIPBearerRecord->iWAPSecurity = CheckBool(fieldValue);	
				}				
			else if(fieldName.CompareF(_L("ProxyLogin")) == 0)
				{
				wapIPBearerRecord->iWAPProxyLoginName.SetMaxLengthL(fieldValue.Length());
				wapIPBearerRecord->iWAPProxyLoginName = fieldValue;	
				}								
			else if(fieldName.CompareF(_L("ProxyPassword")) == 0)
				{
				wapIPBearerRecord->iWAPProxyLoginPass.SetMaxLengthL(fieldValue.Length());	
				wapIPBearerRecord->iWAPProxyLoginPass = fieldValue;		
				}				
			
			equalPos = fieldData.Locate(equal);
			if(equalPos >= 0 )
				{
				fieldName.Set(fieldData.Left(equalPos));
				fieldData.Set(fieldData.Mid(equalPos+1));		
				}
			else
				{
				fieldName.Set(fieldData);	
				}
				
			commaPos = fieldData.Locate(KCommaSeparator);
			if(commaPos >= 0 )
				{
				fieldValue.Set(fieldData.Left(commaPos));
				fieldData.Set(fieldData.Mid(commaPos+1));
				}
			else
				{
				fieldValue.Set(fieldData);	
				}							
			}while(equalPos > 0);
		
		err = wapIPBearerRecord->FindL(*iDBSession);
		CleanupStack::PopAndDestroy(wapIPBearerRecord);
		}			
	else if(aTableNam.CompareF(_L("WAPSMSBearer")) == 0)
		{
		CCDWAPSMSBearerRecord* wapSmsBearerRecord = static_cast<CCDWAPSMSBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdWAPSMSBearerRecord));
		CleanupStack::PushL(wapSmsBearerRecord);
		do
			{
			equalPos = commaPos = 0;
			if(fieldName.CompareF(_L("GatewayAddress")) == 0)
				{
				wapSmsBearerRecord->iWAPGatewayAddress.SetMaxLengthL(fieldValue.Length());
				wapSmsBearerRecord->iWAPGatewayAddress = fieldValue;	
				}								
			else if(fieldName.CompareF(_L("ServiceCentreAddress")) == 0)
				{
				wapSmsBearerRecord->iWAPServiceCentreAddress.SetMaxLengthL(fieldValue.Length());
				wapSmsBearerRecord->iWAPServiceCentreAddress = fieldValue;	
				}								
			
			equalPos = fieldData.Locate(equal);
			if(equalPos >= 0 )
				{
				fieldName.Set(fieldData.Left(equalPos));
				fieldData.Set(fieldData.Mid(equalPos+1));		
				}
			else
				fieldName.Set(fieldData);
			commaPos = fieldData.Locate(KCommaSeparator);
			if(commaPos >= 0 )
				{
				fieldValue.Set(fieldData.Left(commaPos));
				fieldData.Set(fieldData.Mid(commaPos+1));
				}
			else
				fieldValue.Set(fieldData);			
			}while(equalPos > 0);
		
		err = wapSmsBearerRecord->FindL(*iDBSession);		
				
		CleanupStack::PopAndDestroy(wapSmsBearerRecord);
		}		
	return err;
	}

void CDoCmdCommDB::DoCmdCommDBCheckL()
{	
	//
	// Get SQL
	const TPtrC& mSQL = (*iArgs)[0];
	//
	// Get table name
	const TPtrC& mTableName = (*iArgs)[1];
	
	
	
	TBool retValue = DoFindRecordExists(mTableName, mSQL);
	
	if(!retValue)
		{
		_LIT(KReadFormStr,"ERROR: \"%S\" Returned NO match from CommDat");
		//HBufC* mOut = HBufC::NewL(KReadFormatString1().Length() + mSQL.Length());
		HBufC* mOut = HBufC::NewL(250);
		mOut->Des().Format(KReadFormStr, &mSQL);
		delete mOut;
		//User::Leave(retValue);
		}
	else
		{
		_LIT(KReadFormStr,"OK: record found in COMMDAT (SQL=\"%S\")");
		HBufC* mOut = HBufC::NewL(KReadFormStr().Length() + 5 + mSQL.Length());
		mOut->Des().Format(KReadFormStr, /*nRecordNum, */&mSQL);
		iTestHarness.TestUtils().WriteComment((mOut->Left(230)));	// Note we can only log upto 230 chars.
		delete mOut;
		}
}

CDoCmdCommDB::~CDoCmdCommDB()
{
	delete iArgs;
	delete iDBSession;
}

//
// CDoCmdClient
// Handles the running of the Client Commands
CDoCmdClient* CDoCmdClient::NewL(CDesCArrayFlat& aArgs, CClientTestHarness& aTestHarness, const TCmdClient& aCmd)
{
	CDoCmdClient* self = new (ELeave) CDoCmdClient(aTestHarness, aCmd);
	CleanupStack::PushL(self);
	self->ConstructL(aArgs);
	CleanupStack::Pop(self);
	return self;
}

void CDoCmdClient::ConstructL(const CDesCArrayFlat& aArgs)
{
	iArgs = new(ELeave) CDesCArrayFlat(3);
	CleanupStack::PushL(iArgs);
	for(TInt m=0;m<aArgs.Count();m++)
		iArgs->InsertL(m,aArgs[m]);
	CleanupStack::Pop(iArgs);
}

CDoCmdClient::~CDoCmdClient()
{
	delete iArgs;
}

void CDoCmdClient::StartL(TRequestStatus& aStatus)
{
	iTestHarness.InstantiateClientMtmL();

	switch(iCmd)
	{
		//
		// Parse
		case ECmdClientParse:
			DoCmdClientParseL();
			break;

		//
		// Process
		case ECmdClientProcess:
			DoCmdClientProcessL();
			break;

		// 
		// Find in subject
		case ECmdClientFindSubject:
			DoCmdClientFindInSubjectL();
			break;

		//
		// Find in body
		case ECmdClientFindBody:
			DoCmdClientFindInBodyL();
			break;

		default:
			User::Leave(KErrNotFound);
	}

	//
	// We leave on error, so always say KErrNone
	TRequestStatus* req = &aStatus;
	User::RequestComplete(req, KErrNone);
}

void CDoCmdClient::DoCmdClientProcessL()
{
	//
	// Fail the test if we dont have a connection
	User::LeaveIfNull(iTestHarness.TestUtils().iBioClientMtm);

	//
	// Create a wait AO.  This AO just calls CActiveScheduler::Stop in its RunL
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;

	//
	// Create a selection that contains the one BIO msg we are currently working on
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	const TMsvId& msgID = iTestHarness.CurrentBioMsg();
	selection->AppendL(msgID);

	//
	// Start the operation running
	TBuf8<1> parms;	
	CMsvOperation* operation = NULL;
	operation = iTestHarness.TestUtils().iBioClientMtm->InvokeAsyncFunctionL( KBiosMtmProcess, *selection, parms, wait->iStatus);

	//
	// Wait for it to finish
	wait->Start();
	CActiveScheduler::Start();

	//
	// Get error
	TInt error = operation->iStatus.Int();
	delete operation;
	
	// Access the optional parameter, if present should be a error code.
	if (iArgs->Count() > 0)
		{
		TPtrC	arg((*iArgs)[0]);
		TInt	expectedErr = KErrNone;
		TLex	lex;
		HBufC*	comment = HBufC::NewLC(100);

		lex.Assign(arg);
		lex.Val(expectedErr);	// this returns an err code, check

		if (error == expectedErr)
			{
			_LIT(KMsvExpectedErr, "Parser's ProcessL() failed correctly with error code %d");
			comment->Des().Format(KMsvExpectedErr, error);
			}

		else
			{
			_LIT(KMsvExpectedErr, "Parser's ProcessL() failed in-correctly with error code %d, expected error code %d");
			comment->Des().Format(KMsvExpectedErr, error, expectedErr);
			}
		iTestHarness.TestUtils().WriteComment(*comment);
		iTestHarness.SetExpectedError(expectedErr);

		CleanupStack::PopAndDestroy(comment);
		}



	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(wait);

	User::LeaveIfError(error);
}

void CDoCmdClient::DoCmdClientParseL()
{
	//
	// Fail the test if we dont have a connection
	User::LeaveIfNull(iTestHarness.TestUtils().iBioClientMtm);

	//
	// Create a wait AO.  This AO just calls CActiveScheduler::Stop in its RunL
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;

	//
	// Create a selection that contains the one BIO msg we are currently working on
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	const TMsvId& msgID = iTestHarness.CurrentBioMsg();
	selection->AppendL(msgID);
	
	//
	// Get the iMtmData1 and iMtmData3 members
	iTestHarness.TestUtils().iMsvEntry->SetEntryL(msgID);
	TMsvEntry msvEntry = iTestHarness.TestUtils().iMsvEntry->Entry();

	_LIT(KMsvb4, "Message with ID %d BEFORE PARSE: iMtmData1=%d, iMtmData3=%d"); 	
	_LIT(KMsvAfter, "Message with ID %d AFTER PARSE: iMtmData1=%d, iMtmData3=%d");
	
	HBufC* comment = HBufC::NewLC(KMsvb4().Length() + 50);	// 30
	comment->Des().Format(KMsvb4, msgID, msvEntry.MtmData1(), msvEntry.MtmData3());
	iTestHarness.TestUtils().WriteComment(*comment);

	// set waiter active - avoid nested active scheduler problems...
	wait->Start();

	//
	// Start the operation running
	TBuf8<1> parms;	
	CMsvOperation* operation = NULL;
	operation = iTestHarness.TestUtils().iBioClientMtm->InvokeAsyncFunctionL( KBiosMtmParse, *selection, parms, wait->iStatus);

	//
	// Wait for it to finish 
	CActiveScheduler::Start();

	//
	// Re-get the message - it should have changed
	msvEntry = iTestHarness.TestUtils().iMsvEntry->Entry();
	comment->Des().Format(KMsvAfter, msgID, msvEntry.MtmData1(), msvEntry.MtmData3());
	iTestHarness.TestUtils().WriteComment(*comment);

	//
	// Get error
	TInt error = operation->iStatus.Int();
	delete operation;
	
	// Access the optional parameter, if present should be a error code.
	if (iArgs->Count() > 0)
		{
		TPtrC	arg((*iArgs)[0]);
		TInt	expectedErr = KErrNone;	// will have to get this via iTestHarness or something similar DON'T understand comment ????
		TLex	lex;

		lex.Assign(arg);
		lex.Val(expectedErr);	// this returns an err code, check

		if (error == expectedErr)
			{
			_LIT(KMsvExpectedErr, "Parser's ParseL() failed correctly with error code %d");
			comment->Des().Format(KMsvExpectedErr, error);
			}

		else
			{
			_LIT(KMsvExpectedErr, "Parser's ParseL() failed in-correctly with error code %d, expected error code %d");
			comment->Des().Format(KMsvExpectedErr, error, expectedErr);
			}
		iTestHarness.TestUtils().WriteComment(*comment);
		iTestHarness.SetExpectedError(expectedErr);
		}

//	else
//	iTestHarness.SetExpectedError(KErrNone);	// do we need this...,



	CleanupStack::PopAndDestroy(comment);
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(wait);

	User::LeaveIfError(error);
}

void CDoCmdClient::DoCmdClientFindInSubjectL()
{
	_LIT(KMsgNotFound, "Text %S NOT found in message (Id: %d)'s subject");
	_LIT(KMsgFound, "Text %S found in message (Id: %d)'s subject");

	//
	// Fail the test if we dont have a connection
	User::LeaveIfNull(iTestHarness.TestUtils().iBioClientMtm);

	//
	// Swicth to the current BIO message
	const TMsvId& msgID = iTestHarness.CurrentBioMsg();
	iTestHarness.TestUtils().iBioClientMtm->SwitchCurrentEntryL(msgID);
	iTestHarness.TestUtils().iBioClientMtm->LoadMessageL();
	
	//
	// Look for the string
	TPtrC aArg((*iArgs)[0]);
	TMsvPartList retPart =  iTestHarness.TestUtils().iBioClientMtm->Find(aArg, KMsvMessagePartDescription);
	TBuf<200> comment;
	if(retPart & KMsvMessagePartDescription)
	{
		comment.Format(KMsgFound, &aArg, msgID);
	}
	else
	{
		comment.Format(KMsgNotFound, &aArg, msgID);
	}
	
	iTestHarness.TestUtils().WriteComment(comment);
}

void CDoCmdClient::DoCmdClientFindInBodyL()
{
	_LIT(KMsgNotFound, "Text %S NOT found in message (Id: %d)'s body");
	_LIT(KMsgFound, "Text %S found in message (Id: %d)'s body");

	//
	// Fail the test if we dont have a connection
	User::LeaveIfNull(iTestHarness.TestUtils().iBioClientMtm);

	// Swicth to the current BIO message
	const TMsvId& msgID = iTestHarness.CurrentBioMsg();
	iTestHarness.TestUtils().iBioClientMtm->SwitchCurrentEntryL(msgID);
	iTestHarness.TestUtils().iBioClientMtm->LoadMessageL();
	
	//
	// Look for the string
	TPtrC aArg((*iArgs)[0]);
	TMsvPartList retPart =  iTestHarness.TestUtils().iBioClientMtm->Find(aArg, KMsvMessagePartBody);

	TBuf<200> comment;
	if(retPart & KMsvMessagePartBody)
	{
		comment.Format(KMsgFound, &aArg,  msgID);
	}
	else
	{
		comment.Format(KMsgNotFound, &aArg, msgID);
	}
	
	iTestHarness.TestUtils().WriteComment(comment);
}

//
// CDoCmdServer
// Handles the running of the Server Commands
CDoCmdServer* CDoCmdServer::NewL(CDesCArrayFlat& aArgs, CClientTestHarness& aTestHarness, const TCmdServer& aCmd)
{
	CDoCmdServer* self = new (ELeave) CDoCmdServer(aTestHarness, aCmd);
	CleanupStack::PushL(self);
	self->ConstructL(aArgs);
	CleanupStack::Pop(self);
	return self;
}

void CDoCmdServer::ConstructL(const CDesCArrayFlat& aArgs)
{
	iArgs = new(ELeave) CDesCArrayFlat(3);
	CleanupStack::PushL(iArgs);
	for(TInt m=0;m<aArgs.Count();m++)
		iArgs->InsertL(m,aArgs[m]);
	CleanupStack::Pop(iArgs);
}

CDoCmdServer::~CDoCmdServer()
{
	delete iArgs;
}

void CDoCmdServer::StartL(TRequestStatus& aStatus)
{
	iTestHarness.InstantiateServerMtmL();

	switch(iCmd)
	{
		//
		// Parse
		case ECmdServerParse:
			DoCmdServerParseL();
			break;

		//
		// Process
		case ECmdServerProcess:
			DoCmdServerProcessL();
			break;

		default:
			User::Leave(KErrNotFound);
	}

	//
	// We leave on error, so always say KErrNone
	TRequestStatus* req = &aStatus;
	User::RequestComplete(req, KErrNone);
}

void CDoCmdServer::DoCmdServerProcessL()
{
	//
	// Fail the test if we dont have a connection
	User::LeaveIfNull(iTestHarness.TestUtils().iBioServerMtm);

	//
	// Create a wait AO.  This AO just calls CActiveScheduler::Stop in its RunL
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;

	//
	// Create a selection that contains the one BIO msg we are currently working on
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	const TMsvId& msgID = iTestHarness.CurrentBioMsg();
	selection->AppendL(msgID);

	//
	// Start the operation
	TBuf8<1> parms;	
	iTestHarness.TestUtils().iBioServerMtm->StartCommandL(*selection, KBiosMtmProcess, parms, wait->iStatus);

	//
	// Wait for it to finish
	wait->Start();
	CActiveScheduler::Start();

	//
	// Get error
	TInt error = wait->iStatus.Int();
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(wait);

	User::LeaveIfError(error);
}

void CDoCmdServer::DoCmdServerParseL()
{
	//
	// Fail the test if we dont have a connection
	User::LeaveIfNull(iTestHarness.TestUtils().iBioServerMtm);

	//
	// Create a wait AO.  This AO just calls CActiveScheduler::Stop in its RunL
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->iStatus = KRequestPending;

	//
	// Create a selection that contains the one BIO msg we are currently working on
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	const TMsvId& msgID = iTestHarness.CurrentBioMsg();
	selection->AppendL(msgID);

	//
	// Start the operation running
	TBuf8<1> parms;	
	iTestHarness.TestUtils().iBioServerMtm->StartCommandL(*selection, KBiosMtmParse, parms, wait->iStatus);

	//
	// Wait for it to finish
	wait->Start();
	CActiveScheduler::Start();

	//
	// Get error
	TInt error = wait->iStatus.Int();
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(wait);

	User::LeaveIfError(error);
}


//
// CDoCmdMsv
// Handles the running of the Msv commands

// For 3 argument commands
CDoCmdMsv* CDoCmdMsv::NewL(const CDesCArrayFlat& aArgs,CClientTestHarness& aTestHarness, TCmdMsv aCmd)
{
	CDoCmdMsv* self = new (ELeave) CDoCmdMsv(aTestHarness, aCmd);
	CleanupStack::PushL(self);
	self->ConstructL(aArgs);
	CleanupStack::Pop(self);
	return self;
}

CDoCmdMsv* CDoCmdMsv::NewL(const TDesC& aName,CClientTestHarness& aTestHarness, TCmdMsv aCmd)
{
	CDoCmdMsv* self = new (ELeave) CDoCmdMsv(aName,aTestHarness, aCmd);
	return self;
}

void CDoCmdMsv::ConstructL(const CDesCArrayFlat& aArgs)
{
	iArgs = new(ELeave) CDesCArrayFlat(3);
	CleanupStack::PushL(iArgs);
	for(TInt m=0;m<aArgs.Count();m++)
		iArgs->InsertL(m,aArgs[m]);
	CleanupStack::Pop(iArgs);
}

void CDoCmdMsv::StartL(TRequestStatus& aStatus)
{

	
	TPtrC mExpected;
	if ((iCmd!=ECmdDumpSmsMsgCentres) && (iCmd!=ECmdInstantNamedPop3) && (iCmd!=ECmdInstantNamedImap) && (iCmd!=ECmdInstantNamedSmtp)
		&& (iCmd!=ECmdInstantDefaultPop3) && (iCmd!=ECmdInstantDefaultSmtp) && (iCmd!=ECmdInstantDefaultImap))
		mExpected.Set( (*iArgs)[0] );

	switch(iCmd)
	{

	/*
		N.B.  The implementation for some of the features unsupported by the
		Symbian Spec. is present but #commented out - if these features are required
		in future it should only be necessary to fill in the //if compare fails
		stubs.  Several commands have already been implemented which are not supported
		by the BIO spec.
	*/

		//The Sms Settings
		case ECmdDumpSmsMsgCentres:
		{
			iTestHarness.InstantiateSmsSettingsL();

			_LIT(KSmsMsgCentre,"SMS Message Centres present:");
			iTestHarness.TestUtils().WriteComment(KSmsMsgCentre);

			for (TInt j=0;j<=iTestHarness.iSmsSettings->ServiceCenterCount()-1;j++)
			{
				CSmsServiceCenter& mSmsN = iTestHarness.iSmsSettings->GetServiceCenter(j);
				_LIT(KSmsMsgCentre,"   SMS Centre: Name \"%S\" Address \"%S\"");
				HBufC* td = HBufC::NewLC(35+mSmsN.Name().Length()+mSmsN.Address().Length());
				
				TPtrC name = mSmsN.Name();
				TPtrC address = mSmsN.Address();

				td->Des().Format(KSmsMsgCentre,&name,&address);
				iTestHarness.TestUtils().WriteComment(*td);
				CleanupStack::PopAndDestroy(td);

			}
		break;
		}
		case ECmdChkDefaultSmsMsgCentre:
		{
		TBuf<KCommsDbSvrMaxFieldLength> mRead;
		CMDBSession *dbSession = CMDBSession::NewL(CMDBSession::LatestVersion());
		CleanupStack::PushL(dbSession);		
		CCDGlobalSettingsRecord *globalSettingsRecord = static_cast<CCDGlobalSettingsRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdGlobalSettingsRecord));
		CleanupStack::PushL(globalSettingsRecord);
		
		globalSettingsRecord->LoadL(*dbSession);	
		
		CCDModemBearerRecord *modemRecord = static_cast<CCDModemBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
		CleanupStack::PushL(modemRecord);
		modemRecord->SetRecordId(globalSettingsRecord->iModemForPhoneServicesAndSMS); 
		modemRecord->LoadL(*dbSession);
		mRead = modemRecord->iMessageCentreNumber;
		
		CleanupStack::PopAndDestroy(3); //dbSession, globalSettingsRecord , modemRecord

		if (mRead.CompareF(mExpected)!=0)
			{
			_LIT(KTmp,"ERROR - checking Address for DEFAULT SMS centre (from CommDB) expected \"%S\" read \"%S\"");
			HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
			td->Des().Format(KTmp,&mExpected,&mRead);
			iTestHarness.TestUtils().WriteComment(*td);
			CleanupStack::PopAndDestroy(td);
			User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
			}
			else
			{
			_LIT(KOK,"OK Default Sms Centre Address Correct");
			iTestHarness.TestUtils().WriteComment(KOK);
			}
		break;						
		}
		case ECmdChkSmsMsgCentreNumber:
		{
			TPtrC mCentreName = mExpected;
			mExpected.Set((*iArgs)[1]);
			

			iTestHarness.InstantiateSmsSettingsL();

			TPtrC name;
			TPtrC mRead;

			for (TInt j=0;j<=iTestHarness.iSmsSettings->ServiceCenterCount();j++)
			{

				if (j==iTestHarness.iSmsSettings->ServiceCenterCount())
				//Past the end of the array!!
				{
					User::Leave(KErrNotFound);
					break; //To make the control flow obvious to the
					       //compiler
				}

				CSmsServiceCenter& mSmsN = iTestHarness.iSmsSettings->GetServiceCenter(j);
				
				name.Set(mSmsN.Name());
				mRead.Set(mSmsN.Address());
				
				if (name.CompareF(mCentreName)==0) break;  //Found Centre Name
				
			}

			 

			if (mRead.CompareF(mExpected)!=0)
				{
					_LIT(KTmp,"ERROR - checking Address for SMS centre (%S) expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mCentreName.Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mCentreName,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Sms Centre Address Correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}


		break;
		}
	
	    //The POP3 Settings

		case ECmdInstantNamedPop3:
		{
			iTestHarness.InstantiateNamedPop3SettingsL(iName);
			break;
		}

		case ECmdInstantDefaultPop3:
		{
			iTestHarness.UnInstantiatePop3SettingsL();
			iTestHarness.InstantiatePop3SettingsL();
			break;
		}

		case ECmdInstantDefaultSmtp:
		{
			iTestHarness.UnInstantiateSmtpSettingsL();
			iTestHarness.InstantiateSmtpSettingsL();
			break;
		}

		case ECmdInstantDefaultImap:
		{
			iTestHarness.UnInstantiateImapSettingsL();
			iTestHarness.InstantiateImapSettingsL();
			break;
		}

		case ECmdInstantNamedSmtp:
		{
			iTestHarness.InstantiateNamedSmtpSettingsL(iName);
			break;
		}

		case ECmdInstantNamedImap:
		{
			iTestHarness.InstantiateNamedImapSettingsL(iName);
			break;
		}

		case ECmdChkPop3Version:  //Command takes one arguments: [Version#]
		{
				_LIT(KOK, "OK POP3 Version deprecated - ignored");
				iTestHarness.TestUtils().WriteComment(KOK);

			break;
		}

	case ECmdChkPop3UserAddress:  //Command takes one arguments: [user@address]
		{		
				_LIT(KOK, "OK POP3 User Address deprecated - ignored");
				iTestHarness.TestUtils().WriteComment(KOK);

			break;
		}

		case ECmdChkPop3ServerAddress:  //Command takes one arguments: [Server_address]
		{		
				iTestHarness.InstantiatePop3SettingsL();

				TPtrC mRead=iTestHarness.iPop3Settings->ServerAddress();
				
				if (mRead.Compare(mExpected)!=0)
				{
					_LIT(KTmp,"ERROR - checking Pop3 Server Address expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK POP3 Server Address correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}


			break;
		}
		
		case ECmdChkPop3Port:  //Command takes one arguments: [Port#]
		{		
				iTestHarness.InstantiatePop3SettingsL();
						
				//Convert mExpected to a TInt
				TLex lexer(mExpected);
				TInt mExpectedInt;
				User::LeaveIfError(lexer.Val(mExpectedInt));


				TInt mRead = iTestHarness.iPop3Settings->Port();
				
				if (mRead!=mExpectedInt)
				{
					_LIT(KTmp,"ERROR - checking Pop3 Port expected \"%d\" read \"%d\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+30);
					td->Des().Format(KTmp,mExpectedInt,mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK POP3 Port correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}
		case ECmdChkPop3AutoSendOnConnect:  //Command takes one arguments: [True/False]
		{		
				
				iTestHarness.InstantiatePop3SettingsL();
				//Convert mExpected to a TBool
				TBool mExpBool=EFalse;
				_LIT(KTrueDes,"true");
				_LIT(KFalseDes,"false");
				
				if (mExpected.CompareF(KTrueDes)==0)
					mExpBool = ETrue;
				else if (mExpected.CompareF(KFalseDes)==0)
					mExpBool = EFalse;
				else
					{
						//script file syntax error
						User::Leave(KErrNotSupported);
					}


				TBool mRead = iTestHarness.iPop3Settings->AutoSendOnConnect();
				
				if (mRead!=mExpBool)
				{
					_LIT(KTmp,"ERROR - checking Pop3 AutoSendOnConnect expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+6);
					TBuf<5> mRDes = (mRead?_L("TRUE"):_L("FALSE"));
					td->Des().Format(KTmp,&mExpected,&mRDes);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK POP3 AutoSendOnConnect correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}
		case ECmdChkPop3Apop:  //Command takes one arguments: [True/False]
		{		
				iTestHarness.InstantiatePop3SettingsL();

				//Convert mExpected to a TBool
				TBool mExpBool=EFalse;
				_LIT(KTrueDes,"true");
				_LIT(KFalseDes,"false");
				
				if (mExpected.CompareF(KTrueDes)==0)
					mExpBool = ETrue;
				else if (mExpected.CompareF(KFalseDes)==0)
					mExpBool = EFalse;
				else
					{
						//script file syntax error
						User::Leave(KErrNotSupported);
					}


				TBool mRead = iTestHarness.iPop3Settings->Apop();
				
				if (mRead!=mExpBool)
				{
					_LIT(KTmp,"ERROR - checking Pop3 APop expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+6);
					TBuf<5> mRDes = (mRead?_L("TRUE"):_L("FALSE"));
					td->Des().Format(KTmp,&mExpected,&mRDes);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK POP3 APop correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}

		case ECmdChkDeleteMsgsFromServer:  //Command takes one arguments: [True/False]
		{		
				iTestHarness.InstantiatePop3SettingsL();

				//Convert mExpected to a TBool
				TBool mExpBool=EFalse;
				_LIT(KTrueDes,"true");
				_LIT(KFalseDes,"false");
				
				if (mExpected.CompareF(KTrueDes)==0)
					mExpBool = ETrue;
				else if (mExpected.CompareF(KFalseDes)==0)
					mExpBool = EFalse;
				else
					{
						//script file syntax error
						User::Leave(KErrNotSupported);
					}


				TBool mRead = iTestHarness.iPop3Settings->DeleteEmailsWhenDisconnecting();
				
				if (mRead!=mExpBool)
				{
					_LIT(KTmp,"ERROR - checking Pop3 DeleteEmailsWhenDisconnecting expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+6);
					TBuf<5> mRDes = (mRead?_L("TRUE"):_L("FALSE"));
					td->Des().Format(KTmp,&mExpected,&mRDes);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK POP3 DeleteEmailsWhenDisconnecting correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}

		case ECmdChkPop3Password:  //Command takes one arguments: [Password]
		{		
				iTestHarness.InstantiatePop3SettingsL();

				TPtrC8 mRead(iTestHarness.iPop3Settings->Password());
				HBufC* kj = HBufC::NewLC(mRead.Length());
				kj->Des().Copy(mRead);

				if (kj->Compare(mExpected)!=0)
				{
					_LIT(KTmp,"ERROR - checking Pop3 Password expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,kj);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK POP3 Password correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				CleanupStack::PopAndDestroy(kj);

			break;
		}
		
		case ECmdChkPop3LoginName:  //Command takes one arguments: [LoginName]
		{		
				iTestHarness.InstantiatePop3SettingsL();

				TPtrC8 mLogon(iTestHarness.iPop3Settings->LoginName());
				HBufC* kj = HBufC::NewLC(mLogon.Length());
				kj->Des().Copy(mLogon);

				if (kj->Compare(mExpected)!=0)
				{
					_LIT(KTmp,"ERROR - checking Pop3 LoginName expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mLogon.Length());
					td->Des().Format(KTmp,&mExpected,kj);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK POP3 LoginName correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				CleanupStack::PopAndDestroy(kj);

			break;
		}
		
		case ECmdChkSmtpServerAddress :  //Command takes one arguments: [ServerAddress]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				TPtrC mRead(iTestHarness.iSmtpSettings->ServerAddress());

				if (mExpected.Compare(mRead)!=0)
				{
					_LIT(KTmp,"ERROR - checking SMTP ServerAddress expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp ServerAddress correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkSmtpUserAddress:  //Command takes one arguments: [EmailAlias]
		{		
				_LIT(KOK, "OK Smtp User Address deprecated - ignored");
				iTestHarness.TestUtils().WriteComment(KOK);
				

			break;
		}
		case ECmdChkSmtpEmailAlias:  //Command takes one arguments: [EmailAlias]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				TPtrC mRead(iTestHarness.iSmtpSettings->EmailAlias());

				if (mExpected.Compare(mRead)!=0)
				{
					_LIT(KTmp,"ERROR - checking SMTP EmailAlias expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp EmailAlias correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkSmtpEmailAddress:  //Command takes one arguments: [EmailAddress]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				TPtrC mRead(iTestHarness.iSmtpSettings->EmailAddress());

				if (mExpected.Compare(mRead)!=0)
				{
					_LIT(KTmp,"ERROR - checking SMTP EmailAddress expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp EmailAddress correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}


			break;
		}
		case ECmdChkSmtpReplyToAddress:  //Command takes one arguments: [ReplyToAddress]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				TPtrC mRead(iTestHarness.iSmtpSettings->ReplyToAddress());

				if (mExpected.Compare(mRead)!=0)
				{
					_LIT(KTmp,"ERROR - checking SMTP ReplyToAddress expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp ReplyToAddress correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}
		case ECmdChkSmtpReceiptAddress:  //Command takes one arguments: [ReceiptAddress]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				TPtrC mRead(iTestHarness.iSmtpSettings->ReceiptAddress());

				if (mExpected.Compare(mRead)!=0)
				{
					_LIT(KTmp,"ERROR - checking SMTP ReceiptAddress expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp ReceiptAddress correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				
			break;
		}
		case ECmdChkSmtpBodyEncoding:  //Command takes one arguments: [BodyEncoding]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				/*enum TMsgOutboxBodyEncoding
				{
					EMsgOutboxDefault,
					EMsgOutboxNoAlgorithm,
					EMsgOutboxMIME,
					EMsgOutboxMHTMLAsMIME,
					EMsgOutboxMHTMLAlternativeAsMIME
				};*/

				//Convert mExpected to a TMsgOutboxBodyEncoding
				TMsgOutboxBodyEncoding mExpEnum=EMsgOutboxDefault;
				_LIT(KDefaultDes,"default");
				_LIT(KNoAlgorithmDes,"noalgorithm");
				_LIT(KMIMEDes,"mime");
				_LIT(KMHTMLAsMIMEDes,"mhtmlasmime");
				_LIT(KMHTMLAlternativeAsMIMEDes,"mhtmlalternativeasmime");
				
				if (mExpected.CompareF(KDefaultDes)==0)
					mExpEnum = EMsgOutboxDefault;
				else if (mExpected.CompareF(KNoAlgorithmDes)==0)
					mExpEnum = EMsgOutboxNoAlgorithm;
				else if (mExpected.CompareF(KMIMEDes)==0)
					mExpEnum = EMsgOutboxMIME;
				else if (mExpected.CompareF(KMHTMLAsMIMEDes)==0)
					mExpEnum = EMsgOutboxMHTMLAsMIME;
				else if (mExpected.CompareF(KMHTMLAlternativeAsMIMEDes)==0)
					mExpEnum = EMsgOutboxMHTMLAlternativeAsMIME;
				else
					{
						//script file syntax error
						User::Leave(KErrNotSupported);
					}

				TMsgOutboxBodyEncoding mRead=iTestHarness.iSmtpSettings->BodyEncoding();

				if (mRead!=mExpEnum)
				{
					_LIT(KTmp,"ERROR - checking SMTP BodyEncoding expected \"%d\" read \"%d\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+12);
					td->Des().Format(KTmp,mExpEnum,mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp BodyEncoding correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}
		case ECmdChkSmtpDefaultMsgCharSet:  //Command takes one arguments: [DefaultMsgCharSet]
		{		
				
				//This test would have to check equality of Uids
				//We can't convert text from the script file to a TUid
				//trivially so leave:
				User::Leave(KErrNotSupported);

			break;
		}
		case ECmdChkSmtpAddVCardToEmail:  //Command takes one arguments: [AddVCardToEmail]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				//Convert mExpected to a TBool
				TBool mExpBool=EFalse;
				_LIT(KTrueDes,"true");
				_LIT(KFalseDes,"false");
				
				if (mExpected.CompareF(KTrueDes)==0)
					mExpBool = ETrue;
				else if (mExpected.CompareF(KFalseDes)==0)
					mExpBool = EFalse;
				else
					{
						//script file syntax error
						User::Leave(KErrNotSupported);
					}

				TBool mRead(iTestHarness.iSmtpSettings->AddVCardToEmail());

				if (mRead!=mExpBool)
				{
					_LIT(KTmp,"ERROR - checking SMTP AddVCardToEmail expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+6);
					TBuf<5> mRDes = (mRead?_L("TRUE"):_L("FALSE"));
					td->Des().Format(KTmp,&mExpected,&mRDes);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp AddVCardToEmail correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}
		case ECmdChkSmtpAddSignatureToEmail:  //Command takes one arguments: [AddSignatureToEmail]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				//Convert mExpected to a TBool
				TBool mExpBool=EFalse;
				_LIT(KTrueDes,"true");
				_LIT(KFalseDes,"false");
				
				if (mExpected.CompareF(KTrueDes)==0)
					mExpBool = ETrue;
				else if (mExpected.CompareF(KFalseDes)==0)
					mExpBool = EFalse;
				else
					{
						//script file syntax error
						User::Leave(KErrNotSupported);
					}

				TBool mRead(iTestHarness.iSmtpSettings->AddSignatureToEmail());

				if (mRead!=mExpBool)
				{
					_LIT(KTmp,"ERROR - checking SMTP AddSignatureToEmail expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+6);
					TBuf<5> mRDes = (mRead?_L("TRUE"):_L("FALSE"));
					td->Des().Format(KTmp,&mExpected,&mRDes);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp AddSignatureToEmail correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}
		case ECmdChkSmtpRequestReceipts:  //Command takes one arguments: [RequestReceipts]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				//Convert mExpected to a TBool
				TBool mExpBool=EFalse;
				_LIT(KTrueDes,"true");
				_LIT(KFalseDes,"false");
				
				if (mExpected.CompareF(KTrueDes)==0)
					mExpBool = ETrue;
				else if (mExpected.CompareF(KFalseDes)==0)
					mExpBool = EFalse;
				else
					{
						//script file syntax error
						User::Leave(KErrNotSupported);
					}

				TBool mRead(iTestHarness.iSmtpSettings->RequestReceipts());

				if (mRead!=mExpBool)
				{
					_LIT(KTmp,"ERROR - checking SMTP RequestReceipts expected \"%S\" read \"%S\"");					
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+6);
					TBuf<5> mRDes = (mRead?_L("TRUE"):_L("FALSE"));
					td->Des().Format(KTmp,&mExpected,&mRDes);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp RequestReceipts correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}
		case ECmdChkSmtpSendCopyToSelf:  //Command takes one arguments: [SendCopyToSelf]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				/*enum TImSMTPSendCopyToSelf
				{
					ESendNoCopy,
					ESendCopyAsToRecipient,
					ESendCopyAsCcRecipient,
					ESendCopyAsBccRecipient
				};*/
				
				//Convert mExpected to a TImSMTPSendCopyToSelf
				TImSMTPSendCopyToSelf mExpEnum=ESendNoCopy;
				_LIT(K1Des,"nocopy");
				_LIT(K2Des,"copyastorecipient");
				_LIT(K3Des,"copyasccrecipient");
				_LIT(K4Des,"copyasbccrecipient");
				
				
				if (mExpected.CompareF(K1Des)==0)
					mExpEnum = ESendNoCopy;
				else if (mExpected.CompareF(K2Des)==0)
					mExpEnum = ESendCopyAsToRecipient;
				else if (mExpected.CompareF(K3Des)==0)
					mExpEnum = ESendCopyAsCcRecipient;
				else if (mExpected.CompareF(K4Des)==0)
					mExpEnum = ESendCopyAsBccRecipient;
				else
					{
						//script file syntax error
						User::Leave(KErrNotSupported);
					}

				TImSMTPSendCopyToSelf mRead=iTestHarness.iSmtpSettings->SendCopyToSelf();


				if (mRead!=mExpEnum)
				{
					_LIT(KTmp,"ERROR - checking SMTP SendCopyToSelf expected \"%d\" read \"%d\"");
					
					HBufC* td = HBufC::NewLC(KTmp().Length()+12);
					td->Des().Format(KTmp,mExpEnum,mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp SendCopyToSelf correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}/*
		case ECmdChkSmtpSendMessageOption:  //Command takes one arguments: [SendMessageOption]
		{		
				iTestHarness.InstantiateSmtpSettingsL();

				TPtrC8 mRead(iTestHarness.iSmtpSettings->SendMessageOption());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking SMTP SendMessageOption expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,kj);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Smtp SendMessageOption correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				CleanupStack::PopAndDestroy(kj);

			break;
		}

		*/
		
		case ECmdChkImapLoginName:  //Command takes one arguments: [LoginName]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC8 mRead(iTestHarness.iImapSettings->LoginName());
				HBufC* kj = HBufC::NewLC(mRead.Length());
				kj->Des().Copy(mRead);

				if (kj->Compare(mExpected)!=0)
				{
					_LIT(KTmp,"ERROR - checking IMAP LoginName expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,kj);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap LoginName correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				CleanupStack::PopAndDestroy(kj);

			break;
		}
		case ECmdChkImapPassword:  //Command takes one arguments: [Password]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC8 mRead(iTestHarness.iImapSettings->Password());
				HBufC* kj = HBufC::NewLC(mRead.Length());
				kj->Des().Copy(mRead);

				if (kj->Compare(mExpected)!=0)
				{
					_LIT(KTmp,"ERROR - checking IMAP Password expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,kj);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap Password correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				CleanupStack::PopAndDestroy(kj);

			break;
		}
		case ECmdChkImapFolderPath:  //Command takes one arguments: [FolderPath]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC8 mRead(iTestHarness.iImapSettings->FolderPath());
				HBufC* kj = HBufC::NewLC(mRead.Length());
				kj->Des().Copy(mRead);

				if (kj->Compare(mExpected)!=0)
				{
					_LIT(KTmp,"ERROR - checking IMAP FolderPath expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,kj);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap FolderPath correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				CleanupStack::PopAndDestroy(kj);

			break;
		}
		case ECmdChkImapServerAddress:  //Command takes one arguments: [Server_address]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead=iTestHarness.iImapSettings->ServerAddress();
				
				if (mRead.Compare(mExpected)!=0)
				{
					_LIT(KTmp,"ERROR - checking Imap Server Address expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap Server Address correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}


			break;
		}
		
		case ECmdChkImapPort:  //Command takes one arguments: [Port#]
		{		
				iTestHarness.InstantiateImapSettingsL();
						
				//Convert mExpected to a TInt
				TLex lexer(mExpected);
				TInt mExpectedInt;
				User::LeaveIfError(lexer.Val(mExpectedInt));


				TInt mRead = iTestHarness.iImapSettings->Port();
				
				if (mRead!=mExpectedInt)
				{
					_LIT(KTmp,"ERROR - checking Imap Port expected \"%d\" read \"%d\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+30);
					td->Des().Format(KTmp,mExpectedInt,mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap Port correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

			break;
		}

		
		/*
		case ECmdChkImapPathSeparator:  //Command takes one arguments: [PathSeparator]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->PathSeparator());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP PathSeparator expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap PathSeparator correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkImapDisconnectedUserMode:  //Command takes one arguments: [DisconnectedUserMode]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->DisconnectedUserMode());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP DisconnectedUserMode expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap DisconnectedUserMode correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkImapSynchronise:  //Command takes one arguments: [Synchronise]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->Synchronise());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP Synchronise expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap Synchronise correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkImapSubscribe:  //Command takes one arguments: [Subscribe]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->Subscribe());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP Subscribe expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap Subscribe correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkImapAutoSendOnConnect:  //Command takes one arguments: [AutoSendOnConnect]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->AutoSendOnConnect());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP AutoSendOnConnect expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap AutoSendOnConnect correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkImapMaxEmailSize:  //Command takes one arguments: [MaxEmailSize]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->MaxEmailSize());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP MaxEmailSize expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap MaxEmailSize correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkImapDeleteEmailsWhenDisconnecting:  //Command takes one arguments: [DeleteEmailsWhenDisconnecting]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->DeleteEmailsWhenDisconnecting());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP DeleteEmailsWhenDisconnecting expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap DeleteEmailsWhenDisconnecting correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkImapAcknowledgeReceipts:  //Command takes one arguments: [AcknowledgeReceipts]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->AcknowledgeReceipts());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP AcknowledgeReceipts expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap AcknowledgeReceipts correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkImapGetMailOptions:  //Command takes one arguments: [GetMailOptions]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->GetMailOptions());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP GetMailOptions expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap GetMailOptions correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}
		case ECmdChkImapInboxSynchronisationLimit:  //Command takes one arguments: [InboxSynchronisationLimit]
		{		
				iTestHarness.InstantiateImapSettingsL();

				TPtrC mRead(iTestHarness.iImapSettings->InboxSynchronisationLimit());

				if (//Compare fails)
				{
					_LIT(KTmp,"ERROR - checking IMAP InboxSynchronisationLimit expected \"%S\" read \"%S\"");
					HBufC* td = HBufC::NewLC(KTmp().Length()+mExpected.Length()+mRead.Length());
					td->Des().Format(KTmp,&mExpected,&mRead);
					iTestHarness.TestUtils().WriteComment(*td);
					CleanupStack::PopAndDestroy(td);
					User::Leave(KErrGeneral);  // The test failed so the framework requires us to leave
				}
				else
				{
					_LIT(KOK,"OK Imap InboxSynchronisationLimit correct");
					iTestHarness.TestUtils().WriteComment(KOK);
				}

				

			break;
		}

		*/
		default:
		{
		//DO Nothing
		}
	}

	//
	// We leave on error, so always say KErrNone
	TRequestStatus* req = &aStatus;
	User::RequestComplete(req, KErrNone);
}

CDoCmdMsv::~CDoCmdMsv()
{
	delete iArgs;
}
