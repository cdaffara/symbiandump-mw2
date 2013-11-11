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
// AddImap4TransportBufferSizes
// [Paramaters]
// BearerType: Bearer type to set values for
// FetchRequestSize : Value to use for fetch request size
// MaxOutstandingFetchRequests: Value to use for max outstanding fetch requests
// [TestStep Description]
// Sets parameters of Imap4 email accounts
// 
//

#include "T_AddImap4TransportBufferSizes.h"
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>
#include <cemailaccounts.h>
#include <imapset.h>
#include <cimaptransportbuffersizes.h>

/* Literals Used */
_LIT(KBearerTypes, "BearerTypes");
_LIT(KFetchRequestSize, "FetchRequestSize");
_LIT(KMaxOutstandingFetchRequests, "MaxOutstandingFetchRequests");

/**
	Function : CT_MsgAddImap4TransportBufferSizes
	Description : Constructor
	@param : aSharedDataIMAP		Reference to CT_MsgSharedDataImap
	@return : N/A
*/
CT_MsgAddImap4TransportBufferSizes::CT_MsgAddImap4TransportBufferSizes(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KAddImap4TransportBufferSizes);
	}

/**
	Function : ~CT_MsgAddImap4TransportBufferSizes
	Description : Destructor
	@return : N/A
*/
CT_MsgAddImap4TransportBufferSizes::~CT_MsgAddImap4TransportBufferSizes()
	{
	}

/**
	Function : doTestStepL
	Description : Adds a transport buffer sizes rule to the list of rules for
	              the IMAP email accounts.
	@return : TVerdict - Test step result
	@leave : KMsvNullIndexEntryId	Invalid IMAP account name specified
*/
TVerdict CT_MsgAddImap4TransportBufferSizes::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : AddImap4TransportBufferSizes"));

	TBool useEmpty = EFalse;

	TUint32 bearerTypes = 0;
	TInt fetchRequestSize = 0;
	TInt maxOutstandingFetchRequests = 0;

	TPtrC bearerTypesStr;
	if (GetStringFromConfig(ConfigSection(), KBearerTypes, bearerTypesStr))
		{
		bearerTypes = CT_MsgUtilsEnumConverter::ConvertDesToBearerTypes(bearerTypesStr);

		if (!GetIntFromConfig(ConfigSection(), KFetchRequestSize, fetchRequestSize))
			{
			ERR_PRINTF1(_L("Fetch request size not specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}

		if (!GetIntFromConfig(ConfigSection(), KMaxOutstandingFetchRequests, maxOutstandingFetchRequests))
			{
			ERR_PRINTF1(_L("Max outstanding fetch requests not specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
		}
	else
		{
		INFO_PRINTF1(_L("Using empty transport buffer sizes list"));
		useEmpty = ETrue;
		}

	CEmailAccounts *accounts = CEmailAccounts::NewLC();
	CImapTransportBufferSizes* sizes = CImapTransportBufferSizes::NewLC();

	if (!useEmpty)
		{
		TInt pos = 0;
		TRAPD(err, pos = sizes->AddSizesL(0, bearerTypes, fetchRequestSize, maxOutstandingFetchRequests));
	
		if (err || pos < 0)
			{
			INFO_PRINTF3(_L("Failed to add size - Leave error %d, Pos err %d"), err, pos);
			SetTestStepResult(EFail);
			}
		}
		
	accounts->SaveImapTransportBufferSizesL(*sizes);

	CleanupStack::PopAndDestroy(2, accounts);

	return TestStepResult();
	}
