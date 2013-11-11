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
// CheckChildrenCountImap
// [Paramaters]
// ImapAccountName			:Name of the Imap account.
// ExpectedChildrenCount	:Expected number of children to be present in the
// folder.
// FolderName				:Name of the remote folder under the created Imap
// account whose entries count needs to checked.
// NotEqualCount            :This is a flag set to 1 if true or set to zero if false.
// This flag is set to true if the actual number of children,
// is expected not to equal the expected number of children. 
// (Possibilities of use: scenario if actual children is expected to be less
// than the expected number of children, for example when synchronising an account
// and cancelling that synchronisation - here the actual number of children
// within the inbox will be less than total number of emails)
// FromAddress :If set, then only the children whose 'From' address matches
// this string are counted.
// Subject :If set, then only the children whose 'Subject' matches
// this string are counted.
// Obtains a selection of the children entries under the remote folder and obtains its count.
// Checks if children count is as expected. If the count does not match, the test step
// fails.
// 
//

//User includes 
#include "T_CheckChildrenCountImap.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

// Epoc include 
#include <msvapi.h>

// Literals used 
_LIT(KImapAccountName,	"ImapAccountName");
_LIT(KExpCount,			"Expectedcount");
_LIT(KFolderName,		"FolderName");
_LIT(KNotEqualFlag,"NotEqualCount");
_LIT(KFromAddress, "FromAddress");
_LIT(KSubject, "Subject");

/**
  Function : CT_MsgCheckChildrenCountImap
  @param	: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  Description : Constructor
  @return : N/A
*/
CT_MsgCheckChildrenCountImap::CT_MsgCheckChildrenCountImap(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCheckChildrenCountImap);
	}
	
	
/**
  Function : doTestStepL
  Description : Reads the remote foldername and the expected count from the ini file.
				Obtains the count of the children entries under the given folder.
				Verifies whether it matches with the expected count read from the ini file 
  @return : TVerdict - Test step result
			EPass, if the count matches
			EFail if the count does not match
  @leave  :	KErrNotFound	Invalid local folder name specified
*/
TVerdict CT_MsgCheckChildrenCountImap::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step :CheckChildrenCountImap "));
	
	//Reads the expected children count from ini file 
	TPtrC	folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Reads the IMAP account name from the ini file 
		TPtrC	imapAccountName;
		if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
			{
			ERR_PRINTF1(_L("Imap account name is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Gets the Id of the remote folder based on the name specified 
			
			TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
 						
 			TMsvId folderId;
 			if(folderName.Compare(imapAccountName)==0)
 				{
 				folderId = imapServiceId;
 				}
 			else
 				{
 				// Get the Id of the remote folder
 				folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(iSharedDataIMAP.iSession,imapAccountName,folderName);	
 				}
												
			INFO_PRINTF2(_L(" The folder Id is %d"),folderId);

			//If the folder Id is KErrNotFound,then the folder name is invalid 
			if ( folderId == KErrNotFound )
				{
				ERR_PRINTF1(_L("Invalid folder name specified"));
				SetTestStepResult(EFail);
				}
			//If a valid folder name is specified then reads the expected count from the ini file 
			else
				{
				TInt	expectedChildrenCount;
				// Reads the expected children count from ini file 
				if(!GetIntFromConfig( ConfigSection(), KExpCount, expectedChildrenCount))
					{
					ERR_PRINTF1(_L("Expected number of children not specified"));
					SetTestStepResult(EFail);
					}	
				else
					{
					//Creates a selection of the children entries under the given folder and verifies the count
					CMsvEntrySelection* childrenSelection = CT_MsgUtils::CreateChildrenSelectionL(iSharedDataIMAP.iSession,folderId);
					CleanupStack::PushL(childrenSelection);

					TInt	actualCount = childrenSelection->Count();

					// If we only want to match the children with a specific 'From'
					// address, remove any that don't match
					TPtrC fromAddress;
					if (GetStringFromConfig(ConfigSection(), KFromAddress, fromAddress))
						{
						INFO_PRINTF1(_L("Matching against 'From' address"));

						TMsvId tempServiceId;
						TMsvEntry tempEntry;
						for (TInt child = 0; child < childrenSelection->Count(); ++child)
							{
							if (iSharedDataIMAP.iSession->GetEntry(childrenSelection->At(child), tempServiceId, tempEntry) == KErrNone)
								{
								if (tempEntry.iDetails.Compare(fromAddress) != 0)
									{
									--actualCount;
									}
								}
							}
						}

					// If we only want to match the children with a specific 'Subject',
					// remove any that don't match
					TPtrC subject;
					if (GetStringFromConfig(ConfigSection(), KSubject, subject))
						{
						INFO_PRINTF1(_L("Matching against 'Subject'"));

						TMsvId tempServiceId;
						TMsvEntry tempEntry;
						for (TInt child = 0; child < childrenSelection->Count(); ++child)
							{
							if (iSharedDataIMAP.iSession->GetEntry(childrenSelection->At(child), tempServiceId, tempEntry) == KErrNone)
								{
								if (tempEntry.iDescription.Compare(subject) != 0)
									{
									--actualCount;
									}
								}
							}
						}

					//  Matches the expected count with the actual count 
					//  If the count does not match, fails the test, unless Not equal flag is set
					//
					TInt aNotEqualFlag=0;
					GetIntFromConfig(ConfigSection(),KNotEqualFlag,aNotEqualFlag);
					
					if (aNotEqualFlag==0)
						{
						if ( expectedChildrenCount != actualCount )
							{
							ERR_PRINTF3(_L("Children count is not equal to expected count, expected : %d, actual: %d "),expectedChildrenCount, actualCount);
							SetTestStepResult(EFail);
							}
							// If the count matches the expected count then the test step passes 
							else
							{
							INFO_PRINTF3(_L("Children count is equal to expected count, expected : %d, actual: %d "),expectedChildrenCount, actualCount);
							}
							
						}
					else
						{ 
					 	//  Matches the expected count with the actual count 
					 	// If the actual count is less than expected count , test passes	
						if (actualCount< expectedChildrenCount  )
							{
							INFO_PRINTF3(_L("Children count is less than expected count, expected : %d, actual: %d "),expectedChildrenCount, actualCount);
							}
							// If the count matches the expected count then the test step passes 
							else
							{
							ERR_PRINTF3(_L("Children count is not LESS than  expected count, expected : %d, actual: %d "),expectedChildrenCount, actualCount);
							SetTestStepResult(EFail);
							}				
						}
					CleanupStack::PopAndDestroy(childrenSelection);
					}
				}
			}
		}	
	return TestStepResult();
	}
