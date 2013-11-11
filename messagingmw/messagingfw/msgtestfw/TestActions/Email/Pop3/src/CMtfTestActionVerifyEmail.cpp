// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// VerifyEmail
// [Action Parameters]
// CMsvSession&			session   <input>			: Reference to the session. 
// CMsvEntrySelection&	SentMailSelection <input>	: Reference to the 
// sent mail selection		
// CMsvEntrySelection&	RecvMailSelection <input>	: Reference to the 
// received mail selection
// TInt					Number of mails under test
// CExpMailInfo			A list of expected results
// [Action Description]
// Compares the recieved mails with the expected number s of lines to test POP3 TOP command
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file
*/
// 
#include <e32std.h> 
#include <msvstd.h>
#include <msvapi.h>
#include <txtrich.h>
#include <mtclreg.h>
#include <miutmsg.h>
#include <imcm.rsg>


#include "CMtfTestActionUtils.h"
#include "CMtfTestActionVerifyEmail.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"
#include "TMtfTestParameterType.h"
#include "CPop3MailInfo.h"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "timrfc822datefield.h"
#endif

/**
  Function : NewL
  Description : Standard symbian 2 phase constructor
  @internalTechnology
  @param aMtfTestCase - CMtfTestCase test case reference for base class constructor
  @param aActionParameters - pointer to the parameters for this test action
  @return  A CMtfTestAction pointer to the newly created CMtfTestActionVerifyEmail object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionVerifyEmail::NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters )
	{
	CMtfTestActionVerifyEmail* self = new (ELeave) CMtfTestActionVerifyEmail(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : CMtfTestActionVerifyEmail constructor
  Description : Private constructor - must not leave
  @internalTechnology
  @param  aTestCase - CMtfTestCase reference for the base class constructor
  @return N/A
  @pre none
  @post none
*/
CMtfTestActionVerifyEmail::CMtfTestActionVerifyEmail(CMtfTestCase& aTestCase) : 
																				CMtfSynchronousTestAction(aTestCase),	
																				iSession(NULL),
																				iSentSelectionList(NULL),
																				iInboxSelectionList(NULL)
	{
	}

/**
  Function : CMtfTestActionVerifyEmail destructor
  Description : Destructor
  @internalTechnology
  @param  N/A
  @return N/A
  @pre none
  @post none
*/
CMtfTestActionVerifyEmail::~CMtfTestActionVerifyEmail()
	{
	iExpResults.Reset();
	iExpResults.Close();
	}

/**
  Function : LoadParametersL
  Description : Loads the test action parameters 
  @internalTechnology
  @param  none
  @return ETrue if parameters loaded Efalse on failure
  @pre none
  @post Upon failure TestCase status is EFail
*/
TBool CMtfTestActionVerifyEmail::LoadParametersL()
	{

	iSession	= ObtainParameterReferenceL<CMsvSession> (TestCase(),ActionParameters().Parameter(0));
	if( ! iSession )
		{
		TestCase().ERR_PRINTF1(_L("Invalid session object"));
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}
	iSentSelectionList = ObtainParameterReferenceL<CMsvEntrySelection> (TestCase(), ActionParameters().Parameter(1)); 
	if( ! iSentSelectionList )
		{
		TestCase().ERR_PRINTF1(_L("Invalid sentSelectionList"));
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}
	iInboxSelectionList = ObtainParameterReferenceL<CMsvEntrySelection> (TestCase(), ActionParameters().Parameter(2)); 
	if( ! iInboxSelectionList )
		{
		TestCase().ERR_PRINTF1(_L("Invalid inboxSelectionList"));
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}
	
	if( iInboxSelectionList->Count() != iSentSelectionList->Count() )
		{
		TestCase().ERR_PRINTF3( _L("iInboxSelectionList->Count(%d) != iSentSelectionList->Count(%d)"),
								 iInboxSelectionList->Count(), iSentSelectionList->Count() );
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}

	TInt numTests = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3));
	if( numTests != iInboxSelectionList->Count() )
		{
		TestCase().ERR_PRINTF1(_L("iNumTests != iInboxSelectionList"));
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}
	iExpResults.Reset();
	for( TInt i=0;i<numTests;i++) 
		{
		CExpPop3MailInfo* pExp = ObtainParameterReferenceL<CExpPop3MailInfo> ( TestCase(), ActionParameters().Parameter(i+4) );
		if( ! pExp )
			{
			TestCase().ERR_PRINTF1(_L("Invalid expected mail Info"));
			TestCase().SetTestStepResult(EFail);
			return EFalse;
			}
		else
			{
			iExpResults.Append( pExp );
			}
		}

	return ETrue;
	}

/**
  Function : ExecuteActionL
  Description : Test framework entry point for CMtfTestActionVerifyEmail test action
  @internalTechnology
  @param  none
  @return void
  @pre none  
  @post TestCase status EPass if tests passed otherwise EFail
*/
void CMtfTestActionVerifyEmail::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionVerifyEmail);
	if( LoadParametersL() )
		{
		User::LeaveIfError( iFs.Connect() );
		CleanupClosePushL(iFs);	
			// Normally do not push an object that is a data member of another object.
			// But in this case we want to close early if we get a leave.
			// Otherwise the iFs would be open till the thread had begun to die.
		
		if( TestCase().TestStepResult() == EPass)
			{
			CheckPartialDownloadedFlag();
			CMsvEntry* recvMsvEntry = iSession->GetEntryL(KMsvRootIndexEntryId);
			CleanupStack::PushL( recvMsvEntry );
			CMsvEntry* sentMsvEntry = iSession->GetEntryL(KMsvRootIndexEntryId);
			CleanupStack::PushL( sentMsvEntry );

			for( TInt i=0;i<iInboxSelectionList->Count();i++)
				{
				recvMsvEntry->SetEntryL( iInboxSelectionList->At(i) );
				TMsvEntry recvEntry = recvMsvEntry->Entry();
				for( TInt j=0;j<iSentSelectionList->Count();j++)
					{
					sentMsvEntry->SetEntryL( iSentSelectionList->At(j) );
					TMsvEntry sentEntry = sentMsvEntry->Entry();
					if( recvEntry.iDescription.Compare( sentEntry.iDescription ) == 0 ) 
						{
						CExpPop3MailInfo* pExpMailInfo = NULL;
						for( int k = 0;k<iExpResults.Count();k++)
							{
							if( iExpResults[k]->GetDescription().Compare( recvEntry.iDescription ) == 0 )
								{
								pExpMailInfo =  iExpResults[k];
								break;
								}
							}
						if( ! pExpMailInfo )
							{
							TestCase().ERR_PRINTF2(_L("Test %d failed could not retrieve expected mail info"), j+1 );
							TestCase().SetTestStepResult(EFail);
							break;
							}


						if( ! DoCompareL( recvEntry, *pExpMailInfo, sentEntry ) )
							{
							TestCase().INFO_PRINTF2(_L("Test %d failed"),  j+1 );
							TestCase().SetTestStepResult(EFail);
							}														
						}
					}
				}
			CleanupStack::PopAndDestroy(2, recvMsvEntry ); // recvMsvEntry, sentMsvEntry
			}
			
		CleanupStack::PopAndDestroy(&iFs);
		}
		
	TestCase().INFO_PRINTF1( _L("Test Action CMtfTestActionVerifyEmail Completed") );
	TestCase().ActionCompletedL(*this);	
	}

/**
  Function : CheckPartialFlagOfRecvEntry
  Description : checks the Partialdownloaded flag 
*/
void CMtfTestActionVerifyEmail::CheckPartialDownloadedFlag()
{
	CMsvEntry* recvMsvEntry = iSession->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL( recvMsvEntry );
	TMsvEmailEntry entry;
	TMsvId service = KMsvNullIndexEntryId;
	for( TInt i = 0; i < iInboxSelectionList->Count(); ++i)
		{
		iSession->GetEntry((iInboxSelectionList->At(i)), service, entry);
		if(entry.PartialDownloaded())
			{
			TestCase().INFO_PRINTF1( _L("Partial downloaded flag is ETrue") );	
			}
		else
			{
			TestCase().INFO_PRINTF1( _L("Partial downloaded flag is EFalse") );
			}
		}
	CleanupStack::PopAndDestroy();
}

/**
  Function : DoCompareL
  Description : Comparison of 2 email messages identified by TMsvEntry
  @internalTechnology
  @param  aRecvEntry - The recieved email identifier
  @param aMailInfo - expected state of the recieved email
  @param aSentEntry - Identifier of the original sent email foir comparison purposes
  @return ETrue if emails match EFalse otherwise
  @pre Email must have been sent and recieved correctly and parameters loaded ok
  @post none
*/
TBool CMtfTestActionVerifyEmail::DoCompareL( TMsvEntry& aRecvEntry, CExpPop3MailInfo& aMailInfo, TMsvEntry& aSentEntry )
	{
	TBool ret = ETrue;
	TBool footerExists = EFalse;
	TInt footersize = 0;
		
			
	TInt nLines = GetMessageLinesL(aRecvEntry, KUidMsvEmailTextEntry, footerExists, footersize);
	if( nLines != aMailInfo.GetNumLinesBodyText() )
		{
		TestCase().ERR_PRINTF3(_L("Lines of body text wrong actual = %d expected = %d!"), nLines, aMailInfo.GetNumLinesBodyText() );
		ret = EFalse;
		}
	nLines = GetMessageLinesL(aRecvEntry, KUidMsvEmailHtmlEntry, footerExists, footersize );

	if( nLines != aMailInfo.GetNumLinesHtml() )
		{		
		TestCase().ERR_PRINTF3(_L("HTML size wrong actual = %d expected = %d!"), nLines, aMailInfo.GetNumLinesHtml() );
		ret = EFalse;
		}

	if( ! CheckAttachmentsL( aRecvEntry, aMailInfo, aSentEntry ) )
		{
		TestCase().ERR_PRINTF1(_L("Attachment comparison failed !"));
		ret = EFalse;
		}

	if( ! footerExists && aMailInfo.GetFooterExpected() )
		{
		TestCase().ERR_PRINTF1(_L("An expected footer was missing !"));
		ret = EFalse;
		}
	else if( footerExists && ! aMailInfo.GetFooterExpected() )
		{
		TestCase().ERR_PRINTF1(_L("An unexpected footer was found !"));
		ret = EFalse;
		}
	else if(aMailInfo.GetFooterExpected() && aMailInfo.GetFooterSize()!=0 
			&& aMailInfo.GetFooterSize() != footersize)
		{
			TestCase().ERR_PRINTF3(_L("KB left on server size wrong = %d expected = %d!"), footersize, aMailInfo.GetFooterSize());
			ret = EFalse;			
		}
	return ret;
	}




//
// CAttachmentItem : Used to ensure that MMsvAttachmentManager is not destroyed until we want it to be. 
// 		To do this we need to preserve certain objects CImEmailMessage, which if disappeared 
//		would invalidate MMsvAttachmentManager. 
//		But we also want to delete these objects in a contraolled manner 
//		after we have finished with MMsvAttachmentManager.

void CAttachmentItem::ConstructL( CMsvSession& aSession, TMsvEntry aEntry)
	{
		
	iMsvEntry = aSession.GetEntryL(aEntry.Id());

	iMsvEntry->SetEntryL(aEntry.Id());
	iEmailMsg = CImEmailMessage::NewL(*iMsvEntry);
		
	CMsvOperationActiveSchedulerWait* waiter = CMsvOperationActiveSchedulerWait::NewLC();
 	iEmailMsg->GetAttachmentsListL( waiter->iStatus, aEntry.Id(),
								   CImEmailMessage::EAllAttachments, 
								   CImEmailMessage::EThisMessageOnly);
	waiter->Start();
	
	CleanupStack::PopAndDestroy( 1 , waiter );
		
	}


CAttachmentItem* CAttachmentItem::NewL( CMsvSession& aSession, TMsvEntry aEntry )
	{
	CAttachmentItem* self=CAttachmentItem::NewLC( aSession, aEntry );
	CleanupStack::Pop(self);
	
	return self;
	}

CAttachmentItem* CAttachmentItem::NewLC( CMsvSession& aSession, TMsvEntry aEntry )
	{
	CAttachmentItem* self=new (ELeave) CAttachmentItem;
	CleanupStack::PushL(self);
	
	self->ConstructL(aSession, aEntry);
	
	return self;
	}

CAttachmentItem::~CAttachmentItem()
	{
	delete iEmailMsg;
	delete iMsvEntry;	
	}


MMsvAttachmentManager& CAttachmentItem::MMsvAttachmentManager() const
	{			
	return (iEmailMsg->AttachmentManager());		
	}
	
	


/**
  Function : CheckAttachmentsL
  Description : Compares attachments for the email messages aRecvEntry and aSentEntry using expected results in aMailInfo
  @internalTechnology
  @param aRecvEntry - an identifier for the recieved email
  @param aMailInfo - expected state of recieved email attachments
  @param aSentEntry - an identifier for the original email
  @return ETrue if attachments match otherwise EFalse
  @pre aRecvEntry and aSentEntry must be valid email identifiers
  @post none
*/
TBool CMtfTestActionVerifyEmail::CheckAttachmentsL( TMsvEntry& aRecvEntry, CExpPop3MailInfo& aMailInfo, TMsvEntry& aSentEntry )
	{
	TBool ret = ETrue;
	CAttachmentItem* recvManager = CAttachmentItem::NewLC( *iSession, aRecvEntry );
	CAttachmentItem* sentManager = CAttachmentItem::NewLC( *iSession, aSentEntry );

	TInt recvCount = recvManager->MMsvAttachmentManager().AttachmentCount();	
	TInt sentCount = sentManager->MMsvAttachmentManager().AttachmentCount();

	TFileName recvFileName;
	TFileName sentFileName;

	if( recvCount == aMailInfo.GetNumAttachments() )
		{
		for( TInt i=0;i<recvCount;i++ )
			{
			for( TInt j=0;j<sentCount;j++ )
				{
				CMsvAttachment* recvAttInfo = recvManager->MMsvAttachmentManager().GetAttachmentInfoL(i);
				CleanupStack::PushL(recvAttInfo);

				CMsvAttachment* sentAttInfo = sentManager->MMsvAttachmentManager().GetAttachmentInfoL(j);
				CleanupStack::PushL(sentAttInfo);

				if( recvAttInfo->AttachmentName().Compare( sentAttInfo->AttachmentName() ) == 0 )
					{
					recvFileName = recvAttInfo->FilePath();
					sentFileName = sentAttInfo->FilePath();
					ret = CompareFilesL( recvFileName, sentFileName );
					}
				CleanupStack::PopAndDestroy(2, recvAttInfo);		
				}
			}
		}
	else
		{
		TestCase().INFO_PRINTF3( _L("Warning - Expected attachments mismatch recieved (%d) expected (%d)"),
								 recvCount, aMailInfo.GetNumAttachments() );
		ret = EFalse;
		}

	CleanupStack::PopAndDestroy(2, recvManager); // recvManager, sentManager
	return ret;
	}


/**
  Function : CompareFilesL
  Description : Performs byte for byte copmparison of the files indicated by aFileName1 and aFileName2
  @internalTechnology
  @param aFileName1 - filename of first file to be compared
  @param aFileName2 - filename of second file to be compared
  @return ETrue if files match, EFalse otherwise
  @pre Filenames must be valid or function will leave
  @post none
*/
TBool CMtfTestActionVerifyEmail::CompareFilesL( TFileName& aFileName1, TFileName& aFileName2 )
	{

	TBool ret = ETrue;
	RFile file1, file2;

	User::LeaveIfError( file1.Open( iFs, aFileName1, EFileRead ) );
	User::LeaveIfError( file2.Open( iFs, aFileName2, EFileRead ) );

	TInt sz1, sz2;
	User::LeaveIfError( file1.Size(sz1) );
	User::LeaveIfError( file2.Size(sz2) );
	
	if( sz1 != sz2 )
		{
		TestCase().ERR_PRINTF3( _L("File size mismatch 1(%d)   2(%d)"), sz1, sz2 );
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
			if( char1.Compare( char2 ) != 0 )
				{
				ret = EFalse;
				break;
				}
			}
		while( len1 && len2 );
		}
	file1.Close();
	file2.Close();
	return ret;
	}

/**
  Function : GetMessageLinesL
  Description : Counts the number of lines in a message's HTML or body text
  @internalTechnology
  @param aEntry - email message identifier
  @param aUid - request identifier KUidMsvEmailHtmlEntry or KUidMsvEmailTextEntry at present
  @param aFooterExists - reference to a boolean variable only used when aUid == KUidMsvEmailTextEntry
  @param aFooterSize - expected KB left on server inserted into footer string if present 
  @return Returns the number of lines in the requested message part
  @pre aEntry must be a valid email identifier
  @post none
*/
TInt CMtfTestActionVerifyEmail::GetMessageLinesL(TMsvEntry& aEntry, TUid aUid, TBool& aFooterExists, TInt &aFooterSize )
	{
	TInt lines = 0; 
	
	TMsvId msgPartIndex = GetMessagePartIndexL(aEntry,aUid);
	
	if(msgPartIndex)
		{
		CMsvEntry* msvEntry = iSession->GetEntryL(msgPartIndex);
		CleanupStack::PushL(msvEntry);
	
		msvEntry->SetEntryL(msgPartIndex);
		if( aUid == KUidMsvEmailTextEntry )
			{
			lines = CountLinesOfBodyTextL( *msvEntry, aFooterExists, aFooterSize );
			}
		else if( aUid == KUidMsvEmailHtmlEntry )
			{
			lines = CountLinesOfHtmlL( *msvEntry );
			}

		CleanupStack::PopAndDestroy(msvEntry);
		}
	return lines;
	}

/**
  Function : CountLinesOfBodyTextL
  Description : Counts the lines of body text in the email aEntry
  @internalTechnology
  @param aEntry - a reference to an object representing the email
  @param aFooterExists - reference to a boolean variable - set to ETrue if the footer string is found in the body text
  @param aFooterSize - expected KB left on server inserted into footer string if present 
  @return number of lines in the body text 
  @pre  aEntry must be a valid email object
  @post none
*/
TInt CMtfTestActionVerifyEmail::CountLinesOfBodyTextL( CMsvEntry& aEntry, TBool& aFooterExists, TInt& aFooterSize)
	{
	_LIT(KServerMessage,"*This message was transferred with a trial version of CommuniGate(r) Pro*");
	TInt lines = 0;
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
		CRichText*	bodyText1=CRichText::NewL( paraFormatLayer1, charFormatLayer1, CEditableText::EFlatStorage, 256);
		CleanupStack::PushL(bodyText1);

		bodyText1->Reset();
		if ( msvStore1->HasBodyTextL())
			{
			msvStore1->RestoreBodyTextL(*bodyText1);
			TUint16 val = CEditableText::ELineBreak;
			TInt n = 0;
			TInt pos = 0;
			for(;;)
				{
				TPtrC bodyText = bodyText1->Read(pos);
				n = bodyText.Find( &val, 1 );
				if( n < 0 )
					break;
				lines++;
				pos += n+1;
				
				//This Check is needed to ignore the disclaimer line introduced by communigate Pro Server
				TInt fpos = bodyText.FindC(KServerMessage);
				if (fpos != KErrNotFound)
					{
					lines--;
					}
				}
			TPtrC	pBt = bodyText1->Read(0);
			aFooterExists = CheckIfFooterMessageExistsL( pBt , aFooterSize);
			}		
		CleanupStack::PopAndDestroy(4, msvStore1);
		}
	return lines;
	}

/**
  Function : CountLinesOfHtmlL
  Description : Counts the number of lines of HTML
  @internalTechnology
  @param aEntry - a reference to an object representing the email
  @return number of lines of HTML
  @pre  aEntry must be a valid email object
  @post none
*/
TInt CMtfTestActionVerifyEmail::CountLinesOfHtmlL( CMsvEntry& aEntry )
	{ 
	TInt lines = 0;
	aEntry.SetEntryL(aEntry.EntryId());
	
	CMsvStore* store = aEntry.ReadStoreL();
	CleanupStack::PushL(store);
	
	MMsvAttachmentManager& attManager = store->AttachmentManagerL();
	
	RFile htmlFile = attManager.GetAttachmentFileL(0);
	CleanupClosePushL(htmlFile);
	
	_LIT8( KFindData, "\r\n" );
	TInt htmlSize = 0;
	User::LeaveIfError( htmlFile.Size(htmlSize) );
	HBufC8* fBuf = HBufC8::NewLC( htmlSize );
	TPtr8 p = fBuf->Des();
	htmlFile.Read( p );
	TInt pos = 0;
	for(;;)
		{
		pos = p.Find( KFindData );
		if( pos < 0 )
			break;
		p = p.Mid(pos+2);
		lines++;
		}
		
	CleanupStack::PopAndDestroy( fBuf );
	CleanupStack::PopAndDestroy(); // htmlFile
	CleanupStack::PopAndDestroy(store);
	

	return lines;
	}

/**
  Function : GetMessagePartIndexL
  Description : Retrieves the part index id for the email identified by aEntry
  @internalTechnology
  @param aEntry - a valid email identifier
  @param aUid - an identifier tfor the type of message part to be retrieved
  @return TMsvId -  message part identifier
  @pre none 
  @post none
*/
TMsvId CMtfTestActionVerifyEmail::GetMessagePartIndexL(TMsvEntry& aEntry, TUid aUid)
	{
	static TBool msgPartFound = EFalse;	
	TMsvId msgPartId = 0;
	CMsvEntry*			baseEntry  = iSession->GetEntryL(aEntry.Id());
	CleanupStack::PushL(baseEntry);
	CMsvEntrySelection*	selection =  baseEntry->ChildrenL();
	CleanupStack::PushL(selection);
	TInt count = selection->Count();
	TInt ind1;
	CMsvEntry* entry = iSession->GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(entry);
	for (ind1=0, msgPartFound=EFalse; ind1 < count; ind1++ )
		{
		if(msgPartFound)
			{
			break;
			}		
		TMsvId	childId  = selection->At(ind1);
		entry->SetEntryL(childId);

		if ( entry->Entry().iType  == aUid)
			{
			msgPartId = childId;
			break;
			}
		else if (entry->Entry().iType  == KUidMsvFolderEntry)
			{
			TMsvEntry ent = entry->Entry();
			msgPartId = GetMessagePartIndexL( ent, aUid);
			}
		}
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(baseEntry);
	return msgPartId;
	}

/**
  Function : CheckIfFooterMessageExistsL
  Description : Checks the email for the presence of the footer string
  @internalTechnology
  @param aRecvBodyText -  reference to a descriptor holding the body text
  @param aFooterSize -  the expected number of KB remaining on the server which has been inserted into the footer string
  @return ETrue if footer string found otherwise EFalse 
  @pre PARTIAL_DOWNLOAD_FOOTER_MESSAGE must be a valid string resource identifier
  @post none
*/
TBool CMtfTestActionVerifyEmail::CheckIfFooterMessageExistsL(TPtrC& aRecvBodyText , TInt& aFooterSize )
	{

	RResourceFile resFile;
	CMtfTestActionUtils::OpenResourceFileL(resFile, iFs, KImEngineResourceFile );	// NB leaves if file not found
	TCleanupItem close( CMtfTestActionUtils::CloseResourceFile, &resFile );
	CleanupStack::PushL( close );
	
	HBufC8* buf = NULL;
	buf = resFile.AllocReadLC( PARTIAL_DOWNLOAD_FOOTER_MESSAGE );
	TResourceReader reader;
	reader.SetBuffer(buf);

	HBufC* resourceBuf = (reader.ReadTPtrC()).AllocL();
	CleanupStack::PushL( resourceBuf );

	TInt len = resourceBuf->Find( _L("%d") );
	const TInt KBufLen = 256;
	TBuf<KBufLen> findBuf;

	if( len == KErrNotFound )
		len = resourceBuf->Length();

	if( len > KBufLen )
		len = KBufLen;
	
	findBuf.Copy( resourceBuf->Ptr(), len );
	len = aRecvBodyText.Find( findBuf );

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

	CMtfTestActionUtils::CloseResourceFile( &resFile );
	CleanupStack::PopAndDestroy(3); // buf, resourceBuf, close
	return ( len != KErrNotFound );
	}
