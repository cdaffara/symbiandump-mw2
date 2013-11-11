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
// @file 
// [TestStep Name]
// CheckChildrenCountFlagBase
// [Paramaters]
// ParentFolder				: Folder in which children count is to be verified
// ExpectedChildrenCount	: Expected number of children to be present in the
// folder
// Checks if children count with particular flag set is as expected.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::ChildrenL
// CMsvEntry::SetSortTypeL
// TMsvSelectionOrdering::SetShowInvisibleEntries
// 
//

/* User includes */
#include "T_CheckChildrenCountFlagBase.h"
#include <t_utilsenumconverter.h>
#include <t_utils.h>


/* Literals used */
_LIT(KImapAccountName,	"ImapAccountName");
_LIT(KExpCount,			"Expectedcount");
_LIT(KFolderName,		"FolderName");
_LIT(KFlagName,			"FlagName");
_LIT(KNew,				"New");
_LIT(KComplete,			"Complete");
_LIT(KSeen,				"Seen");
_LIT(KUnread,			"Unread");
_LIT(KDeleted,			"Deleted");



/**
  Function : CT_MsgCheckChildrenCountFlagBase
  Description : Constructor
  @param : aSharedDataIMAP	A reference to CT_MsgSharedDataImap
*/
CT_MsgCheckChildrenCountFlagBase::CT_MsgCheckChildrenCountFlagBase(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCheckChildrenCountFlagBase);
	}



/**
  Function : doTestStepL
  Description : Obtains the count of the entries under the specified parent folder
				for a given flag being set and compare with the expected count
  @return : TVerdict - Test step result
			EPass, if the count matches
			EFail if the count does not match
  @leave  : KErrNotFound	Invalid local folder name specified"
*/
TVerdict CT_MsgCheckChildrenCountFlagBase::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : Check children count"));	
	
	/* Reads the expected children count from ini file */
	TInt expectedChildrenCount;
	if(!GetIntFromConfig( ConfigSection(), KExpCount, expectedChildrenCount))
		{
		ERR_PRINTF1(_L("Expected number of children not specified"));
		SetTestStepResult(EFail);
		}	
	else
		{
		/* Reads the name of the remote folder whose entries need to be compared*/
		TPtrC	folderName;
		if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
			{
			ERR_PRINTF1(_L("Folder name is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			/* Reads the IMAP account name from the ini file */
			TPtrC	imapAccountName;
			if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
				{
				ERR_PRINTF1(_L("Imap account name is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				/* Gets the Id of the remote folder under the specified IMAP account */
				TMsvId	folderId = CT_MsgUtils::GetRemoteFolderIdByNameL(
										iSharedDataIMAP.iSession,
										imapAccountName,folderName);					

				INFO_PRINTF2(_L(" The folder Id is %d"),folderId);

				/* If the folder Id is KErrNotFound,then the folder name is invalid */
				if ( folderId == KErrNotFound )
					{
					ERR_PRINTF1(_L("Invalid local folder name specified"));
					SetTestStepResult(EFail);
					}
				/* If a valid folder is specified then reads the name of the flag */
				else
					{
					TPtrC	flagName;
					if(!GetStringFromConfig( ConfigSection(), KFlagName, flagName))
						{
						ERR_PRINTF1(_L("FlagName is not specified"));
						SetTestStepResult(EFail);
						}
					else
						{
						/* Obtains the count of the children entries under the specified folder
						 * for a given flag being set 
						 */
						TInt count = CountChildrenL(folderId, *iSharedDataIMAP.iSession, flagName);

							/* 
							*  Matches the expected count with the actual count 
							*  If the count does not match, fails the test
							*/
						if (expectedChildrenCount != count)
						    {
						    INFO_PRINTF4(_L("ChildrenCount with Flag %S was %d, expected %d"), &flagName, count, expectedChildrenCount );
							SetTestStepResult(EFail);
							User::Leave(KErrArgument);
							}
						/* If the count matches the expected count then the test step passes */
						else
						    {
							INFO_PRINTF4(_L("CheckChildrenCountWithFlag %S completed with actual count %d,expected count %d."), &flagName, count,expectedChildrenCount);
							}	
						}
					}
				}
			}
		}

	return TestStepResult();
	}




/**
  Function : CountChildrenL
  Description : Obtains a count of the children entries for a given parent entry
  				based on the message type and the flags set.
  @param :	aFolderId		Folder Id whose entries, count needs to be obtained
			aSession		Session object
			aFlagName		The name of the flag which needs to checked
  @return : Returns the count of the children which have the respective flag set.
*/
TInt CT_MsgCheckChildrenCountFlagBase::CountChildrenL(TMsvId aFolderId, CMsvSession& aSession, TPtrC aFlagName)
	{
	TInt childrenCount = 0;

	/* Sets the context on the specified folder whose entries count needs to be obtained */
	CMsvEntry*	entryContext = CMsvEntry::NewL(
									aSession, aFolderId, 
		    						TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(entryContext);

	entryContext->SetEntryL(aFolderId);
	TMsvSelectionOrdering	order;

	order.SetShowInvisibleEntries(ETrue);
	entryContext->SetSortTypeL(order);

	/* Creates a selection object for the children entries */
	CMsvEntrySelection*	selection = entryContext->ChildrenL();
	CleanupStack::PushL(selection);

	/* Obtains the count of the children entries */
	TInt	count = selection->Count();

	/* Loops through the each child entry and sets the context on it */
	for(TInt child=0;child<count;child++)
		{
		entryContext->SetEntryL((*selection)[child]);

		TMsvEmailEntry entry = entryContext->Entry();

		/* If the child entry is a message and if it has the given flag set then increments the
		 * children count */
		if (entry.iType == KUidMsvMessageEntry && CheckFlag(aFlagName,entry) )
			{
			++childrenCount;
			}
		/* 
		 * If the child entry is a folder then follows a recursive pattern to travers further down
		 * till it finds message entries 
		 */
		if (entry.iType==KUidMsvFolderEntry)
			{
			childrenCount +=CountChildrenL((*selection)[child],*iSharedDataIMAP.iSession,aFlagName);
			}
		}
	CleanupStack::PopAndDestroy(2,entryContext); /* selection,entryContext */
	return childrenCount;
	}



/**
  Function : CheckFlag
  Description : Checks if the given flag is set
  @param : aFlagName		The name of the flag which needs to be checked
		   aEntry			The entry object whose entries needs to be checked
  @return : ETrue, if the flag is set
			EFalse if the flag is not set
*/
TBool CT_MsgCheckChildrenCountFlagBase::CheckFlag(TPtrC aFlagName, TMsvEmailEntry& aEntry)
	{
	TBool	result = EFalse;
	/* Checks if the flag New is set*/
	if(aFlagName == KNew)
		{
		result = aEntry.New();
		}
	/* Checks if the flag Complete is set*/
	else if(aFlagName == KComplete)
		{
		result = aEntry.Complete();
		}
	/* Checks if the flag Seen is set*/
	else if(aFlagName == KSeen)
		{
		result = aEntry.SeenIMAP4Flag();
		}
	/* Checks if the flag Unread is set*/
	else if(aFlagName == KUnread)
		{
		result = aEntry.UnreadIMAP4Flag();
		}
	/* Checks if the flag Deleted is set*/
	else if(aFlagName == KDeleted)
		{
		result = aEntry.DeletedIMAP4Flag();
		}
	return result;
	}
