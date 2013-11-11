// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ModifyIndexEntryFlags
// [Paramaters]
// UnReadFlag 
// SetPriority
// Modifies the flags of index entries returned by previous search/sort request.
// [APIs Used]
// 
//

/* User Includes */
#include "t_modifyindexentryflags.h"

/* Epoc includes */
#include <msvsearchsortoperation.h>
#include <t_utilsenumconverter.h>

// Literals used 
_LIT(KUnReadFlag, "UnReadFlag");
_LIT(KSetPriority, "SetPriority");


/**
 Function : CT_MsgModifyIndexEntryFlags
 Description : Constructor
 @param : aSharedDataCommon	    A reference to CT_MsgSharedDataCommon
 @return : N/A
 */
CT_MsgModifyIndexEntryFlags::CT_MsgModifyIndexEntryFlags(CT_MsgSharedDataCommon& aSharedDataCommon) 
: CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName (KModifyIndexEntryFlags);
	}


/**
 Function : doTestStepL
 Description : Verify if message part(s) of the message entries returned by search/sort mechanism are
			   as per the search/sort criteria.
 @return : TVerdict - Test step result
 */
TVerdict CT_MsgModifyIndexEntryFlags::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : ModifyIndexEntryFlags"));
	
	RArray<TMsvEntry> messageEntries = iSharedDataCommon.iSearchSortResultArray;
	TBool unreadFlag = ETrue;
	GetBoolFromConfig(ConfigSection(), KUnReadFlag, unreadFlag);
	TPtrC setPriority;
	GetStringFromConfig(ConfigSection(), KSetPriority, setPriority);

	TMsvPriority priorty = CT_MsgUtilsEnumConverter::ConvertDesToMsgPriority(setPriority);
	CMsvEntry *entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, KMsvRootIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	for(TInt jj = 0; jj < messageEntries.Count(); ++jj)
		{
		entry->SetEntryL(messageEntries[jj].Id());
		TMsvEntry indexEntry = entry->Entry();
		indexEntry.SetUnread(unreadFlag);
		indexEntry.SetPriority(priorty);
		TRAPD(error, entry->ChangeL(indexEntry));
		if (error)
			{
			SetTestStepResult(EFail);
			ERR_PRINTF3(_L("Modification of flags for EntryID %d left with error code %d"), messageEntries[jj].Id(), error);
			break;
			}
		}
	CleanupStack::PopAndDestroy(entry);		
	return TestStepResult();	
	}
