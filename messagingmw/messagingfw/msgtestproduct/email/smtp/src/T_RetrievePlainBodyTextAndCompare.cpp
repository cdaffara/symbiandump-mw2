// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// RetrievePlainBodyTextAndCompare
// [Parameters]
// ServiceType			:	Specifies the service type(IMAP or POP) under which message exists.
// ImapAccountName		:	If service type is IMAP4, specify the IMAP account name that holds the
// message.
// ParentFolderName	:	IMAP4 folder name that holds the message.
// PopAccountName		:	If the the sevice type is POP3, specify POP account name.
// SubjectOfExistingMsg:	Sbject line of the existing message to which forward message
// would be creted.
// EmailFileName		:	File name from which existing message has been created.
// ChunkSize			:	Chunk size to be used for retriving body text of a message.
// Searches for the message with the specified subject and retrives the body part of
// the message and write it to the RetrivedBody.txt file.
// [APIs Used]
// CImEmailMessage::OpenPlainBodyTextForReadL
// CImPlainBodyText::NextChunkL
// NOTE : Comparison part has been removed as it would success only when 
// message creation and fetching both uses same set of character set(8 bit or 16 bit).
// However comaprison can be done manually using the text comparison tools(e.g, Beyond Compare)
// 
//

/**
 @file
*/



// User Includes
#include "T_RetrievePlainBodyTextAndCompare.h"
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>
#include <t_utils.h>


// Epoc Include
#include <miutmsg.h>
#include <cimplainbodytext.h>


// Literals Used
_LIT(KServiceType, "ServiceType");

_LIT(KImapAccountName, "ImapAccountName");
_LIT(KParentFolderName, "ParentFolderName");

_LIT(KPopAccountName,"PopAccountName");

_LIT(KSubjectOfExistingMsg,"SubjectOfExistingMsg");
_LIT(KEmailFileName, "EmailFileName");
_LIT(KChunkSize, "ChunkSize");

_LIT(KRetrivedBody, "c:\\msgtest\\RetrivedBody.txt"); //File used to hold the retrive body part


const TInt KArrayGranularity = 8;
const TInt KZero={0};
/**
CT_RetrievePlainBodyTextAndCompare
Constructor

@param :
@return : N/A


*/
CT_RetrievePlainBodyTextAndCompare::CT_RetrievePlainBodyTextAndCompare(CT_MsgSharedDataSmtp& aSharedDataSMTP)
: CT_MsgAsyncStepSMTP(aSharedDataSMTP)
	{
	SetTestStepName(KRetrievePlainBodyTextAndCompare);
	}


/**
NewL()
Static factory constructor. Uses two phase 
construction.

@param aSharedDataSMTP
@leave KErrNoMemory
@return
A pointer to the newly created CT_RetrievePlainBodyTextAndCompare object.
*/
CT_RetrievePlainBodyTextAndCompare* CT_RetrievePlainBodyTextAndCompare::NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP)
	{
	CT_RetrievePlainBodyTextAndCompare* self = new(ELeave) CT_RetrievePlainBodyTextAndCompare(aSharedDataSMTP);
	CleanupStack::PushL(self);
	self->ConstructL(); 					 // Call CT_AsyncStep::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}


/**
~CT_RetrievePlainBodyTextAndCompare()
Destructor
*/
CT_RetrievePlainBodyTextAndCompare::~CT_RetrievePlainBodyTextAndCompare()
	{}


/**
ProgressL()

@param aFinal
*/
void CT_RetrievePlainBodyTextAndCompare::ProgressL(TBool aFinal)
	{
	if ( aFinal )
		{
		//	Display final progress iOperation
		}
	else
		{
		//	Display current progress iOperation
		}
	}


/**
CancelStep()
*/
void CT_RetrievePlainBodyTextAndCompare::CancelStep()
	{}

	
/**
doTestStepL()
Opens the body text part with read only mode. Reads the contents of body part chunkwise and write it
to the file  RetrivedBody.txt. Then it comapares the contents of RetrivedBody.txt with the input file.
@return
Returns the test step result
*/
TVerdict CT_RetrievePlainBodyTextAndCompare::doTestStepL()
	{
	INFO_PRINTF1( _L("Test Step : Edit Message Body Text") );	
	TMsvId	entryToBesearched;
	TPtrC serviceType;
	if(!GetStringFromConfig( ConfigSection(), KServiceType, serviceType))
		{
		ERR_PRINTF1(_L("\"ServiceType\" for the message is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		TBuf<4> temp(serviceType);
		temp.UpperCase();						// Makes case insensitive
		// Perform the following if message has been downloaded using IMAP4 service
		if(temp.CompareC(_L("IMAP")) == KZero)
			{
			// Read the IMAP account name from the ini file
			TPtrC imapAccountName;
			if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
				{
				ERR_PRINTF1(_L("ImapAccount Name is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				TPtrC	parentFolderName;
				if(!GetStringFromConfig( ConfigSection(), KParentFolderName, parentFolderName))
					{
					ERR_PRINTF1(_L("Parent Folder is not specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					
					// retrieves the folder Id based on the remote service folder name read from the ini file
					entryToBesearched = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataSMTP.iSession, imapAccountName, parentFolderName);
					if( entryToBesearched == KErrNotFound)
						{
						ERR_PRINTF1(_L("Invalid Remote folder name specified"));
						SetTestStepResult(EFail);
						}
					else
						{
						INFO_PRINTF2(_L("The parent folder Id is %d"),entryToBesearched );
						}
					}
				}
			}
		// Perform the following if message has been downloaded using POP3 service 	
		else if(temp.CompareC(_L("POP")) == KZero)
			{
			TPtrC popAccountName;
			if(!GetStringFromConfig( ConfigSection(), KPopAccountName, popAccountName))
				{
				ERR_PRINTF1(_L("PopAccount Name is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				// Retrieves the Pop service Id for the given Pop account
				entryToBesearched = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)popAccountName);
				INFO_PRINTF2(_L("Pop service id is %d"), entryToBesearched);

				// Fails the test step if an invalid POP account is specified 
				if(entryToBesearched == KMsvNullIndexEntryId)
					{
					ERR_PRINTF1(_L("Invalid POP account name specified"));
					SetTestStepResult(EFail);
					}
				}
			}
		else
			{
			ERR_PRINTF1(_L("\"ServiceType\" is not specified: It should be IMAP or POP"));
			SetTestStepResult(EFail);
			}
		}

	if(TestStepResult() == EPass)
		{
		TPtrC subjectOfExistingMsg;	
		if( !GetStringFromConfig(ConfigSection(), KSubjectOfExistingMsg, subjectOfExistingMsg))
			{
			ERR_PRINTF1(_L("\"SubjectOfExistingMsg\" is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Retrieves the message Id based on the message subject from the given remote service folder
			TMsvId messageId;
			messageId = CT_MsgUtils::SearchMessageBySubjectL(iSharedDataSMTP.iSession, entryToBesearched, subjectOfExistingMsg, ETrue);
			if( messageId == KMsvNullIndexEntryId)
				{
				ERR_PRINTF1(_L("The given message is not found"));
				SetTestStepResult(EFail);
				}
			// Message found
			else
				{
				TPtrC emailFileName;
				if(!GetStringFromConfig( ConfigSection(), KEmailFileName, emailFileName))
					{
					ERR_PRINTF1(_L("Parent Folder is not specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					INFO_PRINTF2(_L("The Message Id is %d"),messageId );

					// Setting the current context to the parent of the mesage
					CMsvEntry*	messageEntry = CMsvEntry::NewL(*iSharedDataSMTP.iSession, messageId , TMsvSelectionOrdering());
					CleanupStack::PushL(messageEntry);
					messageEntry->SetEntryL(messageId);
					CImEmailMessage *emailMessage = CImEmailMessage::NewLC(*messageEntry);
					TInt chunkSize(1);
					GetIntFromConfig(ConfigSection(), KChunkSize, chunkSize);
					// Open the body text part for read only purpose
					CImPlainBodyText* bodytext = emailMessage->OpenPlainBodyTextForReadL(CImEmailMessage::EThisMessageOnly, chunkSize);
					CleanupStack::PushL(bodytext);
			

					HBufC*  fileData =  HBufC::NewLC(chunkSize);
					TPtr readData = fileData->Des();
					RFs	fs;
					User::LeaveIfError(fs.Connect());
					CleanupClosePushL(fs);

					RFile	file;
					User::LeaveIfError(file.Replace(fs,KRetrivedBody,EFileWrite));
					CleanupClosePushL(file);
					TInt count = 0;
					// Retrieve and store the body text to a file
					do
						{
						bodytext->NextChunkL(readData);
						_LIT(KServerMessage, "*This message was transferred with a trial version of CommuniGate(tm) Pro*\r\n");
						TInt position = readData.Find(KServerMessage);// readData.Find(KServerMessage);
						if(position != KErrNotFound && count < 1)
							{
							count++;
							readData.Delete(position, KServerMessage().Length());
							TPtrC8 inFileBuf8((TUint8*)(readData.Ptr()), readData.Length()*2);
							file.Write(inFileBuf8);
							}
						else
							{
							TPtrC8 inFileBuf8((TUint8*)(readData.Ptr()), readData.Length()*2);
							file.Write(inFileBuf8);
							}
						}while(readData.Length()>0);
					file.Close();
					CleanupStack::PopAndDestroy(6, messageEntry); // file, fs, fileData, bodytext, emailMessage, messageEntry
					}
				}
			}
		}
	return TestStepResult();	
	}

