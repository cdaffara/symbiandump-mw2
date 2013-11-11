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
// CreateSmtpMessageFromEmailFile
// [Paramaters]
// ParentFolderName	: Name of the local folder under which the message 
// needs to be created
// EmailFileName		: Email file from which the message is to be created
// KilobytesExtra		: Number of lines of 1024 characters to add to body text
// Default value is 0.
// SmtpAccountName		: SMTP account name for the SMTP service that would be used for sending the message.
// If SMTP account name is not specified then default SMTP service ID would be used.
// [Test Step Description]
// Creates an Email message reading the contents from the email file specified
// from the config file.
// [APIs Used]
// CMsvEntry::SetEntryL
// 
//

/**
 @file
*/
	
// User include
#include "ImCltCvRecv.h"
#include <t_utilscentralrepository.h>
#include "T_CreateSmtpMessageFromEmailFile.h"
#include <t_utilsenumconverter.h>


// Literals Used
_LIT(KSmtpAccountName, "SmtpAccountName");
_LIT(KEmailFileName, "EmailFileName");
_LIT(KParentFolderName,"ParentFolderName");
_LIT(KKilobytesExtra, "KilobytesExtra");
_LIT(KOverRideAddress, "OverRideAddress");

// Maximum length of a line read from Email file
const TInt KMaxLenghtOfLine = 1024;
const TInt KMaxLenghtOfChar = 1;

/**
CT_MsgCreateSmtpMessageFromEmailFile()
Sets the test step name to KCreateSmtpMessageFromEmailFile

@param aSharedDataSMTP

*/
CT_MsgCreateSmtpMessageFromEmailFile::CT_MsgCreateSmtpMessageFromEmailFile(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:	CT_MsgSyncStepSMTP(aSharedDataSMTP)
	{
	SetTestStepName(KCreateSmtpMessageFromEmailFile);
	}


/**
NewL()
Static factory constructor. Uses two phase 
construction.

@param aSharedDataSMTP
@leave KErrNoMemory
@return
A pointer to the newly created CT_MsgCreateSmtpMessageFromEmailFile object.
*/
CT_MsgCreateSmtpMessageFromEmailFile* CT_MsgCreateSmtpMessageFromEmailFile::NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP)
	{
	CT_MsgCreateSmtpMessageFromEmailFile* self = new(ELeave) CT_MsgCreateSmtpMessageFromEmailFile(aSharedDataSMTP);
	CleanupStack::PushL(self);
	CleanupStack::Pop(self);
	return self;
	}


/**
~CT_MsgCreateSmtpMessageFromEmailFile()
Destructor
*/
CT_MsgCreateSmtpMessageFromEmailFile::~CT_MsgCreateSmtpMessageFromEmailFile()
	{
	}

	
/**
doTestStepL()
Reads the name of the email file,parent foldername from the ini file
Parses the email file to generate a message under the parent folder 
mentioned.If the number of lines is mentioned in the ini file then adds it to the 
message.If not mentioned takes a default of 0.

@return
Returns the test step result
*/
TVerdict CT_MsgCreateSmtpMessageFromEmailFile::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: CreateSmtpMessageFrom email file"));
	TPtrC emailFileName;	
	// Read the name of the email file from the ini
	if(!GetStringFromConfig(ConfigSection(),KEmailFileName,emailFileName))
		{
		ERR_PRINTF1(_L("Email file name not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		TInt numKilobytesExtra = 0;
		GetIntFromConfig( ConfigSection(), KKilobytesExtra, numKilobytesExtra);

		TPtrC parentFolderName;
		if(!GetStringFromConfig( ConfigSection(), KParentFolderName, parentFolderName))
			{
			ERR_PRINTF1(_L("Parent Folder is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Retrieves the folder Id based on the local folder name read from the ini file
			TMsvId parentFolderId = CT_MsgUtilsEnumConverter::FindFolderIdByName(parentFolderName);
			if( parentFolderId == KErrNotFound)
				{
				ERR_PRINTF1(_L("Invalid local folder name specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				INFO_PRINTF2(_L("The parent folder Id is %d"),parentFolderId );

				// Retrieve the required SMTP service Id
				TMsvId	smtpServiceId(0);
				TPtrC smtpAccountName;
				TInt err;
				if( !GetStringFromConfig(ConfigSection(), KSmtpAccountName, smtpAccountName ))
					{
					TRAP(err, smtpServiceId = CT_MsgUtilsCentralRepository::GetDefaultSmtpServiceIdL());
					}
				else
					{
					TRAP(err, smtpServiceId = CT_MsgUtilsCentralRepository::GetSmtpServiceIdL((TDes&)smtpAccountName));
					}
				if(smtpServiceId == KMsvNullIndexEntryId)
					{
					ERR_PRINTF1(_L("Invalid account name specified"));
					SetTestStepResult(EFail);
					}
				else
					{
					if(err != KErrNone)
						{
						ERR_PRINTF2(_L("Failure while getting the default SMTP Service Id.  error = %d"),err);
						SetTestStepResult(EFail);
						}
					else
						{	
						INFO_PRINTF2(_L("The default Smtp serviceId is %d"),smtpServiceId );

						// Set the context to the folder in which message has to be created
						CMsvEntry*	entry = CMsvEntry::NewL(*iSharedDataSMTP.iSession,parentFolderId,TMsvSelectionOrdering());
						CleanupStack::PushL(entry);
						entry->SetEntryL(parentFolderId);

						RFs	fs;
						User::LeaveIfError(fs.Connect());
						CleanupClosePushL(fs);

						CImCltRecvConvert* recvConvert = CImCltRecvConvert::NewLC(fs,entry,KUidMsgTypeSMTP,
																			   smtpServiceId);
						recvConvert->SetMsvId(parentFolderId);
					
						// Read one line of text from the file, parse the line and set the fields of the email 
						// message based on the contents of the parsed line.								   
						
						// Open the Email file in read mode
						RFile	file;
						User::LeaveIfError(file.Open(fs,emailFileName,EFileRead));
						CleanupClosePushL(file);

						TBuf8<KMaxLenghtOfLine> line;
						TBuf8<KMaxLenghtOfChar> aChar;				// To read one character from the file
						TBool finished = FALSE;

						recvConvert->ResetL();

						// Parse each line from the 

						do
							{
						 	line.FillZ();
						 	line.SetLength(0);
						 	// Read one line from email file
						 	do 
						  		{
						  		file.Read(aChar, 1);
						  		if(aChar.Length())
						  			{
									line.Append(aChar);
									}
						  		else
									{
									finished = TRUE;
									}
						  		}while(aChar.Length() && aChar[0] != 0x0A);
							if(!line.Length())
								{
								break;
								}
								// Parse the line for the fields of the email and store them
							recvConvert->ParseNextFieldL(line);
							}while(!finished);
					
							// For a large email, pad the email with extra characters
							if(numKilobytesExtra > 0)
								{
								const TChar KPadChar = 'L';
								line.Fill(KPadChar, 1024);
								for(TInt i = 0; i < numKilobytesExtra; i++)
									{
									recvConvert->ParseNextFieldL(line);
									}
								}
	
						// Complete the message creation by setting all the required fields and 
						// storing the email on to the message store.
						// The created email's ID is returned by MessageCompelteL function
						//

						TMsvId messageId;
						messageId = recvConvert->MessageCompleteL();
						
						if(messageId == 0)
							{
							User::Leave(KErrUnknown);
							}
						TBool overrideAddr = ETrue; // Default is to override.
						GetBoolFromConfig(ConfigSection(), KOverRideAddress, overrideAddr);
						// If the recipients email address in the message need to be changed to SMTP service's
						// email address field, "overrideAddr" should be set to ETrue.
						if(overrideAddr)
							{
							CImSmtpSettings* settings = new(ELeave) CImSmtpSettings();
							CleanupStack::PushL(settings);
					 		
					 		CEmailAccounts* accounts = CEmailAccounts::NewLC();
					 		TSmtpAccount smtpAccount;
					 		accounts->GetSmtpAccountL(smtpServiceId, smtpAccount);
							accounts->LoadSmtpSettingsL(smtpAccount, *settings);
							CleanupStack::PopAndDestroy(accounts);
							
							HBufC* emailAddress=settings->EmailAddress().AllocL();
							CleanupStack::PushL(emailAddress);
							
							entry->SetEntryL(messageId);
								
							CMsvStore* store = entry->EditStoreL();
							CleanupStack::PushL(store);
							
							CImHeader* header = CImHeader::NewLC(); 

							header->RestoreL(*store); // Retrieves the email message header
							
							header->ToRecipients().Reset();
							header->ToRecipients().AppendL(*emailAddress); // Modification of the e-mail ID
							
							header->StoreL(*store);
							store->CommitL();
							
							CleanupStack::PopAndDestroy(3, emailAddress);
							CleanupStack::PopAndDestroy(settings);
							}
						INFO_PRINTF2(_L("Created a email from file %S"),&emailFileName);
						INFO_PRINTF2(_L("The id of the message created is %d"),messageId);
							
						CleanupStack::PopAndDestroy(4, entry); // file, recvConvert, fs, entry
						}
					}
				}
			}
		}
	return TestStepResult();
	}
		
		
		
		
