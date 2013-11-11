// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// [TestStep Name]
// ComparePopEmailMsgs
// [Paramaters]
// PopAccountName		: 		Name of the POP account
// Compares the recieved mails with the expected numbers of lines to test POP3 TOP command
// 
//
/**
 @file
*/

// User includes
#include "CExpMailInfo.h"
#include "T_ComparePopEmailMsgs.h"
#include <t_utils.h>
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>


// Epoc includes
#include <imcm.rsg>
#include <miutmsg.h>
#include <mmsvattachmentmanager.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#include "timrfc822datefield.h"
#endif

// Literals Used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KParagraphDelimiter,"\x2029\x2029");

const TInt KUnicodeValue = 0x0046;

/**
ConstructL()
Used to ensure that MMsvAttachmentManager is not destroyed until we want it to be.
To do this we need to preserve certain objects of CImEmailMessage, which if disappeared
would invalidate MMsvAttachmentManager. But this should be deleted in a controlled manner.

@param aSession
An object of type CMsvSession object.

@param aEntry
An object of type TMsvEntry used to get the entry ID. 
*/
void CAttachmentItem::ConstructL(CMsvSession& aSession, TMsvEntry aEntry)
	{
	iMsvEntry = aSession.GetEntryL(aEntry.Id());

	iMsvEntry->SetEntryL(aEntry.Id());
	iEmailMsg = CImEmailMessage::NewL(*iMsvEntry);
		
	CMsvOperationActiveSchedulerWait* waiter = CMsvOperationActiveSchedulerWait::NewLC();
 	iEmailMsg->GetAttachmentsListL(waiter->iStatus, aEntry.Id(),
								   CImEmailMessage::EAllAttachments, 
								   CImEmailMessage::EThisMessageOnly);
	waiter->Start();

	CleanupStack::PopAndDestroy(1, waiter); //waiter		
	}


/**
NewL()
Allocates and creates a new CAttachmentItem object

@param aSession
An object of type CMsvSession object.

@param aEntry
An object of type TMsvEntry

@leave KErrNoMemory

@return 
A pointer to an object of type CAttachmentItem

*/
CAttachmentItem* CAttachmentItem::NewL(CMsvSession& aSession, TMsvEntry aEntry)
	{
	CAttachmentItem* self=CAttachmentItem::NewLC(aSession, aEntry);
	CleanupStack::Pop(self);

	return self;
	}


/**
NewLC()
Static factory constructor. Uses two phase construction and leaves nothing on the CleanupStack.


@param aSession
An object of type CMsvSession object.

@param aEntry
An object of type TMsvEntry

@leave KErrNoMemory
@return
A pointer to the newly created CAttachmentItem object.

*/
CAttachmentItem* CAttachmentItem::NewLC(CMsvSession& aSession, TMsvEntry aEntry)
	{
	CAttachmentItem* self=new (ELeave) CAttachmentItem;
	CleanupStack::PushL(self);
	
	self->ConstructL(aSession, aEntry);
	
	return self;
	}


/**
~CAttachmentItem()
Destroy the data members.
*/
CAttachmentItem::~CAttachmentItem()
	{
	delete iEmailMsg;
	iEmailMsg=NULL;

	delete iMsvEntry;
	iMsvEntry=NULL;
	}


/**
MMsvAttachmentManager()

@return
An object of MMsvAttachmentManager
*/
MMsvAttachmentManager& CAttachmentItem::MMsvAttachmentManager() const
	{			
	return (iEmailMsg->AttachmentManager());		
	}


/**
GetExpPop3MailInfoLC()
Sets the CExpPop3MailInfo attributes reading from the INI file

@param  aIndx
Index in the selection list.

@leave KErrNotReady

@return
A pointer to an object of type CExpPop3MailInfo
*/
CExpPop3MailInfo* CT_MsgComparePopEmailMsgs::GetExpPop3MailInfoLC(TInt aIndx)
	{
	CExpPop3MailInfo* expMailInfo  = new (ELeave) CExpPop3MailInfo;
	CleanupStack::PushL(expMailInfo);
	
	TInt comInt = -1;
	_LIT(KNumOfLines,"NumOfLines%d");
	TBuf<256> param;
	param.Format(KNumOfLines,aIndx);
	
	if (!GetIntFromConfig(ConfigSection(), param, comInt))
		{
		ERR_PRINTF1(_L("No Input for NumOfLines"));
		User::Leave(KErrNotReady);
		}
	
	expMailInfo->SetNumLinesBodyText(comInt);
	comInt = -1;
	
	_LIT(KFooterExpected, "FooterExpected%d");
	param.Format(KFooterExpected,aIndx);
	if (!GetIntFromConfig(ConfigSection(), param, comInt))
		{
		ERR_PRINTF1(_L("No Input for Footer Expected"));
		User::Leave(KErrNotReady);
		}
	expMailInfo->SetFooterExpected(comInt);
	
	comInt = -1;
	_LIT(KNumOfLinesHtml, "NumOfLinesHtml%d");
	param.Format(KNumOfLinesHtml, aIndx);
	if (!GetIntFromConfig(ConfigSection(), param, comInt))
		{
		ERR_PRINTF1(_L("No Input For NumOfLinesHtml"));
		User::Leave(KErrNotReady);
		}
	expMailInfo->SetNumLinesHtml(comInt);
	
	TPtrC ptrDes;
	_LIT(KDescription, "Description%d");
	param.Format(KDescription, aIndx);
	if(!GetStringFromConfig(ConfigSection(), param, ptrDes))
		{
		ERR_PRINTF1(_L("No Input for Description"));
		User::Leave(KErrNotReady);
		}
	expMailInfo->SetDescription(ptrDes);
		
	_LIT(KNumOfAttachments, "NumOfAttachments%d");
	param.Format(KNumOfAttachments, aIndx);
	if (!GetIntFromConfig(ConfigSection(), param, comInt))
		{
		ERR_PRINTF1(_L("No Input for NumOfAttachments"));
		User::Leave(KErrNotReady);
		}
	expMailInfo->SetNumAttachments(comInt);
	
	comInt = -1;
	if(expMailInfo->GetFooterExpected())
		{
		_LIT(KFooterSize, "FooterSize%d");
		param.Format(KFooterSize, aIndx);
		if (!GetIntFromConfig(ConfigSection(), param, comInt))
			{
			ERR_PRINTF1(_L("No Input for FooterSize"));
			User::Leave(KErrNotReady);
			}
		expMailInfo->SetFooterSize(comInt);
		}
	else
		{
		expMailInfo->SetFooterSize(0);
		}
	
	return expMailInfo;
	}



/**
LoadParametersL()
Loads the test parameters 

@leave KErrNotReady
@return
ETrue if parameters loaded, Efalse on failure.
*/
TBool CT_MsgComparePopEmailMsgs::LoadParametersL()
	{
	TPtrC sentFolder;
	_LIT(KSentFolder, "SentFolder");
	if(!GetStringFromConfig(ConfigSection(), KSentFolder, sentFolder))
		{
		ERR_PRINTF1(_L("No Input for Sent Folder"));
		User::Leave(KErrNotReady);
		}
			
	iSentSelectionList = CT_MsgUtils ::CreateChildrenSelectionL(iSharedDataPOP.iSession, CT_MsgUtilsEnumConverter::FindFolderIdByName(sentFolder));
	if(! iSentSelectionList)
		{
		ERR_PRINTF1(_L("Invalid sentSelectionList"));
		SetTestStepResult(EFail);
		return EFalse;
		}
	
	TPtrC inboxFolder;
	_LIT(KInboxFolder, "InboxFolder");
	if(!GetStringFromConfig(ConfigSection(), KInboxFolder, inboxFolder))
		{
		ERR_PRINTF1(_L("No Input for Inbox Folder"));
		User::Leave(KErrNotReady);
		}
	
	
	TPtrC popAccountName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("Account name not specified"));
		SetTestStepResult(EFail);
		}
	if (CT_MsgUtilsEnumConverter::FindFolderIdByName(inboxFolder) == KMsvLocalServiceIndexEntryId)
		{
		// We are expecting to get a selection of remote Folder(which is the serviceId)
		TMsvId popServiceId = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)popAccountName);
		
		CMsvEntry* entry = iSharedDataPOP.iSession->GetEntryL(popServiceId);
		CleanupStack::PushL(entry);
		
		entry->SetEntryL(popServiceId); // Setting the context to remote folder.
		
		iInboxSelectionList = CT_MsgUtils ::CreateChildrenSelectionL(iSharedDataPOP.iSession, popServiceId);
		
		CleanupStack::PopAndDestroy(entry);
		}
	else
		{
		iInboxSelectionList = CT_MsgUtils ::CreateChildrenSelectionL(iSharedDataPOP.iSession, CT_MsgUtilsEnumConverter::FindFolderIdByName(inboxFolder));
		}

	if(! iInboxSelectionList)
		{
		ERR_PRINTF1(_L("Invalid inboxSelectionList"));
		SetTestStepResult(EFail);
		return EFalse;
		}
	//Starting the comparision
	if(iInboxSelectionList->Count() != iSentSelectionList->Count())
		{
		ERR_PRINTF3(_L("iInboxSelectionList->Count(%d) != iSentSelectionList->Count(%d)"),
								 iInboxSelectionList->Count(), iSentSelectionList->Count());
		SetTestStepResult(EFail);
		return EFalse;
		}

	TInt comInt = -1;
	_LIT(KNumTest, "NumTest");
	if (!GetIntFromConfig(ConfigSection(), KNumTest, comInt))
		{
		ERR_PRINTF1(_L("No Input for NumTests"));
		User::Leave(KErrNotReady);
		}
	TInt numTests = comInt;
	if(numTests != iInboxSelectionList->Count())
		{
		ERR_PRINTF1(_L("iNumTests != iInboxSelectionList"));
		SetTestStepResult(EFail);
		return EFalse;
		}
		
	iExpResults.Reset();
	for(TInt i = 0;i < numTests; i++) 
		{
		
		CExpPop3MailInfo* pExp = GetExpPop3MailInfoLC(i);
		if(! pExp)
			{
			ERR_PRINTF1(_L("Invalid expected mail Info"));
			SetTestStepResult(EFail);
			return EFalse;
			}
		else
			{
			CleanupStack::Pop(pExp); // pExp			
			iExpResults.Append(pExp);
			}
		}
	return ETrue;
	}


/**
DoCompareL()
Comparison of 2 email messages identified by TMsvEntry

@param  aRecvEntry
The recieved email identifier.

@param  aMailInfo
Expected state of the recieved email.

@param  aSentEntry
Identifier of the original sent email foir comparison purposes.

@return
ETrue if emails match EFalse otherwise
*/
TBool CT_MsgComparePopEmailMsgs::DoCompareL(TMsvEntry& aRecvEntry, CExpPop3MailInfo& aMailInfo, TMsvEntry& aSentEntry)
	{
	TBool ret = ETrue;
	TBool footerExists = EFalse;
	TInt footersize = 0;
		
			
	TInt nLines = GetMessageLinesL(aRecvEntry, KUidMsvEmailTextEntry, footerExists, footersize);
	if(nLines != aMailInfo.GetNumLinesBodyText())
		{
		ERR_PRINTF3(_L("Lines of body text wrong actual = %d expected = %d!"), nLines, aMailInfo.GetNumLinesBodyText());
		ret = EFalse;
		}
	nLines = GetMessageLinesL(aRecvEntry, KUidMsvEmailHtmlEntry, footerExists, footersize);

	if(nLines != aMailInfo.GetNumLinesHtml())
		{		
		ERR_PRINTF3(_L("HTML size wrong actual = %d expected = %d!"), nLines, aMailInfo.GetNumLinesHtml());
		ret = EFalse;
		}

	else if(! CheckAttachmentsL(aRecvEntry, aMailInfo, aSentEntry))
		{
		ERR_PRINTF1(_L("Attachment comparison failed !"));
		ret = EFalse;
		}

	else if(! footerExists && aMailInfo.GetFooterExpected())
		{
		ERR_PRINTF1(_L("An expected footer was missing !"));
		ret = EFalse;
		}
	else if(footerExists && ! aMailInfo.GetFooterExpected())
		{
		ERR_PRINTF1(_L("An unexpected footer was found !"));
		ret = EFalse;
		}
	else if(aMailInfo.GetFooterExpected() && aMailInfo.GetFooterSize()!=0 
			&& aMailInfo.GetFooterSize() != footersize)
		{
		ERR_PRINTF3(_L("KB left on server size wrong = %d expected = %d!"), footersize, aMailInfo.GetFooterSize());
		ret = EFalse;			
		}
	return ret;
	}




/**
CheckAttachmentsL()
Compares attachments for the email messages aRecvEntry and aSentEntry using
expected results in aMailInfo

@param aRecvEntry
An identifier for the recieved email

@param aMailInfo
Expected state of recieved email attachments

@param aSentEntry
An identifier for the original email

@return
ETrue if attachments match otherwise EFalse
*/
TBool CT_MsgComparePopEmailMsgs::CheckAttachmentsL(TMsvEntry& aRecvEntry, CExpPop3MailInfo& aMailInfo, TMsvEntry& aSentEntry)
	{
	TBool ret = ETrue;
	CAttachmentItem* recvManager = CAttachmentItem::NewLC(*iSharedDataPOP.iSession, aRecvEntry);
	CAttachmentItem* sentManager = CAttachmentItem::NewLC(*iSharedDataPOP.iSession, aSentEntry);

	TInt recvCount = recvManager->MMsvAttachmentManager().AttachmentCount();	
	TInt sentCount = sentManager->MMsvAttachmentManager().AttachmentCount();

	TFileName recvFileName;
	TFileName sentFileName;

	if(recvCount == aMailInfo.GetNumAttachments())
		{
		for(TInt i=0;i<recvCount;i++)
			{
			for(TInt j=0;j<sentCount;j++)
				{
				CMsvAttachment* recvAttInfo = recvManager->MMsvAttachmentManager().GetAttachmentInfoL(i);
				CleanupStack::PushL(recvAttInfo);

				CMsvAttachment* sentAttInfo = sentManager->MMsvAttachmentManager().GetAttachmentInfoL(j);
				CleanupStack::PushL(sentAttInfo);

				if(recvAttInfo->AttachmentName().Compare(sentAttInfo->AttachmentName()) == 0)
					{
					recvFileName = recvAttInfo->FilePath();
					sentFileName = sentAttInfo->FilePath();
					ret = CompareFilesL(recvFileName, sentFileName);
					}
				CleanupStack::PopAndDestroy(2, recvAttInfo);		
				}
			}
		}
	else
		{
		INFO_PRINTF3(_L("Warning - Expected attachments mismatch recieved (%d) expected (%d)"),
								 recvCount, aMailInfo.GetNumAttachments());
		ret = EFalse;
		}
	CleanupStack::PopAndDestroy(2, recvManager); // recvManager,sentManager
	return ret;
	}



/**
CompareFilesL()
Performs byte for byte copmparison of the files indicated by aFileName1 and aFileName2

@param aFileName1
Filename of first file to be compared

@param aFileName2
Filename of second file to be compared

@return
ETrue if files match, EFalse otherwise
*/
TBool CT_MsgComparePopEmailMsgs::CompareFilesL(TFileName& aFileName1, TFileName& aFileName2)
	{
	TBool ret = ETrue;
	RFile file1, file2;

	User::LeaveIfError(file1.Open(iFs, aFileName1, EFileRead));
	User::LeaveIfError(file2.Open(iFs, aFileName2, EFileRead));

	TInt sz1, sz2;
	User::LeaveIfError(file1.Size(sz1));
	User::LeaveIfError(file2.Size(sz2));
	
	if(sz1 != sz2)
		{
		ERR_PRINTF3(_L("File size mismatch 1(%d)   2(%d)"), sz1, sz2);
		ret = EFalse;
		}
	else
		{
		TBuf8<1> char1;
		TBuf8<1> char2;
		TInt len1, len2;
		do
			{
			file1.Read(char1);
			file2.Read(char2);
			len1 = char1.Length();
			len2 = char2.Length();
			if(char1.Compare(char2) != 0)
				{
				ret = EFalse;
				break;
				}
			}
		while(len1 && len2);
		}
	file1.Close();
	file2.Close();
	return ret;
	}


/**
GetMessageLinesL()
Counts the number of lines in a message's HTML or body text

@param aEntry
Email message identifier

@param aUid
Request identifier KUidMsvEmailHtmlEntry or KUidMsvEmailTextEntry at present

@param aFooterExists
Reference to a boolean variable only used when aUid == KUidMsvEmailTextEntry

@param aFooterSize
Expected KB left on server inserted into footer string if present 

@return
Returns the number of lines in the requested message part
*/
TInt CT_MsgComparePopEmailMsgs::GetMessageLinesL(TMsvEntry& aEntry, TUid aUid, TBool& aFooterExists, TInt &aFooterSize)
	{
	TInt lines = 0; 
	
	TMsvId msgPartIndex = GetMessagePartIndexL(aEntry,aUid);
	
	if(msgPartIndex)
		{
		CMsvEntry* msvEntry = iSharedDataPOP.iSession->GetEntryL(msgPartIndex);
		CleanupStack::PushL(msvEntry);
	
		msvEntry->SetEntryL(msgPartIndex);
		if(aUid == KUidMsvEmailTextEntry)
			{
			lines = CountLinesOfBodyTextL(*msvEntry, aFooterExists, aFooterSize);
			}
		else if(aUid == KUidMsvEmailHtmlEntry)
			{
			lines = CountLinesOfHtmlL(*msvEntry);
			}
		CleanupStack::PopAndDestroy(msvEntry);
		}
	return lines;
	}


/**
CountLinesOfBodyTextL()
Counts the lines of body text in the email aEntry

@param aEntry
A reference to an object representing the email

@param aFooterExists
Reference to a boolean variable - set to ETrue if the footer string is found in the body text

@param aFooterSize
Expected KB left on server inserted into footer string if present 

@return
Number of lines in the body text 
*/
TInt CT_MsgComparePopEmailMsgs::CountLinesOfBodyTextL(CMsvEntry& aEntry, TBool& aFooterExists, TInt& aFooterSize)
	{
	TInt lines = 0;
	TInt count =0;
	aFooterExists=EFalse;
	aFooterSize=0;
	aEntry.SetEntryL(aEntry.EntryId());
	if(aEntry.HasStoreL())
		{
		CMsvStore*	msvStore1= aEntry.ReadStoreL();
		CleanupStack::PushL(msvStore1);
		CParaFormatLayer*	paraFormatLayer1 = CParaFormatLayer::NewL();
		CleanupStack::PushL(paraFormatLayer1);
		CCharFormatLayer*	charFormatLayer1 = CCharFormatLayer::NewL(); 
		CleanupStack::PushL(charFormatLayer1);
		CRichText*	bodyText1=CRichText::NewL(paraFormatLayer1, charFormatLayer1, CEditableText::EFlatStorage, 256);
		CleanupStack::PushL(bodyText1);

		bodyText1->Reset();
		if (msvStore1->HasBodyTextL())
			{
			msvStore1->RestoreBodyTextL(*bodyText1);
			TUint16 val = CEditableText::ELineBreak;
			TUint16 val1 = CEditableText::EParagraphDelimiter;
			TUint16 val2 = KUnicodeValue;
			
			TInt n = 0;
			TInt pos = 0;
			for(;;)
				{
				TPtrC bodyText = bodyText1->Read(pos);
				n = bodyText.Find(&val, 1);
								
				// if iStore8BitData flag is set, the line is terminated by "CEditableText::EParagraphDelimiter"			
				if(msvStore1->IsPresentL(KMsvPlainBodyText8))
					{
					if ( 0 == count )
						{
						TPtrC buffer = bodyText.Mid(0,n+2);
						// eg for 8bit the body look as : This is a simple email message.\x2028\x2029\x2029
						// eg for 16bit the body look as: This is a simple email message.\x2028\x2028\x2029
						if((bodyText.Right(2).Compare(KParagraphDelimiter)==KErrNone) && \
									 buffer.Find(&val2,1) != 75)
							{
							lines++;	
							count++;
							}				
							// Increment the line if EParagraphDelimiter or 0x0046 is found sequence as eg:1. \x2028\x2029
							// 2. \x2028\x2029\x2028\x2029\x0046 3. \x2028\x2029\x2028\x2029\x2028\x2029
						else if ( (buffer.Find(&val1,1)==0 && n==-1) || (buffer.Find(&val2,1)==1) \
										|| (buffer.Find(&val1,1)>0) )
							{
							lines++;		
							}
						}
					}
				if(n < 0)
					break;
				lines++;
				pos += n+1;
								
				//This Check is needed to delete the extra line introduced by communigate Pro Server
				TInt fpos = CheckIfServerMessageExists(bodyText);
				if (fpos != KErrNotFound)
					{
					lines--;
					}
				}
			TPtrC	pBt = bodyText1->Read(0);
			aFooterExists = CheckIfFooterMessageExistsL(pBt , aFooterSize);
			}		
		CleanupStack::PopAndDestroy(4, msvStore1);  //bodyText1,charFormatLayer1,paraFormatLayer1,msvStore1.
		}
	return lines;
	}


/**
CheckIfServerMessageExists()
Checks if any server message has been added to the Body text returns the 
position of the server message in the buffer and the length of the server
message.This function can be enhanced to check for any other server message encountered.
If not found, returns KErrNotFound.

@param aBodyText
The body text of the message

@return
Position of the message, if not found returns KErrNotFound
*/
 TInt CT_MsgComparePopEmailMsgs::CheckIfServerMessageExists(TPtrC aBodyText)
	{
	_LIT(KServerMessage,"*This message was transferred with a trial version of CommuniGate(tm) Pro*");

	TInt pos = aBodyText.FindC(KServerMessage);
	
	return pos;
	}


/**
CountLinesOfHtmlL()
Counts the number of lines of HTML

@param aEntry
A reference to an object representing the email

@return
Number of lines of HTML
*/
TInt CT_MsgComparePopEmailMsgs::CountLinesOfHtmlL(CMsvEntry& aEntry)
	{ 
	TInt lines = 0;
	aEntry.SetEntryL(aEntry.EntryId());
	
	
	CMsvStore* store = aEntry.ReadStoreL();
	CleanupStack::PushL(store);
	
	MMsvAttachmentManager& attManager = store->AttachmentManagerL();
	
	RFile htmlFile = attManager.GetAttachmentFileL(0);
	CleanupClosePushL(htmlFile);
	
	_LIT8(KFindData, "\r\n");
	TInt htmlSize = 0;
	User::LeaveIfError(htmlFile.Size(htmlSize));
	HBufC8* fBuf = HBufC8::NewLC(htmlSize);
	TPtr8 p = fBuf->Des();
	htmlFile.Read(p);
	TInt pos = 0;
	for(;;)
		{
		pos = p.Find(KFindData);
		if(pos < 0)
			{
			break;
			}
		p = p.Mid(pos+2);
		lines++;
		}
		
	CleanupStack::PopAndDestroy(fBuf);
	CleanupStack::PopAndDestroy(); // htmlFile
	CleanupStack::PopAndDestroy(store);
	
	return lines;
	}



/**
GetMessagePartIndexL()
Retrieves the part index id for the email identified by aEntry

@param aEntry
A valid email identifier

@param aUid
An identifier tfor the type of message part to be retrieved

@return
TMsvId - message part identifier
*/
TMsvId CT_MsgComparePopEmailMsgs::GetMessagePartIndexL(TMsvEntry& aEntry, TUid aUid)
	{
	static TBool msgPartFound = EFalse;	
	TMsvId msgPartId = 0;
	CMsvEntry*	baseEntry  = iSharedDataPOP.iSession->GetEntryL(aEntry.Id());
	CleanupStack::PushL(baseEntry);
	CMsvEntrySelection*	selection =  baseEntry->ChildrenL();
	CleanupStack::PushL(selection);
	TInt count = selection->Count();
	TInt ind1;
	CMsvEntry* entry = iSharedDataPOP.iSession->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(entry);
	for (ind1=0, msgPartFound=EFalse; ind1 < count; ind1++)
		{
		if(msgPartFound)
			{
			break;
			}		
		TMsvId	childId  = selection->At(ind1);
		entry->SetEntryL(childId);

		if (entry->Entry().iType  == aUid)
			{
			msgPartId = childId;
			break;
			}
		else if (entry->Entry().iType  == KUidMsvFolderEntry)
			{
			TMsvEntry ent = entry->Entry();
			msgPartId = GetMessagePartIndexL(ent, aUid);
			}
		}
	CleanupStack::PopAndDestroy(3, baseEntry);  // entry,selection,baseEntry
	return msgPartId;
	}




/**
CheckIfFooterMessageExistsL()
Checks the email for the presence of the footer string

@param aRecvBodyText
Reference to a descriptor holding the body text

@param aFooterSize
The expected number of KB remaining on the server which has been inserted into the footer string

@return
ETrue if footer string found otherwise EFalse 
*/
TBool CT_MsgComparePopEmailMsgs::CheckIfFooterMessageExistsL(TPtrC& aRecvBodyText , TInt& aFooterSize)
	{
	RResourceFile resFile;
	CT_MsgUtils ::OpenResourceFileL(resFile, iFs, KImEngineResourceFile);	// NB leaves if file not found
	TCleanupItem close(CT_MsgUtils ::CloseResourceFile, &resFile);
	CleanupStack::PushL(close);
	
	HBufC8* buf = NULL;
	buf = resFile.AllocReadLC(PARTIAL_DOWNLOAD_FOOTER_MESSAGE);
	TResourceReader reader;
	reader.SetBuffer(buf);

	HBufC* resourceBuf = (reader.ReadTPtrC()).AllocL();
	CleanupStack::PushL(resourceBuf);

	TInt len = resourceBuf->Find(_L("%d"));
	const TInt KBufLen = 256;
	TBuf<KBufLen> findBuf;

	if(len == KErrNotFound)
		{
		len = resourceBuf->Length();
		}

	if(len > KBufLen)
		{
		len = KBufLen;
		}
	
	findBuf.Copy(resourceBuf->Ptr(), len);
	len = aRecvBodyText.Find(findBuf);

	if(len>=0)
		{
		TPtrC rest=aRecvBodyText.Right(aRecvBodyText.Length()-len-findBuf.Length());
		TLex lex(rest.Left(rest.Locate(TChar(' '))));
		lex.Val(aFooterSize);
		}
	else
		{
		aFooterSize=0;
		}

	CT_MsgUtils ::CloseResourceFile(&resFile);
	CleanupStack::PopAndDestroy(3); // buf, resourceBuf, close
	return (len != KErrNotFound);
	}

//const TInt KGranuality = 5;

/**
CT_MsgComparePopEmailMsgs()
Constructor

@param  aSharedDataPOP
Data shared across the POP test steps. Ex. SessionF
*/
CT_MsgComparePopEmailMsgs::CT_MsgComparePopEmailMsgs(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgAsyncStepPOP(aSharedDataPOP)  
	{
	iSentSelectionList = NULL;
	iInboxSelectionList = NULL;
	SetTestStepName(KComparePopEmailMsgs);
	}


/**
NewL()
Allocates and creates a new CT_MsgComparePopEmailMsgs object

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session

@leave KErrNoMemory
@return
Returns pointer to an object of CT_MsgComparePopEmailMsgs.
*/
CT_MsgComparePopEmailMsgs* CT_MsgComparePopEmailMsgs::NewL(CT_MsgSharedDataPop& aSharedDataPOP)
	{
	CT_MsgComparePopEmailMsgs* self = new(ELeave) CT_MsgComparePopEmailMsgs(aSharedDataPOP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}		


/**
ProgressL()

*/

//
//
//	I have no idea what this code is doing.
//	It looks very messy.
//	Someone is going to have to reowrk this code so that we can cancel and do progress
//
//
void CT_MsgComparePopEmailMsgs::ProgressL(TBool bFinal)
	{
	//	TODO
	if (bFinal)
		{
		//	Display final progress
		}
	else
		{
		//	Display current progress
		}
	}


/**
CancelStep()

*/
void CT_MsgComparePopEmailMsgs::CancelStep()
	{
	//	TODO
	}


/**
~CT_MsgComparePopEmailMsgs()
Deletes all the data members.
*/
CT_MsgComparePopEmailMsgs::~CT_MsgComparePopEmailMsgs()
	{
	// To delete each element in the array
	for(TInt i=0; i < iExpResults.Count(); i++)
		{
		CExpPop3MailInfo* pExp = iExpResults[i];
		delete pExp;
		pExp=NULL;
		}
	iExpResults.Reset();
	iExpResults.Close();

	delete iSentSelectionList;
	iSentSelectionList=NULL;
	delete iInboxSelectionList;
	iInboxSelectionList=NULL;
	}


/**
doTestStepL()
Performs the comparison of POP messages

@return
Returns the teststep result.
*/
TVerdict CT_MsgComparePopEmailMsgs::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: Compare Pop Email Msgs"));
		
	if(LoadParametersL())
		{
		User::LeaveIfError(iFs.Connect());
		CleanupClosePushL(iFs);
		
			// Normally do not push an object that is a data member of another object.
			// But in this case we want to close early if we get a leave.
			// Otherwise the iFs would be open till the thread had begun to die.
		if(TestStepResult() == EPass)
			{
			CMsvEntry* recvMsvEntry = iSharedDataPOP.iSession->GetEntryL(KMsvRootIndexEntryId);
			CleanupStack::PushL(recvMsvEntry);
			CMsvEntry* sentMsvEntry = iSharedDataPOP.iSession->GetEntryL(KMsvRootIndexEntryId);
			CleanupStack::PushL(sentMsvEntry);

			for(TInt i = 0;i < iInboxSelectionList->Count();i++)
				{
				recvMsvEntry->SetEntryL(iInboxSelectionList->At(i));
				TMsvEntry recvEntry = recvMsvEntry->Entry();
				for(TInt j = 0;j < iSentSelectionList->Count();j++)
					{
					sentMsvEntry->SetEntryL(iSentSelectionList->At(j));
					TMsvEntry sentEntry = sentMsvEntry->Entry();
					if(recvEntry.iDescription.Compare(sentEntry.iDescription) == 0) 
						{
						CExpPop3MailInfo* pExpMailInfo = NULL;
						for(int k = 0;k < iExpResults.Count();k++)
							{
							if(iExpResults[k]->GetDescription().Compare(recvEntry.iDescription) == 0)
								{
								pExpMailInfo =  iExpResults[k];
								break;
								}
							}
						if(! pExpMailInfo)
							{
							ERR_PRINTF2(_L("Test %d failed could not retrieve expected mail info"), j+1);
							SetTestStepResult(EFail);
							break;
							}

						if(! DoCompareL(recvEntry, *pExpMailInfo, sentEntry))
							{
							INFO_PRINTF2(_L("Test %d failed"), j + 1);
							SetTestStepResult(EFail);
							}														
						}
					}
				}
			CleanupStack::PopAndDestroy(2, recvMsvEntry); // recvMsvEntry, sentMsvEntry
			}		
		CleanupStack::PopAndDestroy();
		}		
	INFO_PRINTF1(_L("T_ComparePopEmailMsgs Completed"));
	return TestStepResult();
	}

