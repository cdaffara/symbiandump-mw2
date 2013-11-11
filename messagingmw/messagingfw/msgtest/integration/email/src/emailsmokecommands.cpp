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

#include "emailsmokecommands.h"

#ifndef _NO_IAP_PREFS
#include <iapprefs.h>
#endif

_LIT(K_T_MSG_STRUCT_FILE, "c:\\logs\\email\\Entry_Structure.txt");
_LIT(KCountText, "[%4d] Children of '%S' counted -\n\t\t\t      Services: %4d, Folders: %4d, Messages: %4d, Attachments: %4d\n");
_LIT(KErrService, "[%4d] Error! No service has been created or used!");
_LIT(KErrSelectText, "Error! Cannot find entry '%S'.\n");

const TInt KDefaultWaitTime = 5000000;

const TInt KNoBodyText = -1;

class TLineReader : public CBase
	{
private:
	RFile& iFile;
	TBuf8<512> iBuffer;
	TInt iBufferIndex;

	TBool Peek(char& rChar)
		{
		TBool characterFound = ETrue;
		if (iBufferIndex == iBuffer.Size())
			{
			// Refresh buffer
			TInt readError = iFile.Read(iBuffer);
			iBufferIndex = 0;
			if ((readError != KErrNone) || (iBuffer.Size() == 0))
				{
				characterFound = EFalse;
				}
			}

		if (characterFound)
			{
			rChar = iBuffer[iBufferIndex];
			}

		return characterFound;
		};

	TBool GetCharacter(char& rChar)
		{
		TBool characterFound = Peek(rChar);
		if (characterFound)
			{
			iBufferIndex++;
			}

		return characterFound;
		}

public:
	TLineReader(RFile& rFile) : iFile(rFile), iBufferIndex(0)
		{};

	HBufC8* GetLineLC()
		{
		// Find next cr or lf (and ignore subsequent cr's or lf's)
		HBufC8* newLine = HBufC8::NewLC(512);

		char ch;
		
		TBool characterFound = GetCharacter(ch);
		TBool eolFound = EFalse;
		
		while ((characterFound) && (!eolFound))
			{
			if ((ch == 0x0d) || (ch == 0x0a))
				// cr or lf found, ignore subsequent cr and lf's
				{
				eolFound = ETrue;
				while ((characterFound) && ((ch == 0x0d) || (ch == 0x0a)))
					{
					characterFound = Peek(ch);
					if ((characterFound) && ((ch == 0x0d) || (ch == 0x0a)))
						{
						characterFound = GetCharacter(ch);
						}
					}
				}

			if ((characterFound) && (!eolFound))
				{
				newLine->Des().Append(ch);
				characterFound = GetCharacter(ch);
				}
			}
		
		return newLine;
		};
	};




//
//
// CMsvTestCleanMessageFolder
//

void CCleanMessageFolder::StartL(TRequestStatus& aStatus)
	{
	TInt err=KErrNone;
	TRAP(err, iTestUtils.CleanMessageFolderL());
	TRAP(err,iTestUtils.ClearEmailAccountsL());
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

CCleanMessageFolder::CCleanMessageFolder(CEmailTestUtils& aTestUtils) : iTestUtils(aTestUtils)
	{
	}


//
//
// CDumpMailStore
//

void CDumpMailStore::StartL(TRequestStatus& aStatus)
	{
	TFileName filepath(iPath);
	filepath.Insert(0, _L("dump\\"));
	TParse parsedFileName;
	iTestUtils.ResolveLogFile(filepath, parsedFileName);
	
	// Clean up any existing logs
	CFileMan* fileMan = CFileMan::NewL(iTestUtils.FileSession()); 
	CleanupStack::PushL(fileMan);
	TInt err = fileMan->RmDir(parsedFileName.FullName()); 

	if(err == KErrNone || err == KErrNotFound)
		{
		iTestUtils.ResolveLogFile(filepath, parsedFileName); // create logs dir again
		TRAP(err, iTestUtils.FindChildrenL(KMsvRootIndexEntryId, parsedFileName.FullName()));
		}
	
	CleanupStack::PopAndDestroy(); // fileman
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

CDumpMailStore::CDumpMailStore(CEmailTestUtils& aTestUtils, const TDesC& aPath) : iTestUtils(aTestUtils)
	{
	iPath = aPath;
	}


//
//
// CCompareFiles
//
//  Compares two files passed in as arguments, the first residing in \MsgLogs\t_email\dump\ and the second
//  residing in \msgtest\emailsmoke\reference\ .

void CCompareFiles::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}


void CCompareFiles::StartL(TRequestStatus& aStatus)
	{
	TInt err=KErrNone;
	
	RFileReadStream file1;
	RFileReadStream file2;
	HBufC8* line1Buffer = HBufC8::NewLC(1024);
	TPtr8 line1 = line1Buffer->Des();
	TBuf8<1> aChar1;

	HBufC8* line2Buffer = HBufC8::NewLC(1024);
	TPtr8 line2 = line2Buffer->Des();
	TBuf8<1> aChar2;

	// Generate the file names, first the result file and then the reference file.

	TFileName file1Name(iPath1);
	file1Name.Insert(0, _L("dump\\"));
	TParse parsedFileName1;
	iTestUtils.ResolveLogFile(file1Name, parsedFileName1);
	
	
	TFileName file2Name(iPath2);
	TParse parsedFileName2;
	iTestUtils.ResolveFile(_L("emailsmoke\\reference"), file2Name, parsedFileName2);

	TInt error1=KErrNone;
	TInt error2=KErrNone;
		
	error1=file1.Open(iTestUtils.FileSession(), parsedFileName1.FullName(), EFileShareAny);
	if(error1!=KErrNone)
		{
		LogCommentFormat( _L("\nERROR %d opening file %s"), error1, file1Name);
		err = KErrUnknown;
		}
	else
		{
		error2=file2.Open(iTestUtils.FileSession(), parsedFileName2.FullName(), EFileShareAny);
		if(error2!=KErrNone)
			{
			LogCommentFormat( _L("\nERROR %d opening file %s"), error2, file2Name);
			err = KErrUnknown;
			}
			else
				LogCommentFormat( _L("\nFiles open"));
		}
		
	if((error1==KErrNone)&&(error2==KErrNone))
		{
		// read the file into the conversion object
		TInt file1LineCounter = 0;
	
		TBool finished = EFalse;
	
		do	{
			line1.FillZ();
			line1.SetLength(0);
			// compile the lines one char at a time
			do	{
				TRAPD( error, file1.ReadL(aChar1, 1) ); 
				if (error!=KErrEof)
					line1.Append(aChar1);
				else
					{
					finished = ETrue;
					break;
					}
				// stop at the end of line or no more data
				}
			while((aChar1[0]!=0x0A)&&(line1.Length()<1024));

			line2.FillZ();
			line2.SetLength(0);
			do	{
				TRAPD( error, file2.ReadL(aChar2, 1) ); 
				if (error!=KErrEof)
					line2.Append(aChar2);
				else
					{
					finished = ETrue;
					break;
					}
				// stop at the end of line or no more data
				}
			while((aChar2[0]!=0x0A)&&(line2.Length()<1024));
				
			if(!finished)
				{
				line1.TrimRight();
				line2.TrimRight();

				if(aChar1[0]==0x0A) 
					file1LineCounter++;

				if(line1.Compare(line2))
					{
					LogCommentFormat(_L("\nERROR - Line %d is different "), file1LineCounter);
					err = KErrUnknown;
					}
				}
			}
		while(!finished);
		}		

	file1.Close();
	file2.Close();

	CleanupStack::PopAndDestroy(2);  //line 1 , line 2

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	
	}

CCompareFiles::CCompareFiles(CMsvTestUtils& aTestUtils, const TDesC& aPath1, const TDesC& aPath2) : iTestUtils(aTestUtils)
	{
	iPath1 = aPath1;
	iPath2 = aPath2;
	}


//
//
// CDumpToFile
//

void CDumpToFile::WriteToFileL(RFile& aFile, const TDesC& aInputLine)
	{
	HBufC8* pOutputLine = HBufC8::NewLC(aInputLine.Length()*2);
	TPtr8 pOutput = pOutputLine->Des();
	pOutput.Zero();

	TUint lowChar;
	TUint highChar;
	TInt i=0;
	
	while(i<aInputLine.Length())
		{
		lowChar=((TUint)aInputLine[i])&0x00FF;
		pOutput.Append(lowChar);
		highChar=((TUint)aInputLine[i++])&0xFF00;
		pOutput.Append(highChar);
		}

	aFile.Write(pOutput);
	CleanupStack::PopAndDestroy(); // pBodyText
	}



//
//
// CDumpBodyText
//

void CDumpBodyText::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;
	TFileName filepath(iPath);
	filepath.Insert(0, _L("dump\\"));
	TParse parsedFileName;
	iTestUtils.ResolveLogFile(filepath, parsedFileName);
	
	// delete any previous logs 
	err = iTestUtils.FileSession().Delete(parsedFileName.FullName());
	
	if(err == KErrNone || err == KErrNotFound)
		{
		RFile file;
		err = file.Replace(iTestUtils.FileSession(), parsedFileName.FullName(), EFileShareAny | EFileStreamText | EFileWrite);
		if(err == KErrNone)
			{
			TInt offset=0;
			err = file.Seek(ESeekEnd, offset);

			if (err == KErrNone)
				{
				TInt numEmails = iSelection.Count();
				TMsvId entryId;
				
				// dump the contents of every message in the selection to the file
				for(TInt i=0; i<numEmails; i++)
					{
					entryId = iSelection.At(i);
					DumpBodyTextToFileL(file, entryId);
					}
				}
			file.Close();
			}
		}
	
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

void CDumpBodyText::DumpBodyTextToFileL(RFile& aFile, TMsvId& aEntryId)
	{
	TBuf<100> buf;
	CParaFormatLayer* paraFormatLayer=CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);

	CCharFormatLayer* charFormatLayer=CCharFormatLayer::NewL(); 
	CleanupStack::PushL(charFormatLayer);

	CRichText* bodyText=CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage, 256);
	CleanupStack::PushL(bodyText);

	CImEmailMessage* imEmailMessage = CImEmailMessage::NewLC(*(iTestUtils.iMsvEntry));
	imEmailMessage->GetBodyTextL(aEntryId, CImEmailMessage::EThisMessageOnly, *bodyText, *paraFormatLayer, *charFormatLayer);
	
	HBufC* pBodyText = HBufC::NewLC(bodyText->DocumentLength()+(bodyText->DocumentLength()/70)+1);
	TPtr pBody = pBodyText->Des();
	bodyText->Extract(pBody, 0);

	buf.Zero();
	//buf.AppendFormat(_L("*** %d *************** RichText Data ***************\n"), aEntryId);
	buf.AppendFormat(_L("************************** RichText Data ***************\n"));
	buf.AppendFormat(_L("Size >>> %d\n"), bodyText->DocumentLength());	
	WriteToFileL(aFile, buf);
	
	RemoveRichtextFormating(pBody);
	WriteToFileL(aFile, pBody);
	
	buf.Zero();
	buf.AppendFormat(_L("\n********************* end of Body ***********************\n"));
	WriteToFileL(aFile, buf);
	
	CleanupStack::PopAndDestroy(); // pBodyText
	CleanupStack::PopAndDestroy();  // imEmailMessage
	CleanupStack::PopAndDestroy(3); // bodyText, charFormatLayer, paraFormatLayer
	}

CDumpBodyText::CDumpBodyText(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils, const TDesC& aPath) : 
	iSelection(aSelection),
	iTestUtils(aTestUtils)
	{
	iPath = aPath;
	}

void CDumpBodyText::RemoveRichtextFormating(TDes& aSourceLine)
	{
	TUint8* ptr = (TUint8*)aSourceLine.Ptr();
	TUint8* start = ptr;

	TInt totalLength = aSourceLine.Length()*2;
	do {
		if(*ptr==CEditableText::EParagraphDelimiter || *ptr==CEditableText::ELineBreak || *ptr==CEditableText::EPageBreak)
			*ptr=0x0A;
		} while((++ptr-start)<totalLength);
	}
	

//
//
// CGetAttachmentNames
//

void CGetAttachmentNames::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;
	TFileName filepath(iPath);
	filepath.Insert(0, _L("dump\\"));
	TParse parsedFileName;
	iTestUtils.ResolveLogFile(filepath, parsedFileName);
	
	// delete any previous logs 
	err = iTestUtils.FileSession().Delete(parsedFileName.FullName());
	
	if(err == KErrNone || err == KErrNotFound)
		{
		RFile file;
		err = file.Replace(iTestUtils.FileSession(), parsedFileName.FullName(), EFileShareAny | EFileStreamText | EFileWrite);
		if(err == KErrNone)
			{
			TInt offset=0;
			err = file.Seek(ESeekEnd, offset);

			if (err == KErrNone)
				{
				TInt numEmails = iSelection.Count();
				TMsvId entryId;
				
				// dump the attachment names of every message in the selection to the file
				for(TInt i=0; i<numEmails; i++)
					{
					entryId = iSelection.At(i);
					DumpAttachmentNamesToFileL(file, entryId);
					}
				}
			file.Close();
			}
		}
	
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

void CGetAttachmentNames::DumpAttachmentNamesToFileL(RFile& aFile, TMsvId& aEntryId)
	{
	TBuf<100> buf;

	CImEmailMessage* imEmailMessage = CImEmailMessage::NewLC(*(iTestUtils.iMsvEntry));
	imEmailMessage->GetAttachmentsListL(aEntryId, CImEmailMessage::EAllAttachments, CImEmailMessage::EThisMessageOnly);
	
	TPtrC attachName(KNullDesC);
	
	MMsvAttachmentManager& manager = imEmailMessage->AttachmentManager();
		
	for(TInt i =0; i<manager.AttachmentCount(); i++)
		{
		CMsvAttachment* attachmentInfo = manager.GetAttachmentInfoL(i);
		CleanupStack::PushL(attachmentInfo);
		
		attachName.Set(attachmentInfo->AttachmentName());		
		WriteToFileL(aFile, attachName);
		buf.Zero();
		buf.AppendFormat(_L("\n"));
		WriteToFileL(aFile, buf);
		
		CleanupStack::PopAndDestroy(attachmentInfo);
		}	
	
	CleanupStack::PopAndDestroy(imEmailMessage);  // imEmailMessage
	}

CGetAttachmentNames::CGetAttachmentNames(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils, const TDesC& aPath) : 
	iSelection(aSelection),
	iTestUtils(aTestUtils)
	{
	iPath = aPath;
	}


//
//
// CDumpHeaderFields
//

void CDumpHeaderFields::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;
	TFileName filepath(iPath);
	filepath.Insert(0, _L("dump\\"));
	TParse parsedFileName;
	iTestUtils.ResolveLogFile(filepath, parsedFileName);
	
	// delete any previous logs 
	err = iTestUtils.FileSession().Delete(parsedFileName.FullName());
	
	if(err == KErrNone || err == KErrNotFound)
		{
		RFile file;
		err = file.Replace(iTestUtils.FileSession(), parsedFileName.FullName(), EFileShareAny | EFileStreamText | EFileWrite);
		if(err == KErrNone)
			{
			TInt offset=0;
			err = file.Seek(ESeekEnd, offset);

			if (err == KErrNone)
				{
				TInt numEmails = iSelection.Count();
				TMsvId entryId;
				
				// dump the header fields of every message in the selection to the file
				for(TInt i=0; i<numEmails; i++)
					{
					entryId = iSelection.At(i);
					DumpHeaderFieldsToFileL(file, entryId);
					}
				}
			file.Close();
			}
		}
	
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

void CDumpHeaderFields::DumpHeaderFieldsToFileL(RFile& aFile, TMsvId& aEntryId)
	{
	TInt i;
	
	iTestUtils.iMsvEntry->SetEntryL(aEntryId);
	CImHeader* imHeader = CImHeader::NewLC();
	
	CMsvStore* store = iTestUtils.EditStoreL();
	CleanupStack::PushL(store);
	
	imHeader->RestoreL(*store);
	
	TPtrC subjectField(imHeader->Subject());
	WriteToFileL(aFile, subjectField);
	WriteToFileL(aFile, _L("\n"));
	
	TPtrC fromField(imHeader->From());
	WriteToFileL(aFile, fromField);
	WriteToFileL(aFile, _L("\n"));
	
	const CDesCArray& toField = imHeader->ToRecipients();
	for (i=0; i < toField.Count(); i++)
		{
		WriteToFileL(aFile, toField[i]);
		}
	WriteToFileL(aFile, _L("\n"));
	
	const CDesCArray& ccField = imHeader->CcRecipients();
	for (i=0; i < ccField.Count(); i++)
		{
		WriteToFileL(aFile, ccField[i]);
		}
	WriteToFileL(aFile, _L("\n"));

	const CDesCArray& bccField = imHeader->BccRecipients();
	for (i=0; i < bccField.Count(); i++)
		{
		WriteToFileL(aFile, bccField[i]);
		}
	WriteToFileL(aFile, _L("\n"));
	
	CleanupStack::PopAndDestroy(2, imHeader);  // store, imHeader
	}

CDumpHeaderFields::CDumpHeaderFields(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils, const TDesC& aPath) : 
	iSelection(aSelection),
	iTestUtils(aTestUtils)
	{
	iPath = aPath;
	}


//
//
// CChangeCharset
//
	
CChangeCharset::CChangeCharset(TUint aNewCharset, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection), 
  iNewCharset(aNewCharset)
	{
	}

void CChangeCharset::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;
	TInt numEmails = iSelection.Count();
	TMsvId entryId;
	CImEmailMessage* imEmailMessage = CImEmailMessage::NewLC(*(iTestUtils.iMsvEntry));
	TUint storedCharset;
	TInt override;
	CImHeader* imHeader = CImHeader::NewLC();
	
	// change the charset of every message in the selection
	for(TInt i=0; i<numEmails; i++)
		{
		entryId = iSelection.At(i);
		imEmailMessage->SetCharacterSetL(entryId, iNewCharset);
		// Verify the charset has been changed, and it's been overriden
		imEmailMessage->GetCharacterSetL(entryId, storedCharset, override);
		if(storedCharset != iNewCharset || override == FALSE)
			err = KErrUnknown;

		// do the header charset as well
		iTestUtils.iMsvEntry->SetEntryL(entryId);
		
		CMsvStore* store = iTestUtils.EditStoreL();
		CleanupStack::PushL(store);
		
		imHeader->RestoreL(*store);
		imHeader->SetOverrideCharset(iNewCharset);
		// Verify we can retrieve it
		storedCharset = imHeader->OverrideCharset();
		
		// redecode header fields
		imHeader->ReDecodeL(iTestUtils.FileSession());
		// store it for later
		imHeader->StoreL(*store);
		
		store->CommitL();
		
		CleanupStack::PopAndDestroy(store);
		}
	CleanupStack::PopAndDestroy(2, imEmailMessage); // imHeader, imEmailMessage

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}


//
//
// CAddComment
//

CAddComment* CAddComment::NewL(const CDesCArrayFlat& aComment, CMsvTestUtils& aTestUtils)
	{
	CAddComment* self = new (ELeave) CAddComment(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL(aComment);
	CleanupStack::Pop(self);
	return self;
	}

void CAddComment::ConstructL(const CDesCArrayFlat& aComment)
	{
	// need to construct one big HBufC string to use as the comment
	//  first, find out size of array...
	TInt commentSize = 0;
	for(TInt i = 0; i < aComment.Count(); i++)
		commentSize += ((aComment[i]).Length() + _L(" ").Size()); // extra 1 added for padding between words.

	//  second, construct HBufC.... 
	iComment = HBufC::NewL(commentSize);

	//  create a pointer to the HBufC, so that we can add text to it.
	TPtr commentPtr = iComment->Des();

	//  now create string
	for(TInt j = 0; j < aComment.Count(); j++)
		{
		commentPtr.Append((aComment)[j]);
		commentPtr.Append(_L(" ")); // extra 1 added for padding between words.
		}

	//  hopefully everything should be added ok!
	}

void CAddComment::StartL(TRequestStatus& aStatus)
	{
	// now we have to add the debugging line number which we only know at run time...
	TBuf<7> commentString;
	commentString.Format(_L("[%4d] "), iDebugInfo.LineNumber());

	// create a pointer to the HBufC, realloc size and then insert new data...
	iComment = iComment->ReAllocL(iComment->Size() + commentString.Size());
	TPtr commentPtr = iComment->Des();
	commentPtr.Insert(0, commentString);

	iTestUtils.WriteComment(*iComment);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

CAddComment::CAddComment(CMsvTestUtils& aTestUtils) : iTestUtils(aTestUtils)
	{
	}

CAddComment::~CAddComment()
	{
	delete iComment;
	}

//
//
// CCreateEmailService
//

void CCreateEmailService::StartL()
	// This function should always be called by the StartL function of
	// any derived classes. It should be called by the StartL function of any derived classes.
	{
	}

CCreateEmailService::CCreateEmailService(CEmailClientTest& aTestHarness) : iTestHarness(aTestHarness)
	{
	}

CCreateEmailService::~CCreateEmailService()
	{
	delete iDetails;
	}

TMsvEntry& CCreateEmailService::Entry()
	{
	return iEntry;
	}

void CCreateEmailService::SetDetailsStringL(const TDesC& aDetails)
	{
	delete iDetails;
	iDetails = 0;
	iDetails = HBufC::NewL(aDetails.Length());
	(*iDetails) = aDetails;
	iEntry.iDetails.Set(*iDetails);
	}


//
//
// CCountChildren
//
CCountChildren::CCountChildren(TMsvId& aParentFolder, CEmailClientTest& aTestHarness) 
: iTestHarness(aTestHarness),
  iParentFolder(aParentFolder)
  	{
	}

void CCountChildren::StartL(TRequestStatus& aStatus)
	{

	iTestHarness.EmailTestUtils().iMsvEntry->SetEntryL(iParentFolder);

	// get a list of children underneath the parent.
	CMsvEntrySelection* tempSelection = iTestHarness.EmailTestUtils().iMsvEntry->ChildrenL();
	CleanupStack::PushL(tempSelection);

	// create a temp CMsvEntry to use to find out message type.
	CMsvEntry& msvEntry = *(iTestHarness.EmailTestUtils().iMsvEntry);

	// and while we are pointing at the parent, find out its iDetails
	HBufC* messageDetails = HBufC::NewLC(msvEntry.Entry().iDetails.Size()); 
	(messageDetails->Des()).Copy(msvEntry.Entry().iDetails);

	// Counters
	TInt noServices = 0;
	TInt noFolders = 0;
	TInt noMessages = 0;
	TInt noAttachments = 0;

	// Copy the child entries to the given selection
	TInt index = tempSelection->Count();
	while (index--)
		{
		msvEntry.SetEntryL((*tempSelection)[index]);
		if(msvEntry.Entry().iType == KUidMsvServiceEntry)
			{
			noServices++;
			}
		else if(msvEntry.Entry().iType == KUidMsvFolderEntry)
			{
			noFolders++;
			}
		else if(msvEntry.Entry().iType == KUidMsvMessageEntry)
			{
			noMessages++;
			}
		else if(msvEntry.Entry().iType == KUidMsvAttachmentEntry)
			{
			noAttachments++;
			}
		}

	TBuf<256> countString;
	countString.Format(KCountText, iDebugInfo.LineNumber(), messageDetails, noServices, noFolders, noMessages, noAttachments);
	HBufC* countStringHBufC = HBufC::NewLC(countString.Length());
	(countStringHBufC->Des()).Copy(countString);

	iTestHarness.EmailTestUtils().WriteComment(*countStringHBufC);
	CleanupStack::PopAndDestroy(3);

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}


//
//
// CCheckService
//
CCheckService::CCheckService(CEmailClientTest& aTestHarness) : iTestHarness(aTestHarness)
	{
	}

void CCheckService::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	// This is in the wrong parser!!!!  As this contains IMAP specific code it 
	//  should be moved to imaptests.cpp

	if(!iTestHarness.EmailTestUtils().iImapServiceId)
		{
		TBuf<100> logString;
		logString.Format(KErrService, iDebugInfo.LineNumber());
		iTestHarness.LogComment(logString);
		err = KErrUnknown;
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}


//
//
// CGoClientSide
//

void CGoClientSide::StartL(TRequestStatus& aStatus)
	{
	iTestUtils.GoClientSideL();
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

CGoClientSide::CGoClientSide(CMsvTestUtils& aTestUtils) : iTestUtils(aTestUtils)
	{}




//
//
// CGoServerSide
//

void CGoServerSide::StartL(TRequestStatus& aStatus)
	{
	iTestUtils.GoServerSideL();
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

CGoServerSide::CGoServerSide(CMsvTestUtils& aTestUtils) : iTestUtils(aTestUtils)
	{}




//
//
// CResetMessageServer
//

void CResetMessageServer::StartL(TRequestStatus& aStatus)
	{
	iTestUtils.Reset();
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

CResetMessageServer::CResetMessageServer(CMsvTestUtils& aTestUtils) : iTestUtils(aTestUtils)
	{}






//
//
// CCheckMessageFolder
//

void CCheckMessageFolder::StartL(TRequestStatus& aStatus)
	{
	TBool matched = ETrue;
	iTestUtils.FindChildrenL(0x01000, ETrue);

	RFile file1;
	User::LeaveIfError(file1.Open(iTestUtils.FileSession(), K_T_MSG_STRUCT_FILE, EFileShareAny));

	RFile file2;
	User::LeaveIfError(file2.Open(iTestUtils.FileSession(), iFileName, EFileShareAny));

	TLineReader lineReader1(file1);
	TLineReader lineReader2(file2);

	HBufC8* actualLine = lineReader1.GetLineLC();
	HBufC8* expectedLine = lineReader2.GetLineLC();

	while ((actualLine->Des().Size() > 0) && (expectedLine->Des().Size() > 0) && (matched))
		{
		if (actualLine->Des() != expectedLine->Des())
			matched = EFalse;

		CleanupStack::PopAndDestroy(2); // actualLine, expectedLinw

		actualLine = lineReader1.GetLineLC();
		expectedLine = lineReader2.GetLineLC();
		}

	if ((actualLine->Des().Size() != 0)
		|| (expectedLine->Des().Size() != 0))
		{
		matched = EFalse;
		}

	CleanupStack::PopAndDestroy(2); // actualLine, expectedLinw

	if (!matched)
		{
		User::Leave(KErrNotFound);
		}

	file1.Close();
	file2.Close();

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

CCheckMessageFolder::CCheckMessageFolder(const TDesC& aFileName, CMsvTestUtils& aTestUtils) : iTestUtils(aTestUtils)
	{
	iFileName = aFileName;
	}


//
//
// CChangeMsvDrive
//

CChangeMsvDrive::CChangeMsvDrive(CMsvClientTest& aParentTestHarness, TInt aDrive) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness), iDrive(aDrive)
	{
	CActiveScheduler::Add(this);
	}

void CChangeMsvDrive::StartL(TRequestStatus& aStatus)
	{
	iReportStatus = &aStatus;
		
	iParentTestHarness.SetCurrentOperation(iParentTestHarness.MsvTestUtils().iMsvSession->ChangeDriveL(iDrive, iStatus));
	aStatus = KRequestPending;
	SetActive();
	}

void CChangeMsvDrive::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CChangeMsvDrive::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}

CChangeMsvDrive::~CChangeMsvDrive()
	{
	}


//
//
// CRemoveFileOrDir
//

CRemoveFileOrDir::CRemoveFileOrDir(CMsvTestUtils& aTestUtils, const TDesC& aPath) : iTestUtils(aTestUtils)
	{
	iPath = aPath;
	}
	
void CRemoveFileOrDir::StartL(TRequestStatus& aStatus)
	{
	CFileMan* fileMan = CFileMan::NewL(iTestUtils.FileSession()); 
	CleanupStack::PushL(fileMan);
	TParse parse;

	parse.Set(iPath, NULL, NULL);
	TInt error;
	if(parse.NamePresent())
		{
		// specified a file to delete
		error = fileMan->Delete(parse.FullName());
		error = iTestUtils.FileSession().Delete(parse.FullName());
		}
	else
		{
		// specified a directoy to delete
		error = fileMan->RmDir(parse.DriveAndPath()); 
		error = iTestUtils.FileSession().RmDir(parse.DriveAndPath()); 
		}
	if (!(error==KErrNotFound||error==KErrNone))
		{
		TPtrC driveAndPath = parse.DriveAndPath();
		iTestUtils.Printf(_L("Directory %S cannot be removed. "), &driveAndPath);
		iTestUtils.Printf(_L("Please ensure directory is not in use.\n"));
		User::Leave(KErrAccessDenied);
		}
	CleanupStack::PopAndDestroy();

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}


//
//
// CSelectEntry
//

CSelectEntry::CSelectEntry(const TMsvId& aCurrentFolderId, TMsvId* aEntryId, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils), 
  iCurrentFolderId(aCurrentFolderId), 
  iEntryId(aEntryId)
	{
	iEntrySelection = NULL;
	}

CSelectEntry::CSelectEntry(const TMsvId& aCurrentFolderId, CMsvEntrySelection* aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iCurrentFolderId(aCurrentFolderId), 
  iEntrySelection(aSelection)
	{
	iEntryId = NULL;
	}

CSelectEntry* CSelectEntry::NewL(const TDesC& aName, const TMsvId& aCurrentFolderId, TMsvId* aEntryId, CMsvTestUtils& aTestUtils)
	{
	CSelectEntry* self = new (ELeave) CSelectEntry(aCurrentFolderId, aEntryId, aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL(aName);
	CleanupStack::Pop(self);
	return self;
	}

CSelectEntry* CSelectEntry::NewL(const TDesC& aName, const TMsvId& aCurrentFolderId, CMsvEntrySelection* aSelection, CMsvTestUtils& aTestUtils)
	{
	CSelectEntry* self = new (ELeave) CSelectEntry(aCurrentFolderId, aSelection, aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL(aName);
	CleanupStack::Pop(self);
	return self;
	}

void CSelectEntry::ConstructL(const TDesC& aName)
	{
	iEntryName = HBufC::NewL(aName.Length());
	(*iEntryName) = aName;
	}

void CSelectEntry::StartL(TRequestStatus& aStatus)
	{
	// Select current folder
	CMsvEntry& entry = *(iTestUtils.iMsvEntry);
	entry.SetEntryL(iCurrentFolderId);

	// show the invisible folders.....
	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entry.SetSortTypeL(order);

	CMsvEntrySelection* tempSelection = entry.ChildrenL();
	CleanupStack::PushL(tempSelection);
	
	// Check each child entry for a matching string
	TBool found = EFalse;
	TInt entryIndex = tempSelection->Count();
	while ((!found) && (entryIndex-- != 0))
		{
		entry.SetEntryL((*tempSelection)[entryIndex]);
		if (iEntryName->CompareF(entry.Entry().iDescription) == 0)
			{
			found = ETrue;
			}
		else if (iEntryName->CompareF(entry.Entry().iDetails) == 0)
			{
			found = ETrue;
			}
		}

	if (found)
		{
		// decide if its a Selection thats required, or an Entry
		if(iEntryId)
			{
			*iEntryId = entry.Entry().Id();
			}
		else
			{
			(*iEntrySelection).Reset();
			(*iEntrySelection).AppendL(entry.Entry().Id());
			}

		TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNone);
		}
	else
		{
		TBuf<256> errorString;
		errorString.Format(KErrSelectText, iEntryName);
		HBufC* errorStringHBufC = HBufC::NewLC(errorString.Length());
		(errorStringHBufC->Des()).Copy(errorString);

		iTestUtils.WriteComment(*errorStringHBufC);
		CleanupStack::PopAndDestroy();

		TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNotFound);
		}

	CleanupStack::PopAndDestroy(tempSelection);
	}




CSelectEntry::~CSelectEntry()
	{
	delete iEntryName;
	}
/*
CSelectEntry::CSelectEntry(const TMsvId& aCurrentFolderId, TMsvId& aEntryId, CMsvTestUtils& aTestUtils) : iCurrentFolderId(aCurrentFolderId), iTestUtils(aTestUtils), iEntryId(aEntryId)
	{
	}


CSelectEntry* CSelectEntry::NewL(const TDesC& aName, const TMsvId& aCurrentFolderId, TMsvId& aEntryId, CMsvTestUtils& aTestUtils)
	{
	CSelectEntry* self = new (ELeave) CSelectEntry(aCurrentFolderId, aEntryId, aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL(aName);
	CleanupStack::Pop(self);
	return self;
	}

void CSelectEntry::ConstructL(const TDesC& aName)
	{
	iEntryName = HBufC::NewL(aName.Length());
	(*iEntryName) = aName;
	}

void CSelectEntry::StartL(TRequestStatus& aStatus)
	{
	// Select current folder
	CMsvEntry& entry = *(iTestUtils.iMsvEntry);
	entry.SetEntryL(iCurrentFolderId);

	// show the invisible folders.....
	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entry.SetSortTypeL(order);

	CMsvEntrySelection* tempSelection = entry.ChildrenL();
	CleanupStack::PushL(tempSelection);
	
	// Check each child entry for a matching string
	TBool found = EFalse;
	TInt entryIndex = tempSelection->Count();
	while ((!found) && (entryIndex-- != 0))
		{
		entry.SetEntryL((*tempSelection)[entryIndex]);
		if (iEntryName->CompareF(entry.Entry().iDescription) == 0)
			{
			found = ETrue;
			}
		else if (iEntryName->CompareF(entry.Entry().iDetails) == 0)
			{
			found = ETrue;
			}
		}

	if (found)
		{
		iEntryId = entry.Entry().Id();

		TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNone);
		}
	else
		{
		TBuf<256> errorString;
		errorString.Format(KErrSelectText, iEntryName);
		HBufC* errorStringHBufC = HBufC::NewLC(errorString.Length());
		(errorStringHBufC->Des()).Copy(errorString);

		iTestUtils.WriteComment(*errorStringHBufC);
		CleanupStack::PopAndDestroy();

		TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNotFound);
		}

	CleanupStack::PopAndDestroy(tempSelection);
	}




CSelectEntry::~CSelectEntry()
	{
	delete iEntryName;
	}
*/

//
//
// CCheckSelectionCount
//
	
CCheckSelectionCount::CCheckSelectionCount(TInt aCount, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection),
  iTestCount(aCount) 
  	{
	}

void CCheckSelectionCount::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CCheckSelectionCount::StartL(TRequestStatus& aStatus)
	{
	TInt err=KErrNone;
	TInt selectionCount=iSelection.Count();

	LogCommentFormat(_L("[%4d] Folder contains %d entries when %d expected."), iDebugInfo.LineNumber(), selectionCount, iTestCount);

	if (!(selectionCount == iTestCount || selectionCount == iTestCount+1))
		{
		LogCommentFormat(_L("Error!"));
		err = KErrUnknown;
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}


//
//
// CCheckNewFlag
//
	
CCheckNewFlag::CCheckNewFlag(TInt aSelectedIndex, TBool aTestNewStatus, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
:iTestUtils(aTestUtils),
 iSelection(aSelection),
 iSelectedIndex(aSelectedIndex),
 iTestNewStatus(aTestNewStatus) 
	{
	}

void CCheckNewFlag::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CCheckNewFlag::StartL(TRequestStatus& aStatus)
// Check the status flag of the selected entry is the same as the test value
	{
	TInt err=KErrNone;
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);
	TMsvId entryId = iSelection.At(iSelectedIndex);
	msvEntry.SetEntryL(entryId);
	TBool entryIsNew = msvEntry.Entry().New();

	// Check if the Entry is New
	if (entryIsNew) 
		{
		// Check if the entry Should be New
		if (iTestNewStatus)
			{
			// The entry is new
			_LIT(KNewEntry, "Entry %d is New");
			LogCommentFormat(KNewEntry, iSelectedIndex);
			}
		else
			{
			// The Entry is New, but is should be old
			_LIT(KErrNewEntry, "Error, the entry is New.  It should be Old!");
			LogCommentFormat(KErrNewEntry);
			err = KErrUnknown;
			}
			
		}
	else
		{
		// The Entry is Old.  Check if it should be.
		if (!iTestNewStatus)
			{
			// The entry is Old
			_LIT(KOldEntry, "Entry %d is Old");
			LogCommentFormat(KOldEntry, iSelectedIndex);
			}
		else
			{
			// The Entry is Old, but is should be New
			_LIT(KErrOldEntry, "Error, the entry is Old.  It should be New!");
			LogCommentFormat(KErrOldEntry);
			err = KErrUnknown;
			}
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}



//
//
// CCheckPriority
//
	
CCheckPriority::CCheckPriority(TInt aSelectedIndex, TInt aTestPriority, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
:iTestUtils(aTestUtils),
 iSelection(aSelection),
 iSelectedIndex(aSelectedIndex),
 iTestPriority(aTestPriority) 
	{
	}

void CCheckPriority::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CCheckPriority::StartL(TRequestStatus& aStatus)
// Check the Priority flag of the selected entry is the same as the test value
	{
	TInt err=KErrNone;
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);
	TMsvId entryId = iSelection.At(iSelectedIndex);
	msvEntry.SetEntryL(entryId);
	TMsvPriority priority = msvEntry.Entry().Priority();

	// Check if the Priority flag is correct
	if (priority == iTestPriority)
		{
		// Correct Priority
		_LIT(KCorrectPriority, "Entry %d has Priority %d");
		LogCommentFormat(KCorrectPriority, entryId, priority);
		}
	else
		{
		_LIT(KInCorrectPriority, "Error, Entry %d has Priority %d");
		LogCommentFormat(KInCorrectPriority, entryId, priority);
		err = KErrUnknown;
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}



//
//
// CCheckAttachment
//
	
CCheckAttachment::CCheckAttachment(TInt aSelectedIndex, TBool aTestAttachment, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
:iTestUtils(aTestUtils),
 iSelection(aSelection),
 iSelectedIndex(aSelectedIndex),
 iTestAttachment(aTestAttachment) 
	{
	}

void CCheckAttachment::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CCheckAttachment::StartL(TRequestStatus& aStatus)
// Check the attachment flag for this entry is the same as the test value
	{
	TInt err=KErrNone;
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);
	TMsvId entryId = iSelection.At(iSelectedIndex);
	msvEntry.SetEntryL(entryId);
	TBool entryHasAttachment = EFalse;

	// Find out if the Entry has an attachment
	// See if the Entry has a Child Folder Entry
	CMsvEntrySelection* children = msvEntry.ChildrenWithTypeL(KUidMsvFolderEntry);
	CleanupStack::PushL(children);
	if(children->Count() > 0)
		{
		// Find out if the Child Folder Entry has an attachment
		msvEntry.SetEntryL(children->At(0));
		CMsvEntrySelection* grandChildren = msvEntry.ChildrenWithTypeL(KUidMsvAttachmentEntry);
		CleanupStack::PushL(grandChildren);
		if(grandChildren->Count() > 0)
			{
			entryHasAttachment = ETrue;
			}

		CleanupStack::PopAndDestroy(); //grandChildren
		}

	CleanupStack::PopAndDestroy(); //children


	// Check if the Entry has an attachment 
	if (entryHasAttachment)
		{
		// Check if the entry should have an Attachment
		if (iTestAttachment)
			{
			// Entry has an attachment
			_LIT(KAttachment, "Entry %d has an Attachment");
			LogCommentFormat(KAttachment, entryId);
			}
		else
			{
			// There should Not be an attachment, but there is
			_LIT(KErrAttachment, "Error, Entry should NOT have an Attachment!");
			LogCommentFormat(KErrAttachment);
			err = KErrUnknown;
			}
		}
	else
		{
		// Check if the entry should NOT have an Attachment
		if (!iTestAttachment)
			{
			// Entry does NOT have an attachment
			_LIT(KNoAttachment, "Entry %d does not have an Attachment");
			LogCommentFormat(KNoAttachment, entryId);
			}
		else
			{
			// There should be an attachment, but there is NOT
			_LIT(KErrNoAttachment, "Error, Entry should have an Attachment!");
			LogCommentFormat(KErrNoAttachment);
			err = KErrUnknown;
			}
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

//
//
// CCheckChildren
//
	
CCheckChildren::CCheckChildren(TInt aSelectedIndex, TBool aTestAttachment, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
:iTestUtils(aTestUtils),
 iSelection(aSelection),
 iSelectedIndex(aSelectedIndex),
 iTestChildren(aTestAttachment) 
	{
	}

void CCheckChildren::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CCheckChildren::StartL(TRequestStatus& aStatus)	
	{
	TInt err=KErrNone;
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);
	TMsvId entryId = iSelection.At(iSelectedIndex);
	msvEntry.SetEntryL(entryId);
	TBool entryHasChildren = EFalse;

	// Find out if the Entry has children
	entryHasChildren = msvEntry.Count();

	// Check if the Entry has children 
	if (entryHasChildren)
		{
		// Check if the entry should have children
		if (iTestChildren)
			{
			// Entry has children
			_LIT(KChildren, "Entry %d has children");
			LogCommentFormat(KChildren, entryId);
			}
		else
			{
			// Entry should not have children, but there is
			_LIT(KErrChildren, "Error, Entry should NOT have any children!");
			LogCommentFormat(KErrChildren);
			err = KErrUnknown;
			}
		}
	else
		{
		// Check if the entry should NOT have children
		if (!iTestChildren)
			{
			// Entry does NOT have children
			_LIT(KNoChildren, "Entry %d does not have any children");
			LogCommentFormat(KNoChildren, entryId);
			}
		else
			{
			// Entry should have children, but there is NOT
			_LIT(KErrNoChildren, "Error, Entry should have children!");
			LogCommentFormat(KErrNoChildren);
			err = KErrUnknown;
			}
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

//
//
// CCheckBodyText
//
	
CCheckBodyText::CCheckBodyText(TInt aSelectedIndex, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
:iTestUtils(aTestUtils),
 iSelection(aSelection),
 iSelectedIndex(aSelectedIndex) 
	{
	}

CCheckBodyText* CCheckBodyText::NewL(TInt aSelectedIndex, TDesC& aTestBodyText, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils)
	{
	CCheckBodyText* self = new (ELeave) CCheckBodyText(aSelectedIndex, aSelection, aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL(aTestBodyText);
	CleanupStack::Pop(self);
	return self;
	}

void CCheckBodyText::ConstructL(TDesC& aTestBodyText)
	{
	iTestBodyText = HBufC::NewL(200);
	iTestBodyText->Des().Append(aTestBodyText);
	}

CCheckBodyText::~CCheckBodyText()
	{
	delete iTestBodyText;
	}

void CCheckBodyText::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CCheckBodyText::StartL(TRequestStatus& aStatus)
// Check the Body Text of this entry is the same as the test value
	{
	TInt err=KErrNone;
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);
	TMsvId entryId = iSelection.At(iSelectedIndex);
	msvEntry.SetEntryL(entryId);
	
	// Get the ID of the Body Text Entry, by looking at the Child Entries
	TMsvId bodyTextEntryId = KNoBodyText;
	CMsvEntrySelection* children = msvEntry.ChildrenL();
	CleanupStack::PushL(children);
	if(children->Count() > 0)
		{
		// Check if the Child Entry is Body Text or a Folder
		msvEntry.SetEntryL(children->At(0));
		switch (msvEntry.Entry().iType.iUid)
			{
			case KUidMsvEmailTextEntryValue:
				bodyTextEntryId = msvEntry.EntryId();
				break;

			case KUidMsvFolderEntryValue:
				{
				// Check if the Child Folder has a Child Body Text Entry
				CMsvEntrySelection* grandChild = msvEntry.ChildrenWithTypeL(KUidMsvEmailTextEntry);
				CleanupStack::PushL(grandChild);
				if(grandChild->Count() > 0)
					{
					// Get the Id of the Body Text Entry
					msvEntry.SetEntryL(grandChild->At(0));
					bodyTextEntryId = msvEntry.EntryId();
					}

				CleanupStack::PopAndDestroy(); // grandChild
				break;
				}

			default:
				break;
			}
		}

	
	// If there is a Body Text Entry, then check if the Text is correct
	if (bodyTextEntryId == KNoBodyText)
		{
		_LIT(KErrNoBodyText, "Error, there is no Body Text");
		LogCommentFormat(KErrNoBodyText);
		err = KErrUnknown;
		}
	else
		{
		// Set the Entry to be the Body Text
		msvEntry.SetEntryL(bodyTextEntryId);
		if (msvEntry.HasStoreL())
			{
			// Get the store
			CMsvStore* store = msvEntry.ReadStoreL();
			CleanupStack::PushL(store);

			// Get the body Text if it exists
			CParaFormatLayer* paraFormat = CParaFormatLayer::NewL();
			CleanupStack::PushL(paraFormat);
			CCharFormatLayer* charFormat = CCharFormatLayer::NewL();
			CleanupStack::PushL(charFormat);
			CRichText* richText = CRichText::NewL(paraFormat, charFormat);
			CleanupStack::PushL(richText);
			if (store->HasBodyTextL())
				store->RestoreBodyTextL(*richText);

			// Get the Entry Body Text.  Make it the same length as the test Body Text, as the 
			// entry body text will also include the name of the attachmnet if there is one
			HBufC* bodyText = HBufC::NewL(200);
			CleanupStack::PushL(bodyText);
			TPtr bodyTextPtr = bodyText->Des();	
			richText->Extract(bodyTextPtr, 0);
			bodyTextPtr.SetLength(iTestBodyText->Length());

			// Check the Text is Correct
			if (bodyTextPtr.Compare(*iTestBodyText) == 0)
				{
				_LIT(KCorrectBodyText, "Correct Body Text");
				LogCommentFormat(KCorrectBodyText);
				}
			else
				{
				_LIT(KErrWrongBodyText, "Error, Incorrect Body Text");
				LogCommentFormat(KErrWrongBodyText);
				err = KErrUnknown;
				}

			CleanupStack::PopAndDestroy(5);// store, paraFormat, charFormat, richText, bodyText
			}
		else
			{
			_LIT(KErrNoBodyText, "Error, Body Text Entry has No Store");
			LogCommentFormat(KErrNoBodyText);
			err = KErrUnknown;
			}
		}

	CleanupStack::PopAndDestroy(); // children


	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}




//
//
// CCheckRemoteFolderSize
//
	
CCheckRemoteFolderSize::CCheckRemoteFolderSize(TInt aCount, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iTestCount(aCount)
	{
	}

void CCheckRemoteFolderSize::LogCommentFormat(TRefByValue<const TDesC> format,...)
	{
	// Build parameter list.
	VA_LIST list;
	VA_START(list, format);
	TBuf<0x100> buf;
	buf.FormatList(format, list);

	// Log the debug buffer.
	iTestUtils.WriteComment(buf);
	}

void CCheckRemoteFolderSize::StartL(TRequestStatus& aStatus)
	{
	TInt err=KErrNone;

	CMsvEntry& entry = *(iTestUtils.iMsvEntry);
	TMsvEmailEntry message=entry.Entry();
	TInt remotecount=message.RemoteFolderEntries();

	LogCommentFormat(_L("[%4d] Remote folder contains %d entries when %d expected."), iDebugInfo.LineNumber(), remotecount, iTestCount);

	if (remotecount != iTestCount)
		{
		LogCommentFormat(_L("Error!"));
		err = KErrUnknown;
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}


//
//
// CSelectPopulatedInSelection
//

CSelectPopulatedInSelection::CSelectPopulatedInSelection(CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection)
	{
	}

void CSelectPopulatedInSelection::StartL(TRequestStatus& aStatus)
	{
	CMsvEntry& msvEntry = *(iTestUtils.iMsvEntry);

	TInt pos=0;
	while (pos<iSelection.Count())
		{
		msvEntry.SetEntryL(iSelection[pos]);
		TMsvEmailEntry entry(msvEntry.Entry());
		if (entry.BodyTextComplete())
			pos++;
		else
			iSelection.Delete(pos,1);
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}


//
//
// CSelectLastInSelection
//
	
CSelectLastInSelection::CSelectLastInSelection(TInt aCount, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection), 
  iTestCount(aCount)
	{
	}

void CSelectLastInSelection::StartL(TRequestStatus& aStatus)
	{
	TInt selectionCount=iSelection.Count();
	iTestCount=Min(iTestCount,selectionCount);
	iSelection.Delete(iTestCount,iSelection.Count()-iTestCount);
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

//
//
// CSelectFirstInSelection
//
	
CSelectFirstInSelection::CSelectFirstInSelection(TInt aCount, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iSelection(aSelection), 
  iTestCount(aCount)
	{
	}

void CSelectFirstInSelection::StartL(TRequestStatus& aStatus)
	{
	TInt selectionCount=iSelection.Count();
	iTestCount=Min(iTestCount,selectionCount);
	iSelection.Delete(0,iSelection.Count()-iTestCount);
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}

//
//
// CMsvTestEntry
//

CMsvTestEntry::CMsvTestEntry(CMsvClientTest& aParentTestHarness) : iParentTestHarness(aParentTestHarness)
	{
	}

void CMsvTestEntry::StartL(TRequestStatus& aStatus)
	{
	TInt err = KErrNone;
	TBool match = ETrue;

	// Get access to the CMsvEntry owned by the test utils of the parent test harness
	CMsvEntry* cMsvEntry = iParentTestHarness.MsvTestUtils().iMsvEntry;
	
	// Check that there is only one entry currently selected
	if (iParentTestHarness.iCurrentSelection->Count() != 1)
		{
		match = EFalse;
		iParentTestHarness.LogComment(_L("Incorrect number of entries selected"));
		}

	// Set the CMsvEntry to point to the currently selected entry
	// We have already checked that there is exactly one selected entry so the index is safe
	cMsvEntry->SetEntryL((*iParentTestHarness.iCurrentSelection)[0]);

	// Get the TMsvEntry details that we are going to test
	TMsvEntry entry = cMsvEntry->Entry();

	if ((iTestVisible) && match)
		// Test the Visible() flag
		{
		match = (iVisibleValue == entry.Visible());
		if (!match)
			{
			iParentTestHarness.LogComment(_L("Visible() not as expected"));
			}
		}

	if ((iTestComplete) && match)
		// Test the Complete() flag
		{
		match = (iCompleteValue == entry.Complete());
		if (!match)
			{
			iParentTestHarness.LogComment(_L("Complete() not as expected"));
			}
		}

	if (!match)
		// If the entry did not match then log it and complete with an error
		{
		iParentTestHarness.LogComment(_L("Entry not as expected"));
		err = KErrUnknown;
		}

	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

void CMsvTestEntry::TestVisible(TBool aVisibleValue)
	{
	iTestVisible = ETrue; // Specify that the visible flag will be tested
	iVisibleValue = aVisibleValue; // Set the expected value
	}

void CMsvTestEntry::TestComplete(TBool aCompleteValue)
	{
	iTestComplete = ETrue; // Specify that the complete flag will be tested
	iCompleteValue = aCompleteValue; // Set the expected value
	}


//
//
// CSelectEntryById
//

CSelectEntryById::CSelectEntryById(TMsvId aId, TMsvId& aEntryId, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iIdToSelect(aId), 
  iEntryId(aEntryId)
	{
	}

void CSelectEntryById::StartL(TRequestStatus& aStatus)
	{
	iTestUtils.iMsvEntry->SetEntryL(iIdToSelect);
	iEntryId = iIdToSelect;
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}




//
//
// CSelectAllMessages
//

CSelectAllMessages::CSelectAllMessages(TMsvId& aParentFolder, CMsvEntrySelection& aSelection, CMsvTestUtils& aTestUtils) 
: iTestUtils(aTestUtils),
  iParentFolder(aParentFolder), 
  iSelection(aSelection)
	{
	}

void CSelectAllMessages::StartL(TRequestStatus& aStatus)
	{
	iSelection.Reset();
	iTestUtils.iMsvEntry->SetEntryL(iParentFolder);

	CMsvEntrySelection* tempSelection = iTestUtils.iMsvEntry->ChildrenL();
	CleanupStack::PushL(tempSelection);
	// Copy the child entries to the given selection
	TInt index = tempSelection->Count();
	while (index--)
		{
		iSelection.AppendL((*tempSelection)[index]);
		}
	CleanupStack::PopAndDestroy(tempSelection);
	
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}





//
//
// CLongWait
//
CLongWait::CLongWait() 
: CActive(EPriorityNormal),
  iWaitTime(KDefaultWaitTime)
	{
	CActiveScheduler::Add(this);
	}

CLongWait::CLongWait(TInt aWaitTime) 
: CActive(EPriorityNormal)
	{
	iWaitTime = aWaitTime * 1000000;
	CActiveScheduler::Add(this);
	}

CLongWait::~CLongWait()
	{
	delete iTimer;
	}

void CLongWait::StartL(TRequestStatus& aStatus)
	{
	delete iTimer;
	iTimer = 0;
	iTimer = CTestTimer::NewL();
	iReportStatus = &aStatus;
	iTimer->AfterReq(iWaitTime, iStatus);
	aStatus = KRequestPending;
	SetActive();
	}



void CLongWait::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CLongWait::DoCancel()
	{
	}









//
//
// CCreateEmailMessage
//

CCreateEmailMessage::CCreateEmailMessage(const TMsvId& aFolderId, const TMsvId& aServiceId, const TDesC& aFileName, CEmailTestUtils& aTestUtils,TInt aHowMany) : CActive(EPriorityNormal), iCurrentFolderId(aFolderId), iServiceId(aServiceId), iTestUtils(aTestUtils), iHowMany(aHowMany)
	{
	CActiveScheduler::Add(this);
	iFileName = aFileName;
	}

void CCreateEmailMessage::StartL(TRequestStatus& aStatus)
	{
	// Close down the client side session and wait a few seconds
	iTestUtils.Reset();
	delete iTimer;
	iTimer = 0;
	iTimer = CTestTimer::NewL();
	iReportStatus = &aStatus;
	iTimer->AfterReq(5000000, iStatus);
	aStatus = KRequestPending;
	iState = ECreateEmailWait1;
	SetActive();
	}

void CCreateEmailMessage::RunL()
	{
	TInt i;
	switch (iState)
		{
		case ECreateEmailWait1:
			iTestUtils.GoServerSideL();
			// We should be on the server side now, so we can create the message
			for (i=0; i<iHowMany; i++)
				iTestUtils.CreateMessageL(iFileName, iServiceId, iCurrentFolderId);

			// Now go back to the client side and wait a few seconds.
			iTestUtils.GoClientSideL();
			iTimer->AfterReq(5000000, iStatus);
			iState = ECreateEmailWait2;
			SetActive();
			break;
		case ECreateEmailWait2:
			// We should be safely back on the client side so we can continue.
			User::RequestComplete(iReportStatus, iStatus.Int());
			break;
		};
	}

void CCreateEmailMessage::DoCancel()
	{
	}

CCreateEmailMessage::~CCreateEmailMessage()
	{
	if (iTimer)
		{
		iTimer->Cancel();
		}
	delete iTimer;
	}

//
//
// CCopySelection
//

CCopySelection::CCopySelection(CMsvClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CCopySelection::StartL(TRequestStatus& aStatus)
	{
	iParentTestHarness.MsvTestUtils().iMsvEntry->SetEntryL(iParentTestHarness.iSelectedFolder);
	iReportStatus = &aStatus;
	iParentTestHarness.SetCurrentOperation(iParentTestHarness.MsvTestUtils().iMsvEntry->CopyL(*(iParentTestHarness.iCurrentSelection), iParentTestHarness.iDestinationFolder, iStatus));
	aStatus = KRequestPending;
	SetActive();
	}

void CCopySelection::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CCopySelection::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CCopySelection::~CCopySelection()
	{
	}



//
//
// CMoveSelection
//

CMoveSelection::CMoveSelection(CMsvClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CMoveSelection::StartL(TRequestStatus& aStatus)
	{
	iParentTestHarness.MsvTestUtils().iMsvEntry->SetEntryL(iParentTestHarness.iSelectedFolder);
	iReportStatus = &aStatus;
	iParentTestHarness.SetCurrentOperation(iParentTestHarness.MsvTestUtils().iMsvEntry->MoveL(*(iParentTestHarness.iCurrentSelection), iParentTestHarness.iDestinationFolder, iStatus));
	aStatus = KRequestPending;
	SetActive();
	}
void CMoveSelection::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CMoveSelection::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CMoveSelection::~CMoveSelection()
	{
	}


//
//
// CDeleteSelection
//

CDeleteSelection::CDeleteSelection(CMsvClientTest& aParentTestHarness) : CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness)
	{
	CActiveScheduler::Add(this);
	}

void CDeleteSelection::StartL(TRequestStatus& aStatus)
	{
	if (iParentTestHarness.iCurrentSelection->Count() == 0)
		// If there are no entries currently selected then complete without an error
		{
		TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNone);
		}
	else
		// If there are some selected messages then delete them.
		{
		CMsvEntry& sharedCMsvEntry = *(iParentTestHarness.MsvTestUtils().iMsvEntry);

		// Set the CMsvEntry to point to the folder selected by the parent test harness
		sharedCMsvEntry.SetEntryL(iParentTestHarness.iSelectedFolder);

		iReportStatus = &aStatus;

		// Start the delete operation
		CMsvOperation* deleteOperaiton= sharedCMsvEntry.DeleteL(*(iParentTestHarness.iCurrentSelection), iStatus);

		// Set the current delete operation to be the current operation on the parent test harness
		iParentTestHarness.SetCurrentOperation(deleteOperaiton);
		aStatus = KRequestPending;
		SetActive();
		}
	}

void CDeleteSelection::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CDeleteSelection::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}


CDeleteSelection::~CDeleteSelection()
	{
	}


//
//
// CDeleteChildren
//

CDeleteChildren::CDeleteChildren(CMsvClientTest& aParentTestHarness, TInt aSelectedIndex) 
: CActive(EPriorityNormal), iParentTestHarness(aParentTestHarness), iSelectedIndex(aSelectedIndex)
	{
	CActiveScheduler::Add(this);
	}

void CDeleteChildren::StartL(TRequestStatus& aStatus)
// Deletes the children of this entry 	
	{
	CMsvEntry& msvEntry = *(iParentTestHarness.MsvTestUtils().iMsvEntry);
	CMsvEntrySelection& selection = *(iParentTestHarness.iCurrentSelection);
	TMsvId entryId = selection.At(iSelectedIndex);
	msvEntry.SetEntryL(entryId);

	iReportStatus = &aStatus;

	selection.Reset();
	CMsvEntrySelection* tempSelection = msvEntry.ChildrenL();
	CleanupStack::PushL(tempSelection);
	// Copy the child entries to the given selection
	TInt index = tempSelection->Count();
	while (index--)
		{
		selection.AppendL((*tempSelection)[index]);
		}
	CleanupStack::PopAndDestroy(tempSelection);

	if (selection.Count() == 0)
		// If the entry has no children then complete without an error
		{
		TRequestStatus* status = &aStatus;
		User::RequestComplete(status, KErrNone);
		}
	else
		// If the entry has children then delete them.
		{
		// Start the delete operation
		CMsvOperation* deleteOperaiton= msvEntry.DeleteL(selection, iStatus);

		// Set the current delete operation to be the current operation on the parent test harness
		iParentTestHarness.SetCurrentOperation(deleteOperaiton);
		aStatus = KRequestPending;
		SetActive();
		}
	}

void CDeleteChildren::RunL()
	{
	User::RequestComplete(iReportStatus, iStatus.Int());
	}

void CDeleteChildren::DoCancel()
	{
	iParentTestHarness.CurrentOperation().Cancel();
	}

CDeleteChildren::~CDeleteChildren()
	{
	}

